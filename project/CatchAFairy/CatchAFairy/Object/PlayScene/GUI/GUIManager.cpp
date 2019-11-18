//=============================================================================
//	GUIManager.cpp
//
//	GUIマネージャのソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "GUIManager.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "Ishikawa/Common/SafePointer.h"
#include "Ishikawa/Math/ITFloatN.h"
#include "Ishikawa/DirectX/Device/ITDevice.h"
#include "Ishikawa/DirectX/Texture/ITTextureData.h"
#include "Ishikawa/DirectX/ITViewport.h"
#include "CatchAFairy/Scene/SceneManager.h"

using namespace ::CatchAFairy::Object::PlayScene::GUI;

/// <summary>出力サイズ</summary>
const ::Ishikawa::Math::Float2 GUIManager::OUTPUT_SIZE(800.0f, 600.0f);
/// <summary>GUIインデックス数</summary>
const unsigned int GUIManager::GUI_INDEX_COUNT = GUI_VERTEX_COUNT;
/// <summary>形状テクスチャファイルパス</summary>
const wchar_t* const GUIManager::SHAPE_TEXTURE_FILEPATH = L"Resources\\Textures\\Shapes.png";
/// <summary>表示テクスチャファイルパス</summary>
const wchar_t* const GUIManager::VIEW_TEXTURE_FILEPATH = L"Resources\\Textures\\GuiTexture.png";


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
GUIManager::GUIManager() :
	outputTargetTexture(nullptr),
	guiShaderUser(),
	guiVertexBuffer(nullptr),
	guiIndexBuffer(nullptr),
	guiIndexCount(0U),
	guiVertexArray{},
	shapeTexture(),
	viewTexture()
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
GUIManager::~GUIManager(){
	// 何もしない
}


/// <summary>
/// GUI頂点の取得
/// </summary>
/// <param name="_startIndex">取得するGUI頂点のインデックス</param>
/// <param name="_getCount">取得数</param>
/// <returns>GUI頂点のポインタ</returns>
::CatchAFairy::Shader::User::PlayGUI::VSInput* GUIManager::GetGUIVertices(
	unsigned int _startIndex,
	unsigned int _getCount
){
	if(_getCount == 0U){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_getCount is zero. This process is not meaning.");
	}
	if((_startIndex + _getCount) > GUI_VERTEX_COUNT){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("Input value is over of vertices max count.");
	}

	return (&(this->guiVertexArray[_startIndex]));
}


/// <summary>
/// 初期化
/// </summary>
void GUIManager::Initialize(){
	this->guiShaderUser.Initialize();
	this->shapeTexture.Initialize();
	this->viewTexture.Initialize();
}

/// <summary>
/// 描画
/// </summary>
void GUIManager::Render(){
	this->DrawBefore();
	this->Draw();
	this->DrawAfter();
}


/// <summary>
/// ゲームのデバイスに依存するリソースの追加
/// </summary>
/// <param name="_itDevice">自作デバイス</param>
void GUIManager::AddITDeviceDependentResources(::Ishikawa::DirectX::Device::Device* const _itDevice){
	_itDevice->AddDependentResource(&(this->guiShaderUser));
	_itDevice->AddDependentResource(&(this->shapeTexture));
	_itDevice->AddDependentResource(&(this->viewTexture));
}

/// <summary>
/// デバイス依存リソース作成
/// </summary>
void GUIManager::CreateDeviceDependentResources(){
	// 画像読み込み
	this->shapeTexture.Load(SHAPE_TEXTURE_FILEPATH);
	this->viewTexture.Load(VIEW_TEXTURE_FILEPATH);

	// バッファ作成
	this->CreateGUIIndexBuffer();
}

/// <summary>
/// デバイス依存リソース開放
/// </summary>
void GUIManager::ReleaseDeviceDependentResources(){
	// バッファ開放
	this->ReleaseGUIVertexBuffer();
	this->ReleaseGUIIndexBuffer();
}


/// <summary>
/// 描画前
/// </summary>
void GUIManager::DrawBefore(){
	if(this->outputTargetTexture == nullptr){
		// 異常終了
		::Ishikawa::Common::Exception::FunctionFailed("outputTargetTexture is NULL.");
	}

	// 出力リソース
	::CatchAFairy::Shader::User::PlayGUI::OutputResource outputResource;
//	outputResource.renderTargetView = this->guiRenderTargetTexture.GetRenderTargetView();
	outputResource.renderTargetView = this->outputTargetTexture->GetRenderTargetView();

	unsigned int width = 0U, height = 0U;
//	this->guiRenderTargetTexture.GetTextureSize(&width, &height);
	this->outputTargetTexture->GetTextureSize(&width, &height);
	::Ishikawa::DirectX::Viewport viewport;
	viewport.leftTopX	= 0.0f;
	viewport.leftTopY	= 0.0f;
	viewport.width		= static_cast<float>(width);
	viewport.height		= static_cast<float>(height);
	viewport.minDepth	= 0.0f;
	viewport.maxDepth	= 1.0f;
	outputResource.viewport = &viewport;

	// 情報リセット
	const ::Ishikawa::Math::Float4 CLEAR_COLOR(0.0f, 0.0f, 0.0f, 0.0f);
	this->guiShaderUser.DrawBefore(outputResource, nullptr);
}

/// <summary>
/// 描画
/// </summary>
void GUIManager::Draw(){
	// 頂点バッファ作成
	this->CreateGUIVertexBuffer();

	// 入力リソース
	::CatchAFairy::Shader::User::PlayGUI::RenderUseResource inputResource;
	inputResource.constantBuffer.textureViewSize = ::Ishikawa::Math::Float4(OUTPUT_SIZE.x, OUTPUT_SIZE.y, 0.0f, 1.0f);
	inputResource.vertexBuffer = this->guiVertexBuffer;
	inputResource.indexBuffer = this->guiIndexBuffer;
	inputResource.indexCount = this->guiIndexCount;
	inputResource.shapeShaderResourceView = this->shapeTexture.GetShaderResourceView();
	inputResource.viewShaderResourceView = this->viewTexture.GetShaderResourceView();

	// 描画
	this->guiShaderUser.Draw(inputResource);
}

/// <summary>
/// 描画後
/// </summary>
void GUIManager::DrawAfter(){
	this->guiShaderUser.DrawAfter();
}


/// <summary>
/// GUI頂点バッファの作成
/// </summary>
void GUIManager::CreateGUIVertexBuffer(){
	this->ReleaseGUIVertexBuffer();

	this->guiVertexBuffer = this->guiShaderUser.CreateVertexBuffer(this->guiVertexArray, GUI_INDEX_COUNT);
}

/// <summary>
/// GUI頂点バッファの開放
/// </summary>
void GUIManager::ReleaseGUIVertexBuffer(){
	if(this->guiVertexBuffer != nullptr){
		this->guiShaderUser.ReleaseBuffer(this->guiVertexBuffer);
		this->guiVertexBuffer = nullptr;
	}
}

/// <summary>
/// GUIインデックスバッファの作成
/// </summary>
void GUIManager::CreateGUIIndexBuffer(){
	unsigned long indices[GUI_INDEX_COUNT]{0UL};

	for(unsigned int i=0U; i<GUI_INDEX_COUNT; ++i){
		indices[i] = i;
	}

	this->ReleaseGUIIndexBuffer();
	this->guiIndexBuffer = this->guiShaderUser.CreateIndexBuffer(indices, GUI_INDEX_COUNT);
	this->guiIndexCount = GUI_INDEX_COUNT;
}

/// <summary>
/// GUIインデックスバッファの開放
/// </summary>
void GUIManager::ReleaseGUIIndexBuffer(){
	if(this->guiIndexBuffer != nullptr){
		this->guiShaderUser.ReleaseBuffer(this->guiIndexBuffer);
		this->guiIndexBuffer = nullptr;
		this->guiIndexCount = 0U;
	}
}
