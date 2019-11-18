//=============================================================================
//	ITCriAdx2Le.cpp
//
//	自作CRI_ADX2_Le利用のソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ITCriAdx2Le.h"
#include <locale.h>
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#include <cri_adx2le.h>
#pragma warning(default:4820)
#pragma warning(default:4668)
#include "Ishikawa/Common/PatternBase/SingletonManager.h"

using namespace ::Ishikawa::CriAdx2Le;

/// <summary>シングルトンインスタンス</summary>
ITCriAdx2Le* ITCriAdx2Le::instance = nullptr;


/// <summary>
/// インスタンスの取得[自作ウィンドウマネージャ]
/// </summary>
/// <returns>自作ウィンドウマネージャのインスタンス</returns>
template<>
ITCriAdx2Le& Ishikawa::Common::SingletonManager::GetInstance<ITCriAdx2Le>(){
	if(ITCriAdx2Le::instance == nullptr){
		ITCriAdx2Le::instance = new ITCriAdx2Le();
	}
	return (*(ITCriAdx2Le::instance));
}


#pragma region PimplOfITCriAdx2Le
/// <summary>
/// 隠蔽用
/// </summary>
class ITCriAdx2Le::PimplOfITCriAdx2Le final{
private:
	/// <summary>ボイスプールハンドル</summary>
	CriAtomExVoicePoolHn voicePoolHandle;
	/// <summary>D-BASハンドル</summary>
	CriAtomDbasId dBasHandle;
	/// <summary>ACBハンドル</summary>
	CriAtomExAcbHn acbHandle;
	/// <summary>プレーヤハンドル</summary>
	CriAtomExPlayerHn playerHandle;


public:
	// デフォルトコンストラクタ
	PimplOfITCriAdx2Le();
	// デストラクタ
	~PimplOfITCriAdx2Le();

public:
	// ボリューム設定
	inline void SetVolume(float _volume);

public:
	// 初期化
	inline void Initialize(const char* const _acfFilepath, const char* const _acbFilepath, const char* const _awbFilepath);
	// 更新
	inline void Update();
	// 終了
	inline void Finalize();

	// acbファイル変更
	inline void ChangeAcbFile(const char* const _acbFilepath, const char* const _awbFilepath);

	// 再生
	inline unsigned int Play(int _number);
	// 停止
	inline void Stop(unsigned int _id);

private:
	// ユーザ定義エラーコールバック関数
	static void UserErrorCallbackFuncsion(const CriChar8* _errorId, CriUint32 _p1, CriUint32 _p2, CriUint32* const _array);
	// ユーザ定義メモリ確保関数
	static void* UserAllocFuncsion(void* _object, CriUint32 _size);
	// ユーザ定義メモリ解放関数
	static void UserFreeFuncsion(void* _object, void* _pointer);
};


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
ITCriAdx2Le::PimplOfITCriAdx2Le::PimplOfITCriAdx2Le(){
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
ITCriAdx2Le::PimplOfITCriAdx2Le::~PimplOfITCriAdx2Le(){
	// 何もしない
}


/// <summary>
/// ボリューム設定
/// </summary>
/// <param name="_volume">音量</param>
void ITCriAdx2Le::PimplOfITCriAdx2Le::SetVolume(float _volume){
	criAtomExPlayer_SetVolume(this->playerHandle, _volume);
	criAtomExPlayer_UpdateAll(this->playerHandle);
}


/// <summary>
/// 初期化
/// </summary>
/// <param name="_acfFilepath">acfファイルパス</param>
/// <param name="_acbFilepath">acbファイルパス</param>
/// <param name="_awbFilepath">awbファイルパス</param>
void ITCriAdx2Le::PimplOfITCriAdx2Le::Initialize(
	const char* const _acfFilepath,
	const char* const _acbFilepath,
	const char* const _awbFilepath
){
	// エラーコールバック関数の登録
	criErr_SetCallback(UserErrorCallbackFuncsion);
	// メモリアロケータの登録
	criAtomEx_SetUserAllocator(UserAllocFuncsion, UserFreeFuncsion, nullptr);

	// ライブラリ初期化
	criAtomEx_Initialize_WASAPI(nullptr, nullptr, 0);

	// ストリーミング用バッファの作成
	this->dBasHandle = criAtomDbas_Create(nullptr, nullptr, 0);
	// 全体設定ファイルの登録
	criAtomEx_RegisterAcfFile(nullptr, _acfFilepath, nullptr, 0);
	// DSPバス設定の登録
	criAtomEx_AttachDspBusSetting("DspBusSetting_0", nullptr, 0);
	// ボイスプールの作成
	CriAtomExStandardVoicePoolConfig vpconfig;
	criAtomExVoicePool_SetDefaultConfigForStandardVoicePool(&vpconfig);
	vpconfig.player_config.streaming_flag = CRI_TRUE;
	this->voicePoolHandle = criAtomExVoicePool_AllocateStandardVoicePool(&vpconfig, nullptr, 0);
	// サウンドデータの読み込み
	this->acbHandle = criAtomExAcb_LoadAcbFile(nullptr, _acbFilepath, nullptr, _awbFilepath, nullptr, 0);
	// プレーヤハンドルの作成
	this->playerHandle = criAtomExPlayer_Create(nullptr, nullptr, 0);
}

/// <summary>
/// 更新
/// </summary>
void ITCriAdx2Le::PimplOfITCriAdx2Le::Update(){
	criAtomEx_ExecuteMain();
}

/// <summary>
/// 終了
/// </summary>
void ITCriAdx2Le::PimplOfITCriAdx2Le::Finalize(){
	// プレーヤハンドルの破棄
	criAtomExPlayer_Destroy(this->playerHandle);
	// ACBハンドルの解放
	criAtomExAcb_Release(this->acbHandle);
	// ボイスプールの解放
	criAtomExVoicePool_Free(this->voicePoolHandle);
	// 全体設定ファイルの登録解除
	criAtomEx_UnregisterAcf();
	// ライブラリの終了
	criAtomEx_Finalize_WASAPI();
}


/// <summary>
/// acbファイル変更
/// </summary>
/// <param name="_acbFilepath">acbファイルパス</param>
/// <param name="_awbFilepath">awbファイルパス</param>
void ITCriAdx2Le::PimplOfITCriAdx2Le::ChangeAcbFile(
	const char* const _acbFilepath,
	const char* const _awbFilepath
){
	// ACBハンドルの解放
	criAtomExAcb_Release(this->acbHandle);
	// サウンドデータの読み込み
	this->acbHandle = criAtomExAcb_LoadAcbFile(nullptr, _acbFilepath, nullptr, _awbFilepath, nullptr, 0);
}


/// <summary>
/// 再生
/// </summary>
/// <param name="_number">再生する音の番号</param>
/// <returns>再生ID</returns>
unsigned int ITCriAdx2Le::PimplOfITCriAdx2Le::Play(int _number){
	// キューの指定
	criAtomExPlayer_SetCueId(this->playerHandle, this->acbHandle, _number);
	// キューの再生開始
	return criAtomExPlayer_Start(this->playerHandle);
}

/// <summary>
/// 停止
/// </summary>
/// <param name="_id">再生ID</param>
void ITCriAdx2Le::PimplOfITCriAdx2Le::Stop(unsigned int _id){
	// キューの再生終了
	criAtomExPlayback_Stop(_id);
}


/// <summary>
/// ユーザ定義エラーコールバック関数
/// </summary>
/// <param name="_errorId">エラーID</param>
/// <param name="_p1">補足情報1</param>
/// <param name="_p2">補足情報2</param>
/// <param name="_array">配列データ</param>
void ITCriAdx2Le::PimplOfITCriAdx2Le::UserErrorCallbackFuncsion(const CriChar8* _errorId, CriUint32 _p1, CriUint32 _p2, CriUint32* const _array){
	NOT_USING(_array);
	// エラーコードからエラー文字列に変換してコンソール出力する
	OutputDebugStringA(criErr_ConvertIdToMessage(_errorId, _p1, _p2));
}

/// <summary>
/// ユーザ定義メモリ確保関数
/// </summary>
/// <param name="_object">オブジェクト</param>
/// <param name="_size">確保するメモリサイズ</param>
/// <returns>確保したメモリへのポインタ</returns>
void* ITCriAdx2Le::PimplOfITCriAdx2Le::UserAllocFuncsion(void* _object, CriUint32 _size){
	NOT_USING(_object);
	return malloc(_size);
}

/// <summary>
/// ユーザ定義メモリ解放関数
/// </summary>
/// <param name="_object">オブジェクト</param>
/// <param name="_pointer">開放するメモリへのポインタ</param>
void ITCriAdx2Le::PimplOfITCriAdx2Le::UserFreeFuncsion(void* _object, void* _pointer){
	NOT_USING(_object);
	free(_pointer);
}

#pragma endregion PimplOfITCriAdx2Le


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
ITCriAdx2Le::ITCriAdx2Le() :
	itCriAdx2Le(new PimplOfITCriAdx2Le())
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
ITCriAdx2Le::~ITCriAdx2Le(){
	// Pimplデータ削除
	if(this->itCriAdx2Le != nullptr){
		delete this->itCriAdx2Le;
		this->itCriAdx2Le = nullptr;
	}

	// シングルトンインスタンスのリセット
	instance = nullptr;
}


/// <summary>
/// ボリューム設定
/// </summary>
/// <param name="_volume">音量</param>
void ITCriAdx2Le::SetVolume(float _volume){
	this->itCriAdx2Le->SetVolume(_volume);
}


/// <summary>
/// 初期化
/// </summary>
/// <param name="_acfFilepath">acfファイルパス</param>
/// <param name="_acbFilepath">acbファイルパス</param>
/// <param name="_awbFilepath">awbファイルパス</param>
void ITCriAdx2Le::Initialize(
	const char* const _acfFilepath,
	const char* const _acbFilepath,
	const char* const _awbFilepath
){
	this->itCriAdx2Le->Initialize(_acfFilepath, _acbFilepath, _awbFilepath);
}

/// <summary>
/// 更新
/// </summary>
void ITCriAdx2Le::Update(){
	this->itCriAdx2Le->Update();
}

/// <summary>
/// 終了
/// </summary>
void ITCriAdx2Le::Finalize(){
	this->itCriAdx2Le->Finalize();
}


/// <summary>
/// acbファイル変更
/// </summary>
/// <param name="_acbFilepath">acbファイルパス</param>
/// <param name="_awbFilepath">awbファイルパス</param>
void ITCriAdx2Le::ChangeAcbFile(const char* const _acbFilepath, const char* const _awbFilepath){
	this->itCriAdx2Le->ChangeAcbFile(_acbFilepath, _awbFilepath);
}


/// <summary>
/// 再生
/// </summary>
/// <param name="_number">再生する音の番号</param>
/// <returns>再生ID</returns>
unsigned int ITCriAdx2Le::Play(int _number){
	return this->itCriAdx2Le->Play(_number);
}

/// <summary>
/// 停止
/// </summary>
/// <param name="_id">再生ID</param>
void ITCriAdx2Le::Stop(unsigned int _id){
	this->itCriAdx2Le->Stop(_id);
}
