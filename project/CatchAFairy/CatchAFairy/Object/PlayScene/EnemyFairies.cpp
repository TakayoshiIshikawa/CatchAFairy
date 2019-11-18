//=============================================================================
//	EnemyFairies.cpp
//
//	敵妖精達のソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "EnemyFairies.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "Ishikawa/Common/Random.h"
#include "Ishikawa/Model/ITObjectLoader.h"
#include "Ishikawa/DirectX/Device/ITDevice.h"
#include "CatchAFairy/Shader/User/ShaderVertexTopology.h"
#include "CatchAFairy/Shader/User/PlayCameraEyeFirstShaderUser.h"
#include "PlayCamera.h"
#include "PointLight.h"
#include "Player.h"

using namespace ::CatchAFairy::Object::PlayScene;


/// <summary>自作モデルデータファイルパス</summary>
const char* const EnemyFairies::IT_MODEL_DATA_FILEPATH = "Resources/Models/TestFairy001.itobj";
/// <summary>拡散反射テクスチャファイルパス</summary>
const wchar_t* const EnemyFairies::DIFFUSE_TEXTURE_FILEPATH = L"Resources/Textures/White.png";
/// <summary>法線テクスチャファイルパス</summary>
const wchar_t* const EnemyFairies::NORMAL_TEXTURE_FILEPATH = L"Resources/Textures/DefaultNormal.png";

/// <summary>敵妖精生成用配列</summary>
const unsigned int EnemyFairies::ENEMY_FAIRY_GENERATING_COUNT[static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)] = {
	1U,
	5U,
	16U
};


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
EnemyFairies::EnemyFairies() :
	::Ishikawa::DirectX::Device::DependentResource::Base(),
	::CatchAFairy::Object::UpdateInterface(),
	::CatchAFairy::Object::RenderSameInterface(),
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
	diffuseTexture(),
	normalTexture(),
	worldArray{},
	controllerArray{},
	player(nullptr),
	catchCount(0U)
{
	// 自作オブジェクトデータ読み込み
	this->LoadItObjectData();
}

/// <summary>
/// デストラクタ
/// </summary>
EnemyFairies::~EnemyFairies(){
	// 自作オブジェクトデータ開放
	this->ReleaseItObjectData();
}


/// <summary>
/// トポロジータイプの取得
/// </summary>
/// <returns>トポロジータイプ</returns>
::CatchAFairy::Shader::User::VertexTopology EnemyFairies::GetTopology() const {
	return this->topology;
}

/// <summary>
/// 頂点バッファの取得
/// </summary>
/// <returns>頂点バッファ</returns>
ID3D11Buffer* EnemyFairies::GetVertexBuffer() const {
	return this->vertexBuffer;
}

/// <summary>
/// インデックスバッファの取得
/// </summary>
/// <returns>インデックスバッファ</returns>
ID3D11Buffer* EnemyFairies::GetIndexBuffer() const {
	return this->indexBuffer;
}

/// <summary>
/// インデックス数の取得
/// </summary>
/// <returns>インデックス数</returns>
unsigned int EnemyFairies::GetIndexCount() const {
	return this->indexCount;
}

/// <summary>
/// 外接円中心座標の取得
/// </summary>
/// <returns>外接円中心座標</returns>
const ::Ishikawa::Math::Float3& EnemyFairies::GetCircumscribedCircleCenterPosition() const {
	return this->circumscribedCircleCenterPosition;
}

/// <summary>
/// 外接円半径二乗の取得
/// </summary>
/// <returns>外接円半径二乗</returns>
float EnemyFairies::GetSquareCircumscribedCircleRadius() const {
	return this->squareCircumscribedCircleRadius;
}

/// <summary>
/// 拡散反射シェーダリソースビューの取得
/// </summary>
/// <returns>拡散反射シェーダリソースビュー</returns>
ID3D11ShaderResourceView* EnemyFairies::GetDiffuseShaderResourceView() const {
	return this->diffuseTexture.GetShaderResourceView();
}

/// <summary>
/// 法線シェーダリソースビューの取得
/// </summary>
/// <returns>法線シェーダリソースビュー</returns>
ID3D11ShaderResourceView* EnemyFairies::GetNormalShaderResourceView() const {
	return this->normalTexture.GetShaderResourceView();
}

/// <summary>
/// ワールド変換行列数の取得
/// </summary>
/// <returns>ワールド変換行列数</returns>
unsigned int EnemyFairies::GetWorldCount() const {
	return ENEMY_FAIRY_COUNT;
}

/// <summary>
/// ワールド変換行列配列の取得
/// </summary>
/// <returns>ワールド変換行列配列</returns>
const ::Ishikawa::Math::Matrix* EnemyFairies::GetWorldArray() const {
	return this->worldArray;
}

/// <summary>
/// プレイヤの取得
/// </summary>
/// <returns>プレイヤ</returns>
const Player* EnemyFairies::GetPlayer() const {
	return this->player;
}

/// <summary>
/// 指定妖精の種類の取得
/// </summary>
/// <param name="_id">敵妖精ID</param>
/// <returns>指定妖精の種類</returns>
const CatchAFairy::Data::EnemyFairyType EnemyFairies::GetFairyType(unsigned int _id) const {
	return this->controllerArray[_id].GetType();
}


/// <summary>
/// ポイントライトの設定
/// </summary>
/// <param name="_pointLight">ポイントライト</param>
void EnemyFairies::SetPointLight(PointLight* const _pointLight){
	for(unsigned int i=0U; i<ENEMY_FAIRY_COUNT; ++i){
		this->controllerArray[i].SetPointLight(_pointLight);
	}
}

/// <summary>
/// プレイヤの設定
/// </summary>
/// <param name="_player">プレイヤ</param>
void EnemyFairies::SetPlayer(const Player* const _player){
	this->player = _player;
}


/// <summary>
/// デバイスに読み込まれた
/// </summary>
void EnemyFairies::OnDeviceLoaded(){
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
void EnemyFairies::OnDeviceRestored(){
	// 何もしない
}

/// <summary>
/// デバイスが破棄された
/// </summary>
void EnemyFairies::OnDeviceLost(){
	// 何もしない
}

/// <summary>
/// どれかが捕まった
/// </summary>
/// <param name="_id">捕まった妖精のID</param>
void EnemyFairies::OnCatched(unsigned int _id){
	if(_id >= ENEMY_FAIRY_COUNT){
		return;
	}

	++(this->catchCount);

	unsigned int generateIndex = static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Normal);
	for(unsigned int i=static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)-1; i>0; --i){
		if((this->catchCount % ENEMY_FAIRY_GENERATING_COUNT[i]) == 0U){
			generateIndex = i;
			break;
		}
	}
	this->controllerArray[_id].SetType(static_cast<CatchAFairy::Data::EnemyFairyType>(generateIndex));
	this->controllerArray[_id].Reset();
}


/// <summary>
/// 初期化
/// </summary>
void EnemyFairies::Initialize(){
	// 基底の初期化
	this->InitializeITDeviceDependentResource();

	// メンバの初期化
	this->diffuseTexture.Initialize();
	this->normalTexture.Initialize();

	// すべての妖精を初期化
	for(unsigned int i=0U; i<ENEMY_FAIRY_COUNT; ++i){
		this->controllerArray[i].SetType(CatchAFairy::Data::EnemyFairyType::Normal);
		this->controllerArray[i].SetId(::CatchAFairy::Data::PointLightsSetting::PLAYER_FAIRY_COUNT + i);
		this->worldArray[i] = ::Ishikawa::Math::Matrix::IDENTITY;
		this->controllerArray[i].SetWorld(&(this->worldArray[i]));
		this->controllerArray[i].SetEnemyFairies(this);
		this->controllerArray[i].Reset();
	}
}


/// <summary>
/// 更新
/// </summary>
/// <param name="_elapsedTime">更新間隔</param>
void EnemyFairies::Update(float _elapsedTime){
	for(unsigned int i=0U; i<ENEMY_FAIRY_COUNT; ++i){
		this->controllerArray[i].Update(_elapsedTime);
	}
}


/// <summary>
/// 頂点バッファとインデックスバッファ作成
/// </summary>
/// <param name="_useShader">利用するシェーダ</param>
void EnemyFairies::CreateVertexBufferAndIndexBuffer(
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
void EnemyFairies::ReleaseVertexBufferAndIndexBuffer(){
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
/// 自作オブジェクトデータ読み込み
/// </summary>
void EnemyFairies::LoadItObjectData(){
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
void EnemyFairies::ReleaseItObjectData(){
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
