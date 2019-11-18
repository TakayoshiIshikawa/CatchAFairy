//=============================================================================
//	ITDevice.cpp
//
//	自作デバイスのソースファイル
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
/// デフォルトコンストラクタ
/// </summary>
Device::Device() :
	device(nullptr),
	featureLevel(D3D_FEATURE_LEVEL_9_1),
	context(nullptr),
	deviceDependentResourceListEdge()
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
Device::~Device(){
	this->ReleaseContext();
	this->ReleaseDevice();
}


/// <summary>
/// 初期化
/// </summary>
void Device::Initialize(){
	// メンバの初期化
	this->deviceDependentResourceListEdge.Initialize();
}

/// <summary>
/// 初期化
/// </summary>
void Device::Create(){
	// アダプタ取得
	Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter1;
	this->GetAdapter(adapter1.GetAddressOf());
	if(adapter1 == nullptr){
		// 異常終了
		throw Ishikawa::Common::Exception::FunctionFailed("Couldn't get adapter.");
	}

	// デバイス生成
	this->CreateDevice(adapter1.Get());
}

/// <summary>
/// 復元
/// </summary>
void Device::Restore(){
	// 生成し直す
	this->Create();

	// 復元通知
	DependentResource::Interface* resource = this->deviceDependentResourceListEdge.GetNextITDeviceDependentResource();
	while(!(resource->IsITDeviceDependentResourceListEdge())){
		resource->OnDeviceRestored();

		resource = resource->GetNextITDeviceDependentResource();
	}
}

/// <summary>
/// ロスト
/// </summary>
void Device::Lost(){
	// 開放
	this->ReleaseContext();
	this->ReleaseDevice();

	// ロスト通知
	DependentResource::Interface* resource = this->deviceDependentResourceListEdge.GetNextITDeviceDependentResource();
	while(!(resource->IsITDeviceDependentResourceListEdge())){
		resource->OnDeviceLost();

		resource = resource->GetNextITDeviceDependentResource();
	}
}


/// <summary>
/// 依存リソースの追加[1つだけ]
/// </summary>
/// <param name="_dependentResource">追加する依存リソース</param>
void Device::AddDependentResource(DependentResource::Interface* const _dependentResource){
	if(_dependentResource == nullptr){
		throw Ishikawa::Common::Exception::FunctionFailed("You can't add NULL to list of device dependent resource.");
	}

	// 追加するデータをリストから外す
	_dependentResource->RemoveITDeviceDependentResourceList();

	DependentResource::Interface* const tail = this->deviceDependentResourceListEdge.GetBeforeITDeviceDependentResource();
	// 循環リストの一番後ろに追加
	_dependentResource->SetBeforeITDeviceDependentResource(tail);
	_dependentResource->SetNextITDeviceDependentResource(&(this->deviceDependentResourceListEdge));
	tail->SetNextITDeviceDependentResource(_dependentResource);
	this->deviceDependentResourceListEdge.SetBeforeITDeviceDependentResource(_dependentResource);

	// 依存ウィンドウを設定して通知
	_dependentResource->SetDependentITDevice(this);
	_dependentResource->OnDeviceLoaded();
}

/// <summary>
/// 依存リソースの追加[循環リストごと]
/// 追加した後は元リストは空になります
/// </summary>
/// <param name="_dependentResourceList">追加する依存リソースの循環リストの端</param>
void Device::AddDependentResourceList(DependentResource::ListEdge* const _dependentResourceListEdge){
	DependentResource::Interface* const headToBeAdded = _dependentResourceListEdge->GetNextITDeviceDependentResource();
	// 追加するものがないので何もせず終わる
	if(headToBeAdded->IsITDeviceDependentResourceListEdge()) return;
	DependentResource::Interface* const tailToBeAdded = _dependentResourceListEdge->GetBeforeITDeviceDependentResource();

	// 追加リストから中身を切り離す
	_dependentResourceListEdge->SetNextITDeviceDependentResource(_dependentResourceListEdge);
	_dependentResourceListEdge->SetBeforeITDeviceDependentResource(_dependentResourceListEdge);

	DependentResource::Interface* const tail = this->deviceDependentResourceListEdge.GetBeforeITDeviceDependentResource();
	// 切り離した中身を循環リストの一番後ろに追加
	headToBeAdded->SetBeforeITDeviceDependentResource(tail);
	tailToBeAdded->SetNextITDeviceDependentResource(&(this->deviceDependentResourceListEdge));
	tail->SetNextITDeviceDependentResource(headToBeAdded);
	this->deviceDependentResourceListEdge.SetBeforeITDeviceDependentResource(tailToBeAdded);

	// 依存ウィンドウを設定して通知
	DependentResource::Interface* resource = headToBeAdded;
	while(!(resource->IsITDeviceDependentResourceListEdge())){
		resource->SetDependentITDevice(this);
		resource->OnDeviceLoaded();

		resource = resource->GetNextITDeviceDependentResource();
	}
}


/// <summary>
/// デバイスの生成
/// </summary>
/// <param name="_adapter1">アダプタ</param>
void Device::CreateDevice(IDXGIAdapter1* const _adapter1){
	// デバイス生成フラグ
	UINT createDeviceFlag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(DEBUG) | defined(_DEBUG) | defined(__DEBUG)
	createDeviceFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// フィーチャーレベル
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

	

	// デバイス生成
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
			// 異常終了
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

	// デバイス生成通知
//	for(const DxUser* user : this->users){
//		user->OnDeviceRestored();
//	}
}

/// <summary>
/// アダプタの取得
/// </summary>
/// <param name="_adapter1">アダプタ格納先</param>
void Device::GetAdapter(IDXGIAdapter1** const _adapter1){
	(*_adapter1) = nullptr;

	// ファクトリ作成
	Microsoft::WRL::ComPtr<IDXGIFactory1> factory1;
	if(FAILED(CreateDXGIFactory1(IID_PPV_ARGS(factory1.GetAddressOf())))){
		throw Ishikawa::Common::Exception::FunctionFailed("CreateDXGIFactory1() failed!!");
	}

	// アダプタ取得
	Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter1;
	for(UINT adapterIndex = 0;
		DXGI_ERROR_NOT_FOUND != factory1->EnumAdapters1(0, adapter1.ReleaseAndGetAddressOf());
		++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 adapterDesc1;
		adapter1->GetDesc1(&adapterDesc1);

		if(adapterDesc1.Flags == DXGI_ADAPTER_FLAG_SOFTWARE){
			// 基本レンダリングドライバアダプタは選択しない
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
/// デバイスの開放
/// </summary>
void Device::ReleaseDevice(){
	if(this->device != nullptr){
		this->device->Release();
		this->device = nullptr;
	}
}

/// <summary>
/// コンテキストの開放
/// </summary>
void Device::ReleaseContext(){
	if(this->context != nullptr){
		this->context->Release();
		this->context = nullptr;
	}
}
