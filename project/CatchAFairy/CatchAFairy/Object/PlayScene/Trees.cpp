//=============================================================================
//	Trees.cpp
//
//	木々のソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "Trees.h"
#include <math.h>
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "Ishikawa/Common/Random.h"
#include "Ishikawa/Math/ITCollision.h"
#include "Ishikawa/Model/ITObjectLoader.h"
#include "Ishikawa/DirectX/Device/ITDevice.h"
#include "CatchAFairy/Shader/User/ShaderVertexTopology.h"
#include "CatchAFairy/Shader/User/PlayCameraEyeFirstShaderUser.h"

using namespace CatchAFairy::Object::PlayScene;


/// <summary>自作モデルデータファイルパス</summary>
const char* const Trees::IT_MODEL_DATA_FILEPATH = "Resources/Models/TestTree004.itobj";
/// <summary>拡散反射テクスチャファイルパス</summary>
const wchar_t* const Trees::DIFFUSE_TEXTURE_FILEPATH = L"Resources/Textures/TreeTexture01.png";
/// <summary>法線テクスチャファイルパス</summary>
const wchar_t* const Trees::NORMAL_TEXTURE_FILEPATH = L"Resources/Textures/TreeNormal02.png";
/// <summary>オフセットデータファイルパス</summary>
const char* const Trees::OFFSET_DATA_FILEPATH = "Resources/Data/TreesData.csv";
/// <summary>判定軸長さ</summary>
const float Trees::COLLISION_AXIS_HEIGHT = 3.3f;
/// <summary>判定半径</summary>
const float Trees::COLLISION_RADIUS = 0.3f;
/// <summary>最大振幅</summary>
const ::Ishikawa::Math::Float3 Trees::MAX_AMPLITUDE(0.01f, 0.005f, 0.01f);
/// <summary>最小揺れ速度</summary>
const float Trees::MIN_SWING_SPEED = 0.25f * 3.1415926535f;
/// <summary>最大揺れ速度</summary>
const float Trees::MAX_SWING_SPEED = 1.0f * 3.1415926535f;


/// <summary>
/// 行列に変換
/// </summary>
/// <param name="_matrix">保存先の行列</param>
void Trees::WorldOffset::ToMatrix(::Ishikawa::Math::Matrix* const _matrix) const {
	if(_matrix == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_matrix is NULL.");
	}

	// 設定
	(*_matrix) = ::Ishikawa::Math::Matrix::IDENTITY;
	float sinValue = sinf(this->angle);
	float cosValue = cosf(this->angle);
	_matrix->_11 = this->scaleOfWidth * cosValue;
	_matrix->_13 = this->scaleOfWidth * sinValue;
	_matrix->_22 = this->scaleOfHeight;
	_matrix->_31 = this->scaleOfWidth * (-sinValue);
	_matrix->_33 = this->scaleOfWidth * cosValue;
	_matrix->_41 = this->positionX;
	_matrix->_42 = this->positionY;
	_matrix->_43 = this->positionZ;
}


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
Trees::Trees() :
	::CatchAFairy::Object::RenderSameInterface(),
	::Ishikawa::DirectX::Device::DependentResource::Base(),
	useShader(nullptr),
	itObjectVertices(nullptr),
	itObjectVertexCount(0U),
	itObjectIndices(nullptr),
	itObjectIndexCount(0U),
	topology(::CatchAFairy::Shader::User::VertexTopology::TriangleList),
	vertexBuffer(nullptr),
	indexBuffer(nullptr),
	indexCount(0U),
	circumscribedCircleCenterPosition(::Ishikawa::Math::Float3::ZERO),
	squareCircumscribedCircleRadius(0.0f),
	amplitudeArray(nullptr),
	swingSpeedArray(nullptr),
	swingTimeArray(nullptr),
	treesOffset(),
	worldArray(nullptr),
	diffuseTexture(),
	normalTexture()
{
	// 自作オブジェクトデータ読み込み
	this->LoadItObjectData();

	// ワールド変換行列配列の初期化
	this->InitializeWorldArray();
}

/// <summary>
/// デストラクタ
/// </summary>
Trees::~Trees(){
	// ワールド変換行列配列開放
	this->FinalizeWorldArray();
	// 自作オブジェクトデータ開放
	this->ReleaseItObjectData();
}


/// <summary>
/// トポロジータイプの取得
/// </summary>
/// <returns>トポロジータイプ</returns>
::CatchAFairy::Shader::User::VertexTopology Trees::GetTopology() const {
	return this->topology;
}

/// <summary>
/// 頂点バッファの取得
/// </summary>
/// <returns>頂点バッファ</returns>
ID3D11Buffer* Trees::GetVertexBuffer() const {
	return this->vertexBuffer;
}

/// <summary>
/// インデックスバッファの取得
/// </summary>
/// <returns>インデックスバッファ</returns>
ID3D11Buffer* Trees::GetIndexBuffer() const {
	return this->indexBuffer;
}

/// <summary>
/// インデックス数の取得
/// </summary>
/// <returns>インデックス数</returns>
unsigned int Trees::GetIndexCount() const {
	return this->indexCount;
}

/// <summary>
/// 外接円中心座標の取得
/// </summary>
/// <returns>外接円中心座標</returns>
const ::Ishikawa::Math::Float3& Trees::GetCircumscribedCircleCenterPosition() const {
	return this->circumscribedCircleCenterPosition;
}

/// <summary>
/// 外接円半径二乗の取得
/// </summary>
/// <returns>外接円半径二乗</returns>
float Trees::GetSquareCircumscribedCircleRadius() const {
	return this->squareCircumscribedCircleRadius;
}

/// <summary>
/// 拡散反射シェーダリソースビューの取得
/// </summary>
/// <returns>拡散反射シェーダリソースビュー</returns>
ID3D11ShaderResourceView* Trees::GetDiffuseShaderResourceView() const {
	return this->diffuseTexture.GetShaderResourceView();
}

/// <summary>
/// 法線シェーダリソースビューの取得
/// </summary>
/// <returns>法線シェーダリソースビュー</returns>
ID3D11ShaderResourceView* Trees::GetNormalShaderResourceView() const {
	return this->normalTexture.GetShaderResourceView();
}

/// <summary>
/// ワールド変換行列数の取得
/// </summary>
/// <returns>ワールド変換行列数</returns>
unsigned int Trees::GetWorldCount() const {
	return this->treesOffset.GetCount();
}

/// <summary>
/// ワールド変換行列配列の取得
/// </summary>
/// <returns>ワールド変換行列配列</returns>
const ::Ishikawa::Math::Matrix* Trees::GetWorldArray() const {
	return this->worldArray;
}


/// <summary>
/// デバイスに読み込まれた
/// </summary>
void Trees::OnDeviceLoaded(){
	::Ishikawa::DirectX::Device::Device* const itDevice = this->GetDependentITDevice();

	// メンバを登録
	itDevice->AddDependentResource(&(this->diffuseTexture));
	itDevice->AddDependentResource(&(this->normalTexture));

	// テクスチャ読み込み
	this->diffuseTexture.Load(DIFFUSE_TEXTURE_FILEPATH);
	this->normalTexture.Load(NORMAL_TEXTURE_FILEPATH);
}

/// <summary>
/// デバイスが復元された
/// </summary>
void Trees::OnDeviceRestored(){
	// 何もしない
}

/// <summary>
/// デバイスが破棄された
/// </summary>
void Trees::OnDeviceLost(){
	// 何もしない
}


/// <summary>
/// 初期化
/// </summary>
void Trees::Initialize(){
	// 基底の初期化
	this->InitializeITDeviceDependentResource();

	// メンバの初期化
	this->diffuseTexture.Initialize();
	this->normalTexture.Initialize();
}

/// <summary>
/// 更新
/// </summary>
/// <param name="_elapsedTime">更新時間</param>
void Trees::Update(float _elapsedTime){
	this->UpdateWorldArray(_elapsedTime);
}


/// <summary>
/// 頂点バッファとインデックスバッファ作成
/// </summary>
/// <param name="_useShader">利用するシェーダ</param>
void Trees::CreateVertexBufferAndIndexBuffer(
	const ::CatchAFairy::Shader::User::PlayCameraEyeFirst* const _useShader
){
	if(_useShader == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_useShader is NULL.");
	}

	this->useShader = _useShader;

	// 頂点情報
	unsigned int vertexCount = this->itObjectVertexCount;
	::CatchAFairy::Shader::User::PlayCameraEyeFirst::VSInput* vertices =
		new ::CatchAFairy::Shader::User::PlayCameraEyeFirst::VSInput[vertexCount];
	if(vertices == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Couldn't new player's vertex shader input memory.");
	}
	for(unsigned int i=0U; i<vertexCount; ++i){
		vertices[i].position			= this->itObjectVertices[i].position;
		vertices[i].normal				= this->itObjectVertices[i].normal;
		vertices[i].textureCoordinate	= this->itObjectVertices[i].textureCoordinate;
	}
	// 頂点バッファ作成
	this->vertexBuffer = _useShader->CreateVertexBuffer(vertices, vertexCount);
	// 頂点配列削除
	delete[] vertices;
	vertices = nullptr;

	// インデックス情報
	// インデックスバッファ作成
	this->indexBuffer = _useShader->CreateIndexBuffer(this->itObjectIndices, this->itObjectIndexCount);
	this->indexCount = this->itObjectIndexCount;

	// トポロジー設定
	this->topology = ::CatchAFairy::Shader::User::VertexTopology::TriangleList;
}

/// <summary>
/// 頂点バッファとインデックスバッファ開放
/// </summary>
void Trees::ReleaseVertexBufferAndIndexBuffer(){
	if(this->useShader != nullptr){
		// 頂点バッファ開放
		this->useShader->ReleaseBuffer(this->vertexBuffer);
		this->vertexBuffer = nullptr;

		// インデックスバッファ開放
		this->useShader->ReleaseBuffer(this->indexBuffer);
		this->indexBuffer = nullptr;

		this->indexCount = 0U;

		this->useShader = nullptr;
	}
}


/// <summary>
/// ベクトルとの判定
/// </summary>
/// <param name="_startPosition">ベクトル始点</param>
/// <param name="_vector">ベクトル</param>
/// <param name="_matrixVectorToWorld">ベクトル空間からワールド空間への変換行列</param>
/// <param name="_reflectionRate">反射率</param>
/// <returns>反射ベクトル</returns>
::Ishikawa::Math::Float3 Trees::CollideVector(
	const ::Ishikawa::Math::Float3& _startPosition,
	const ::Ishikawa::Math::Float3& _vector,
	const ::Ishikawa::Math::Matrix& _matrixVectorToWorld,
	float _reflectionRate
) const {
	::Ishikawa::Math::Float3 returnVector = _vector;
	for(unsigned int i=0U; i<this->treesOffset.GetCount(); ++i){
		returnVector = ::Ishikawa::Math::Collision::ReflectVectorAtCapsule(
			_startPosition,
			returnVector,
			_matrixVectorToWorld * this->worldArray[i].Invert(),
			::Ishikawa::Math::Float3::ZERO,
			::Ishikawa::Math::Float3(0.0f, COLLISION_AXIS_HEIGHT, 0.0f),
			COLLISION_RADIUS,
			_reflectionRate
		);
	}
	return returnVector;
}


/// <summary>
/// 自作オブジェクトデータ読み込み
/// </summary>
void Trees::LoadItObjectData(){
	::Ishikawa::Model::ItObject::FileLoader itObjectFileLoader;

	itObjectFileLoader.Load(
		&(this->itObjectVertices),
		&(this->itObjectVertexCount),
		&(this->itObjectIndices),
		&(this->itObjectIndexCount),
		&(this->circumscribedCircleCenterPosition),
		&(this->squareCircumscribedCircleRadius),
		IT_MODEL_DATA_FILEPATH
	);
}

/// <summary>
/// 自作オブジェクトデータ開放
/// </summary>
void Trees::ReleaseItObjectData(){
	// 頂点情報
	if(this->itObjectVertices != nullptr){
		delete[] this->itObjectVertices;
		this->itObjectVertices = nullptr;
	}
	this->itObjectVertexCount = 0U;

	// インデックス情報
	if(this->itObjectIndices != nullptr){
		delete[] this->itObjectIndices;
		this->itObjectIndices = nullptr;
	}
	this->itObjectIndexCount = 0U;
}

/// <summary>
/// ワールド変換行列配列の初期化
/// </summary>
void Trees::InitializeWorldArray(){
	const float DELTA_SWING_SPEED = MAX_SWING_SPEED - MIN_SWING_SPEED;

	this->FinalizeWorldArray();

	this->treesOffset.Load(OFFSET_DATA_FILEPATH);
	const ::Ishikawa::Math::Matrix* const startWorldArray = this->treesOffset.GetWorldArray();

	const unsigned int TREE_COUNT = this->treesOffset.GetCount();
	this->amplitudeArray = new ::Ishikawa::Math::Float3[TREE_COUNT];
	this->swingSpeedArray = new ::Ishikawa::Math::Float3[TREE_COUNT];
	this->swingTimeArray = new ::Ishikawa::Math::Float3[TREE_COUNT];
	this->worldArray = new ::Ishikawa::Math::Matrix[TREE_COUNT];

	for(unsigned int i=0U; i<TREE_COUNT; ++i){
		// 振幅
		this->amplitudeArray[i].x = MAX_AMPLITUDE.x * static_cast<float>(::Ishikawa::Common::Random::RealRandom());
		this->amplitudeArray[i].y = MAX_AMPLITUDE.y * static_cast<float>(::Ishikawa::Common::Random::RealRandom());
		this->amplitudeArray[i].z = MAX_AMPLITUDE.z * static_cast<float>(::Ishikawa::Common::Random::RealRandom());
		// 揺れ速度
		this->swingSpeedArray[i].x = MIN_SWING_SPEED + DELTA_SWING_SPEED * static_cast<float>(::Ishikawa::Common::Random::RealRandom());
		this->swingSpeedArray[i].y = MIN_SWING_SPEED + DELTA_SWING_SPEED * static_cast<float>(::Ishikawa::Common::Random::RealRandom());
		this->swingSpeedArray[i].z = MIN_SWING_SPEED + DELTA_SWING_SPEED * static_cast<float>(::Ishikawa::Common::Random::RealRandom());
		// 揺れ時間
		this->swingTimeArray[i] = ::Ishikawa::Math::Float3::ZERO;

		// ワールド行列
		this->worldArray[i] = startWorldArray[i];
	}

}

/// <summary>
/// ワールド変換行列配列の更新
/// </summary>
/// <param name="_elapsedTime">更新時間</param>
void Trees::UpdateWorldArray(float _elapsedTime){
	const float PI_2 = 2.0f * 3.1415926535f;

	const ::Ishikawa::Math::Matrix* const startWorldArray = this->treesOffset.GetWorldArray();
	for(unsigned int i=0U; i<this->treesOffset.GetCount(); ++i){
		// 揺れ時間更新
		this->swingTimeArray[i] += this->swingSpeedArray[i] * _elapsedTime;
		// 各揺れ時間が2PIを超えないようにする
		while(this->swingTimeArray[i].x > PI_2){ this->swingTimeArray[i].x -= PI_2; }
		while(this->swingTimeArray[i].y > PI_2){ this->swingTimeArray[i].y -= PI_2; }
		while(this->swingTimeArray[i].z > PI_2){ this->swingTimeArray[i].z -= PI_2; }

		// 揺れ幅
		::Ishikawa::Math::Float3 amplitude;
		amplitude.x = this->amplitudeArray[i].x * cosf(this->swingTimeArray[i].x);
		amplitude.y = this->amplitudeArray[i].y * cosf(this->swingTimeArray[i].y);
		amplitude.z = this->amplitudeArray[i].z * cosf(this->swingTimeArray[i].z);

		// 揺れ行列
		::Ishikawa::Math::Matrix swingMatrix(
			1.0f, 0.0f, 0.0f, 0.0f,
			amplitude.x, 1.0f+amplitude.y, amplitude.z, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		// ワールド行列
		this->worldArray[i] = swingMatrix * startWorldArray[i];
	}
}

/// <summary>
/// ワールド変換行列配列の終了
/// </summary>
void Trees::FinalizeWorldArray(){
	if(this->worldArray != nullptr){
		delete[] this->worldArray;
		this->worldArray = nullptr;
	}
	if(this->swingTimeArray != nullptr){
		delete[] this->swingTimeArray;
		this->swingTimeArray = nullptr;
	}
	if(this->swingSpeedArray != nullptr){
		delete[] this->swingSpeedArray;
		this->swingSpeedArray = nullptr;
	}
	if(this->amplitudeArray != nullptr){
		delete[] this->amplitudeArray;
		this->amplitudeArray = nullptr;
	}
}
