//=============================================================================
//	GUIManager.cpp
//
//	GUI�}�l�[�W���̃\�[�X�t�@�C��
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

/// <summary>�o�̓T�C�Y</summary>
const ::Ishikawa::Math::Float2 GUIManager::OUTPUT_SIZE(800.0f, 600.0f);
/// <summary>GUI�C���f�b�N�X��</summary>
const unsigned int GUIManager::GUI_INDEX_COUNT = GUI_VERTEX_COUNT;
/// <summary>�`��e�N�X�`���t�@�C���p�X</summary>
const wchar_t* const GUIManager::SHAPE_TEXTURE_FILEPATH = L"Resources\\Textures\\Shapes.png";
/// <summary>�\���e�N�X�`���t�@�C���p�X</summary>
const wchar_t* const GUIManager::VIEW_TEXTURE_FILEPATH = L"Resources\\Textures\\GuiTexture.png";


/// <summary>
/// �f�t�H���g�R���X�g���N�^
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
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
GUIManager::~GUIManager(){
	// �������Ȃ�
}


/// <summary>
/// GUI���_�̎擾
/// </summary>
/// <param name="_startIndex">�擾����GUI���_�̃C���f�b�N�X</param>
/// <param name="_getCount">�擾��</param>
/// <returns>GUI���_�̃|�C���^</returns>
::CatchAFairy::Shader::User::PlayGUI::VSInput* GUIManager::GetGUIVertices(
	unsigned int _startIndex,
	unsigned int _getCount
){
	if(_getCount == 0U){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("_getCount is zero. This process is not meaning.");
	}
	if((_startIndex + _getCount) > GUI_VERTEX_COUNT){
		// �ُ�I��
		throw ::Ishikawa::Common::Exception::FunctionFailed("Input value is over of vertices max count.");
	}

	return (&(this->guiVertexArray[_startIndex]));
}


/// <summary>
/// ������
/// </summary>
void GUIManager::Initialize(){
	this->guiShaderUser.Initialize();
	this->shapeTexture.Initialize();
	this->viewTexture.Initialize();
}

/// <summary>
/// �`��
/// </summary>
void GUIManager::Render(){
	this->DrawBefore();
	this->Draw();
	this->DrawAfter();
}


/// <summary>
/// �Q�[���̃f�o�C�X�Ɉˑ����郊�\�[�X�̒ǉ�
/// </summary>
/// <param name="_itDevice">����f�o�C�X</param>
void GUIManager::AddITDeviceDependentResources(::Ishikawa::DirectX::Device::Device* const _itDevice){
	_itDevice->AddDependentResource(&(this->guiShaderUser));
	_itDevice->AddDependentResource(&(this->shapeTexture));
	_itDevice->AddDependentResource(&(this->viewTexture));
}

/// <summary>
/// �f�o�C�X�ˑ����\�[�X�쐬
/// </summary>
void GUIManager::CreateDeviceDependentResources(){
	// �摜�ǂݍ���
	this->shapeTexture.Load(SHAPE_TEXTURE_FILEPATH);
	this->viewTexture.Load(VIEW_TEXTURE_FILEPATH);

	// �o�b�t�@�쐬
	this->CreateGUIIndexBuffer();
}

/// <summary>
/// �f�o�C�X�ˑ����\�[�X�J��
/// </summary>
void GUIManager::ReleaseDeviceDependentResources(){
	// �o�b�t�@�J��
	this->ReleaseGUIVertexBuffer();
	this->ReleaseGUIIndexBuffer();
}


/// <summary>
/// �`��O
/// </summary>
void GUIManager::DrawBefore(){
	if(this->outputTargetTexture == nullptr){
		// �ُ�I��
		::Ishikawa::Common::Exception::FunctionFailed("outputTargetTexture is NULL.");
	}

	// �o�̓��\�[�X
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

	// ��񃊃Z�b�g
	const ::Ishikawa::Math::Float4 CLEAR_COLOR(0.0f, 0.0f, 0.0f, 0.0f);
	this->guiShaderUser.DrawBefore(outputResource, nullptr);
}

/// <summary>
/// �`��
/// </summary>
void GUIManager::Draw(){
	// ���_�o�b�t�@�쐬
	this->CreateGUIVertexBuffer();

	// ���̓��\�[�X
	::CatchAFairy::Shader::User::PlayGUI::RenderUseResource inputResource;
	inputResource.constantBuffer.textureViewSize = ::Ishikawa::Math::Float4(OUTPUT_SIZE.x, OUTPUT_SIZE.y, 0.0f, 1.0f);
	inputResource.vertexBuffer = this->guiVertexBuffer;
	inputResource.indexBuffer = this->guiIndexBuffer;
	inputResource.indexCount = this->guiIndexCount;
	inputResource.shapeShaderResourceView = this->shapeTexture.GetShaderResourceView();
	inputResource.viewShaderResourceView = this->viewTexture.GetShaderResourceView();

	// �`��
	this->guiShaderUser.Draw(inputResource);
}

/// <summary>
/// �`���
/// </summary>
void GUIManager::DrawAfter(){
	this->guiShaderUser.DrawAfter();
}


/// <summary>
/// GUI���_�o�b�t�@�̍쐬
/// </summary>
void GUIManager::CreateGUIVertexBuffer(){
	this->ReleaseGUIVertexBuffer();

	this->guiVertexBuffer = this->guiShaderUser.CreateVertexBuffer(this->guiVertexArray, GUI_INDEX_COUNT);
}

/// <summary>
/// GUI���_�o�b�t�@�̊J��
/// </summary>
void GUIManager::ReleaseGUIVertexBuffer(){
	if(this->guiVertexBuffer != nullptr){
		this->guiShaderUser.ReleaseBuffer(this->guiVertexBuffer);
		this->guiVertexBuffer = nullptr;
	}
}

/// <summary>
/// GUI�C���f�b�N�X�o�b�t�@�̍쐬
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
/// GUI�C���f�b�N�X�o�b�t�@�̊J��
/// </summary>
void GUIManager::ReleaseGUIIndexBuffer(){
	if(this->guiIndexBuffer != nullptr){
		this->guiShaderUser.ReleaseBuffer(this->guiIndexBuffer);
		this->guiIndexBuffer = nullptr;
		this->guiIndexCount = 0U;
	}
}
