//=============================================================================
//	Ground.cpp
//
//	地面のソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "Ground.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "Ishikawa/Model/ITObjectLoader.h"
#include "Ishikawa/DirectX/Device/ITDevice.h"
#include "CatchAFairy/Shader/User/ShaderVertexTopology.h"
#include "CatchAFairy/Shader/User/PlayCameraEyeFirstShaderUser.h"

using namespace CatchAFairy::Object::PlayScene;


/// <summary>自作モデルデータファイルパス</summary>
const char* const Ground::IT_MODEL_DATA_FILEPATH = "Resources/Models/TestGround001.itobj";
/// <summary>拡散反射テクスチャファイルパス</summary>
const wchar_t* const Ground::DIFFUSE_TEXTURE_FILEPATH = L"Resources/Textures/GroundTexture.png";
/// <summary>法線テクスチャファイルパス</summary>
const wchar_t* const Ground::NORMAL_TEXTURE_FILEPATH = L"Resources/Textures/GroundNormal01.png";


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
Ground::Ground() :
	::CatchAFairy::Object::RenderInterface(),
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
	world(::Ishikawa::Math::Matrix::IDENTITY),
	diffuseTexture(),
	normalTexture()
{
	// 自作オブジェクトデータ読み込み
	this->LoadItObjectData();
}

/// <summary>
/// デストラクタ
/// </summary>
Ground::~Ground(){
	// 自作オブジェクトデータ開放
	this->ReleaseItObjectData();
}


/// <summary>
/// トポロジータイプの取得
/// </summary>
/// <returns>トポロジータイプ</returns>
::CatchAFairy::Shader::User::VertexTopology Ground::GetTopology() const {
	return this->topology;
}

/// <summary>
/// 頂点バッファの取得
/// </summary>
/// <returns>頂点バッファ</returns>
ID3D11Buffer* Ground::GetVertexBuffer() const {
	return this->vertexBuffer;
}

/// <summary>
/// インデックスバッファの取得
/// </summary>
/// <returns>インデックスバッファ</returns>
ID3D11Buffer* Ground::GetIndexBuffer() const {
	return this->indexBuffer;
}

/// <summary>
/// インデックス数の取得
/// </summary>
/// <returns>インデックス数</returns>
unsigned int Ground::GetIndexCount() const {
	return this->indexCount;
}

/// <summary>
/// 外接円中心座標の取得
/// </summary>
/// <returns>外接円中心座標</returns>
const ::Ishikawa::Math::Float3& Ground::GetCircumscribedCircleCenterPosition() const {
	return this->circumscribedCircleCenterPosition;
}

/// <summary>
/// 外接円半径二乗の取得
/// </summary>
/// <returns>外接円半径二乗</returns>
float Ground::GetSquareCircumscribedCircleRadius() const {
	return this->squareCircumscribedCircleRadius;
}

/// <summary>
/// 拡散反射シェーダリソースビューの取得
/// </summary>
/// <returns>拡散反射シェーダリソースビュー</returns>
ID3D11ShaderResourceView* Ground::GetDiffuseShaderResourceView() const {
	return this->diffuseTexture.GetShaderResourceView();
}

/// <summary>
/// 法線シェーダリソースビューの取得
/// </summary>
/// <returns>法線シェーダリソースビュー</returns>
ID3D11ShaderResourceView* Ground::GetNormalShaderResourceView() const {
	return this->normalTexture.GetShaderResourceView();
}

/// <summary>
/// ワールド変換行列の取得
/// </summary>
/// <returns>ワールド変換行列</returns>
const ::Ishikawa::Math::Matrix& Ground::GetWorld() const {
	return this->world;
}


/// <summary>
/// デバイスに読み込まれた
/// </summary>
void Ground::OnDeviceLoaded(){
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
void Ground::OnDeviceRestored(){
	// 何もしない
}

/// <summary>
/// デバイスが破棄された
/// </summary>
void Ground::OnDeviceLost(){
	// 何もしない
}


/// <summary>
/// 初期化
/// </summary>
void Ground::Initialize(){
	// 基底の初期化
	this->InitializeITDeviceDependentResource();

	// メンバの初期化
	this->diffuseTexture.Initialize();
	this->normalTexture.Initialize();
}


/// <summary>
/// 頂点バッファとインデックスバッファ作成
/// </summary>
/// <param name="_useShader">利用するシェーダ</param>
void Ground::CreateVertexBufferAndIndexBuffer(
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
void Ground::ReleaseVertexBufferAndIndexBuffer(){
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
void Ground::LoadItObjectData(){
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
void Ground::ReleaseItObjectData(){
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
