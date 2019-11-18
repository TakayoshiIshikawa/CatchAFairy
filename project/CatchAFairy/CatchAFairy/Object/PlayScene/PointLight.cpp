//=============================================================================
//	PointLight.cpp
//
//	ポイントライトのソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "PointLight.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"

using namespace CatchAFairy::Object::PlayScene;


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
PointLight::PointLight() :
	::CatchAFairy::Object::UpdateInterface(),
	::CatchAFairy::Object::PointLightInterface(),
	vertexArray{},
	vertexBuffer(nullptr),
	indexBuffer(nullptr),
	indexCount(0U),
	reflectionLightShaderUser(nullptr),
	manaLightColor{},
	manaLightRadiusAndLumen{},
	manaLightCreateIndex{0U},
	manaLightArray{}
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
PointLight::~PointLight(){
	// 何もしない
}


/// <summary>
/// 妖精本体につくライトの座標の設定
/// </summary>
/// <param name="_id">設定する妖精のID</param>
/// <param name="_x">X座標</param>
/// <param name="_y">Y座標</param>
/// <param name="_z">Z座標</param>
void PointLight::SetFairyBodyLightPosition(unsigned int _id, float _x, float _y, float _z){
	if(_id >= FAIRY_COUNT) return;

	const unsigned int OFFSET_INDEX = _id * ONE_FAIRY_LIGHT_ALL_COUNT;

	for(unsigned int i=0; i<ONE_FAIRY_BODY_LIGHT_COUNT; ++i){
		::Ishikawa::Math::Float3& position = this->vertexArray[OFFSET_INDEX+i].position;
		position.x = _x;
		position.y = _y;
		position.z = _z;
	}
}

/// <summary>
/// 妖精本体につくライトの色の設定
/// </summary>
/// <param name="_id">設定する妖精のID</param>
/// <param name="_red">赤成分</param>
/// <param name="_green">緑成分</param>
/// <param name="_blue">青成分</param>
/// <param name="_alpha">不透明成分</param>
void PointLight::SetFairyBodyLightColor(unsigned int _id, float _red, float _green, float _blue, float _alpha){
	if(_id >= FAIRY_COUNT) return;

	const unsigned int OFFSET_INDEX = _id * ONE_FAIRY_LIGHT_ALL_COUNT;

	for(unsigned int i=0; i<ONE_FAIRY_BODY_LIGHT_COUNT; ++i){
		::Ishikawa::Math::Float4& color = this->vertexArray[OFFSET_INDEX+i].color;
		color.x = _red;
		color.y = _green;
		color.z = _blue;
		color.w = _alpha;
	}
}

/// <summary>
/// 妖精本体につくライトの半径の設定
/// </summary>
/// <param name="_id">設定する妖精のID</param>
/// <param name="_radius">半径</param>
void PointLight::SetFairyBodyLightRadius(unsigned int _id, float _radius){
	if(_id >= FAIRY_COUNT) return;

	const unsigned int OFFSET_INDEX = _id * ONE_FAIRY_LIGHT_ALL_COUNT;

	for(unsigned int i=0; i<ONE_FAIRY_BODY_LIGHT_COUNT; ++i){
		this->vertexArray[OFFSET_INDEX+i].radiusAndLumen.x = _radius;
	}
}

/// <summary>
/// 妖精本体につくライトの光度の設定
/// </summary>
/// <param name="_id">設定する妖精のID</param>
/// <param name="_lumen">光度</param>
void PointLight::SetFairyBodyLightLumen(unsigned int _id, float _lumen){
	if(_id >= FAIRY_COUNT) return;

	const unsigned int OFFSET_INDEX = _id * ONE_FAIRY_LIGHT_ALL_COUNT;

	for(unsigned int i=0; i<ONE_FAIRY_BODY_LIGHT_COUNT; ++i){
		this->vertexArray[OFFSET_INDEX+i].radiusAndLumen.y = _lumen;
	}
}

/// <summary>
/// マナライト色の設定
/// </summary>
/// <param name="_id">設定する妖精のID</param>
/// <param name="_red">赤成分</param>
/// <param name="_green">緑成分</param>
/// <param name="_blue">青成分</param>
/// <param name="_alpha">不透明成分</param>
void PointLight::SetFairyManaLightColor(unsigned int _id, float _red, float _green, float _blue, float _alpha){
	if(_id >= FAIRY_COUNT) return;
	
	this->manaLightColor[_id].x = _red;
	this->manaLightColor[_id].y = _green;
	this->manaLightColor[_id].z = _blue;
	this->manaLightColor[_id].w = _alpha;
}

/// <summary>
/// マナライト半径の設定
/// </summary>
/// <param name="_id">設定する妖精のID</param>
/// <param name="_radius">半径</param>
void PointLight::SetFairyManaLightRadius(unsigned int _id, float _radius){
	if(_id >= FAIRY_COUNT) return;

	this->manaLightRadiusAndLumen[_id].x = _radius;
}

/// <summary>
/// マナライト光度の設定
/// </summary>
/// <param name="_id">設定する妖精のID</param>
/// <param name="_lumen">光度</param>
void PointLight::SetFairyManaLightLumen(unsigned int _id, float _lumen){
	if(_id >= FAIRY_COUNT) return;

	this->manaLightRadiusAndLumen[_id].y = _lumen;
}


/// <summary>
/// 初期化
/// </summary>
void PointLight::Initialize(){
	this->InitializeVertexArray();

	// 各マナライトに頂点を設定
	for(unsigned int i=0U; i<FAIRY_COUNT; ++i){
		const unsigned int OFFSET_INDEX = i*ONE_FAIRY_LIGHT_ALL_COUNT + ONE_FAIRY_BODY_LIGHT_COUNT;

		for(unsigned int j=0U; j<ONE_FAIRY_MANA_LIGHT_COUNT; ++j){
			this->manaLightArray[i][j].SetLightVertex(&(this->vertexArray[OFFSET_INDEX+j]));
		}
	}
}

/// <summary>
/// 更新
/// </summary>
/// <param name="_elapsedTime">更新間隔</param>
void PointLight::Update(float _elapsedTime){
	// マナライトを更新
	for(unsigned int i=0U; i<FAIRY_COUNT; ++i){
		for(unsigned int j=0U; j<ONE_FAIRY_MANA_LIGHT_COUNT; ++j){
			this->manaLightArray[i][j].Update(_elapsedTime);
		}
	}

	// 頂点バッファ更新
	this->ReleaseVertexBuffer();
	this->CreateVertexBuffer();
}


/// <summary>
/// マナライト生成
/// </summary>
/// <param name="_id">設定する妖精のID</param>
/// <param name="_x">X座標</param>
/// <param name="_y">Y座標</param>
/// <param name="_z">Z座標</param>
void PointLight::GenerateFairyManaLight(unsigned int _id, float _x, float _y, float _z){
	if(_id >= FAIRY_COUNT) return;

	this->manaLightArray[_id][this->manaLightCreateIndex[_id]].Generate(
		::Ishikawa::Math::Float3(_x, _y, _z),
		this->manaLightColor[_id],
		this->manaLightRadiusAndLumen[_id]
	);

	this->manaLightCreateIndex[_id] = (this->manaLightCreateIndex[_id]+1) % ONE_FAIRY_MANA_LIGHT_COUNT;
}


/// <summary>
/// デバイス依存リソース作成
/// </summary>
void PointLight::CreateDeviceDependentResources(){
	this->CreateVertexBuffer();
	this->CreateIndexBuffer();
}

/// <summary>
/// デバイス依存リソース開放
/// </summary>
void PointLight::ReleaseDeviceDependentResources(){
	this->ReleaseIndexBuffer();
	this->ReleaseVertexBuffer();
}


/// <summary>
/// 頂点配列の初期化
/// </summary>
void PointLight::InitializeVertexArray(){
	for(unsigned int i=0U; i<MAX_COUNT; ++i){
		this->vertexArray[i].position = ::Ishikawa::Math::Float3::ZERO;
		this->vertexArray[i].color = ::Ishikawa::Math::Float4::ZERO;
		this->vertexArray[i].radiusAndLumen = ::Ishikawa::Math::Float2(1.0f, 1.0f);
	}
}


/// <summary>
/// 頂点バッファ作成
/// </summary>
void PointLight::CreateVertexBuffer(){
	if(this->reflectionLightShaderUser == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Reflection light shader user is NULL.");
	}

	this->vertexBuffer = this->reflectionLightShaderUser->CreateVertexBuffer(this->vertexArray, MAX_COUNT);
}

/// <summary>
/// インデックスバッファ作成
/// </summary>
void PointLight::CreateIndexBuffer(){
	if(this->reflectionLightShaderUser == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Reflection light shader user is NULL.");
	}

	unsigned long indexArray[MAX_COUNT]{0UL};
	for(unsigned long i=0UL; i<MAX_COUNT; ++i){
		indexArray[i] = i;
	}

	this->indexBuffer = this->reflectionLightShaderUser->CreateIndexBuffer(indexArray, MAX_COUNT);
	this->indexCount = static_cast<unsigned int>(MAX_COUNT);
}

/// <summary>
/// 頂点バッファ開放
/// </summary>
void PointLight::ReleaseVertexBuffer(){
	if(this->reflectionLightShaderUser == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Reflection light shader user is NULL.");
	}

	if(this->vertexBuffer != nullptr){
		this->reflectionLightShaderUser->ReleaseBuffer(this->vertexBuffer);
		this->vertexBuffer = nullptr;
	}
}

/// <summary>
/// インデックスバッファ開放
/// </summary>
void PointLight::ReleaseIndexBuffer(){
	if(this->reflectionLightShaderUser == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Reflection light shader user is NULL.");
	}

	if(this->indexBuffer != nullptr){
		this->reflectionLightShaderUser->ReleaseBuffer(this->indexBuffer);
		this->indexBuffer = nullptr;
		this->indexCount = 0U;
	}
}
