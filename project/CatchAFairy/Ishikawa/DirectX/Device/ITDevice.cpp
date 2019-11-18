//=============================================================================
//	ITDevice.cpp
//
//	����f�o�C�X�̃\�[�X�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITDevice.h"
#include <stdio.h>
#pragma warning(disable:4626)
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#include <wrl/client.h>
#pragma warning(default:4820)
#pragma warning(default:4668)
#pragma warning(default:4626)
#pragma warning(disable:4061)
#pragma warning(disable:4365)
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#pragma warning(disable:4917)
#include <d3d11_1.h>
#pragma warning(default:4917)
#pragma warning(default:4820)
#pragma warning(default:4668)
#pragma warning(default:4365)
#pragma warning(default:4061)
#include "Ishikawa/Common/Exception/ITExceptions.h"
#pragma warning(disable:4996)

using namespace Ishikawa::DirectX::Device;


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
Device::Device() :
	device(nullptr),
	featureLevel(D3D_FEATURE_LEVEL_9_1),
	context(nullptr),
	deviceDependentResourceListEdge()
{
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Device::~Device(){
	this->ReleaseContext();
	this->ReleaseDevice();
}


/// <summary>
/// ������
/// </summary>
void Device::Initialize(){
	// �����o�̏�����
	this->deviceDependentResourceListEdge.Initialize();
}

/// <summary>
/// ������
/// </summary>
void Device::Create(){
	// �A�_�v�^�擾
	Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter1;
	this->GetAdapter(adapter1.GetAddressOf());
	if(adapter1 == nullptr){
		// �ُ�I��
		throw Ishikawa::Common::Exception::FunctionFailed("Couldn't get adapter.");
	}

	// �f�o�C�X����
	this->CreateDevice(adapter1.Get());
}

/// <summary>
/// ����
/// </summary>
void Device::Restore(){
	// ����������
	this->Create();

	// �����ʒm
	DependentResource::Interface* resource = this->deviceDependentResourceListEdge.GetNextITDeviceDependentResource();
	while(!(resource->IsITDeviceDependentResourceListEdge())){
		resource->OnDeviceRestored();

		resource = resource->GetNextITDeviceDependentResource();
	}
}

/// <summary>
/// ���X�g
/// </summary>
void Device::Lost(){
	// �J��
	this->ReleaseContext();
	this->ReleaseDevice();

	// ���X�g�ʒm
	DependentResource::Interface* resource = this->deviceDependentResourceListEdge.GetNextITDeviceDependentResource();
	while(!(resource->IsITDeviceDependentResourceListEdge())){
		resource->OnDeviceLost();

		resource = resource->GetNextITDeviceDependentResource();
	}
}


/// <summary>
/// �ˑ����\�[�X�̒ǉ�[1����]
/// </summary>
/// <param name="_dependentResource">�ǉ�����ˑ����\�[�X</param>
void Device::AddDependentResource(DependentResource::Interface* const _dependentResource){
	if(_dependentResource == nullptr){
		throw Ishikawa::Common::Exception::FunctionFailed("You can't add NULL to list of device dependent resource.");
	}

	// �ǉ�����f�[�^�����X�g����O��
	_dependentResource->RemoveITDeviceDependentResourceList();

	DependentResource::Interface* const tail = this->deviceDependentResourceListEdge.GetBeforeITDeviceDependentResource();
	// �z���X�g�̈�Ԍ��ɒǉ�
	_dependentResource->SetBeforeITDeviceDependentResource(tail);
	_dependentResource->SetNextITDeviceDependentResource(&(this->deviceDependentResourceListEdge));
	tail->SetNextITDeviceDependentResource(_dependentResource);
	this->deviceDependentResourceListEdge.SetBeforeITDeviceDependentResource(_dependentResource);

	// �ˑ��E�B���h�E��ݒ肵�Ēʒm
	_dependentResource->SetDependentITDevice(this);
	_dependentResource->OnDeviceLoaded();
}

/// <summary>
/// �ˑ����\�[�X�̒ǉ�[�z���X�g����]
/// �ǉ�������͌����X�g�͋�ɂȂ�܂�
/// </summary>
/// <param name="_dependentResourceList">�ǉ�����ˑ����\�[�X�̏z���X�g�̒[</param>
void Device::AddDependentResourceList(DependentResource::ListEdge* const _dependentResourceListEdge){
	DependentResource::Interface* const headToBeAdded = _dependentResourceListEdge->GetNextITDeviceDependentResource();
	// �ǉ�������̂��Ȃ��̂ŉ��������I���
	if(headToBeAdded->IsITDeviceDependentResourceListEdge()) return;
	DependentResource::Interface* const tailToBeAdded = _dependentResourceListEdge->GetBeforeITDeviceDependentResource();

	// �ǉ����X�g���璆�g��؂藣��
	_dependentResourceListEdge->SetNextITDeviceDependentResource(_dependentResourceListEdge);
	_dependentResourceListEdge->SetBeforeITDeviceDependentResource(_dependentResourceListEdge);

	DependentResource::Interface* const tail = this->deviceDependentResourceListEdge.GetBeforeITDeviceDependentResource();
	// �؂藣�������g���z���X�g�̈�Ԍ��ɒǉ�
	headToBeAdded->SetBeforeITDeviceDependentResource(tail);
	tailToBeAdded->SetNextITDeviceDependentResource(&(this->deviceDependentResourceListEdge));
	tail->SetNextITDeviceDependentResource(headToBeAdded);
	this->deviceDependentResourceListEdge.SetBeforeITDeviceDependentResource(tailToBeAdded);

	// �ˑ��E�B���h�E��ݒ肵�Ēʒm
	DependentResource::Interface* resource = headToBeAdded;
	while(!(resource->IsITDeviceDependentResourceListEdge())){
		resource->SetDependentITDevice(this);
		resource->OnDeviceLoaded();

		resource = resource->GetNextITDeviceDependentResource();
	}
}


/// <summary>
/// �f�o�C�X�̐���
/// </summary>
/// <param name="_adapter1">�A�_�v�^</param>
void Device::CreateDevice(IDXGIAdapter1* const _adapter1){
	// �f�o�C�X�����t���O
	UINT createDeviceFlag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(DEBUG) | defined(_DEBUG) | defined(__DEBUG)
	createDeviceFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// �t�B�[�`���[���x��
	const D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};
	UINT featureLevelsCount = sizeof(featureLevels) / sizeof(featureLevels[0]);

	

	// �f�o�C�X����
	{
		this->ReleaseDevice();
		this->ReleaseContext();

		HRESULT hr = D3D11CreateDevice(
			_adapter1,
			D3D_DRIVER_TYPE_UNKNOWN,
			nullptr,
			createDeviceFlag,
			featureLevels,
			featureLevelsCount,
			D3D11_SDK_VERSION,
			&(this->device),
			&(this->featureLevel),
			&(this->context)
		);

		if(FAILED(hr)){
			// �ُ�I��
			throw Ishikawa::Common::Exception::FunctionFailed("Couldn't create device.");
		}
	}

#if !(defined(NDEBUG) || defined(_NDEBUG) || defined(__NDEBUG))
	Microsoft::WRL::ComPtr<ID3D11Debug> d3d11Debug;
	if(SUCCEEDED(this->device->QueryInterface<ID3D11Debug>(d3d11Debug.GetAddressOf()))){
		Microsoft::WRL::ComPtr<ID3D11InfoQueue> d3d11InfoQueue;
		if(SUCCEEDED(d3d11Debug.As(&d3d11InfoQueue))){
#	if defined(DEBUG) || defined(_DEBUG) || defined(__DEBUG)
			d3d11InfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
			d3d11InfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#	endif
			D3D11_MESSAGE_ID hide[] = {D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS};
			D3D11_INFO_QUEUE_FILTER filter = {};
			filter.DenyList.NumIDs = sizeof(hide) / sizeof(hide[0]);
			filter.DenyList.pIDList = hide;
			d3d11InfoQueue->AddStorageFilterEntries(&filter);
		}
	}
#endif

	ID3D11Device1* device1 = nullptr;
	if(SUCCEEDED(this->device->QueryInterface<ID3D11Device1>(&device1))){
		device1->Release();
	}

	// �f�o�C�X�����ʒm
//	for(const DxUser* user : this->users){
//		user->OnDeviceRestored();
//	}
}

/// <summary>
/// �A�_�v�^�̎擾
/// </summary>
/// <param name="_adapter1">�A�_�v�^�i�[��</param>
void Device::GetAdapter(IDXGIAdapter1** const _adapter1){
	(*_adapter1) = nullptr;

	// �t�@�N�g���쐬
	Microsoft::WRL::ComPtr<IDXGIFactory1> factory1;
	if(FAILED(CreateDXGIFactory1(IID_PPV_ARGS(factory1.GetAddressOf())))){
		throw Ishikawa::Common::Exception::FunctionFailed("CreateDXGIFactory1() failed!!");
	}

	// �A�_�v�^�擾
	Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter1;
	for(UINT adapterIndex = 0;
		DXGI_ERROR_NOT_FOUND != factory1->EnumAdapters1(0, adapter1.ReleaseAndGetAddressOf());
		++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 adapterDesc1;
		adapter1->GetDesc1(&adapterDesc1);

		if(adapterDesc1.Flags == DXGI_ADAPTER_FLAG_SOFTWARE){
			// ��{�����_�����O�h���C�o�A�_�v�^�͑I�����Ȃ�
			continue;
		}

#if defined(DEBUG) || defined(_DEBUG) || defined(__DEBUG)
		wchar_t buffer[256]{0};
		swprintf(buffer, L"Direct3D Adapter (%u): VID:%04X, PID:%04X - %ls\n",
			adapterIndex,
			adapterDesc1.VendorId,
			adapterDesc1.DeviceId,
			adapterDesc1.Description);
		OutputDebugStringW(buffer);
#endif

		break;
	}

	(*_adapter1) = adapter1.Detach();
}


/// <summary>
/// �f�o�C�X�̊J��
/// </summary>
void Device::ReleaseDevice(){
	if(this->device != nullptr){
		this->device->Release();
		this->device = nullptr;
	}
}

/// <summary>
/// �R���e�L�X�g�̊J��
/// </summary>
void Device::ReleaseContext(){
	if(this->context != nullptr){
		this->context->Release();
		this->context = nullptr;
	}
}
