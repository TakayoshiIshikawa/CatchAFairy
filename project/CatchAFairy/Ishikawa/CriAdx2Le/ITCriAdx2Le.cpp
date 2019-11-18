//=============================================================================
//	ITCriAdx2Le.cpp
//
//	����CRI_ADX2_Le���p�̃\�[�X�t�@�C��
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

/// <summary>�V���O���g���C���X�^���X</summary>
ITCriAdx2Le* ITCriAdx2Le::instance = nullptr;


/// <summary>
/// �C���X�^���X�̎擾[����E�B���h�E�}�l�[�W��]
/// </summary>
/// <returns>����E�B���h�E�}�l�[�W���̃C���X�^���X</returns>
template<>
ITCriAdx2Le& Ishikawa::Common::SingletonManager::GetInstance<ITCriAdx2Le>(){
	if(ITCriAdx2Le::instance == nullptr){
		ITCriAdx2Le::instance = new ITCriAdx2Le();
	}
	return (*(ITCriAdx2Le::instance));
}


#pragma region PimplOfITCriAdx2Le
/// <summary>
/// �B���p
/// </summary>
class ITCriAdx2Le::PimplOfITCriAdx2Le final{
private:
	/// <summary>�{�C�X�v�[���n���h��</summary>
	CriAtomExVoicePoolHn voicePoolHandle;
	/// <summary>D-BAS�n���h��</summary>
	CriAtomDbasId dBasHandle;
	/// <summary>ACB�n���h��</summary>
	CriAtomExAcbHn acbHandle;
	/// <summary>�v���[���n���h��</summary>
	CriAtomExPlayerHn playerHandle;


public:
	// �f�t�H���g�R���X�g���N�^
	PimplOfITCriAdx2Le();
	// �f�X�g���N�^
	~PimplOfITCriAdx2Le();

public:
	// �{�����[���ݒ�
	inline void SetVolume(float _volume);

public:
	// ������
	inline void Initialize(const char* const _acfFilepath, const char* const _acbFilepath, const char* const _awbFilepath);
	// �X�V
	inline void Update();
	// �I��
	inline void Finalize();

	// acb�t�@�C���ύX
	inline void ChangeAcbFile(const char* const _acbFilepath, const char* const _awbFilepath);

	// �Đ�
	inline unsigned int Play(int _number);
	// ��~
	inline void Stop(unsigned int _id);

private:
	// ���[�U��`�G���[�R�[���o�b�N�֐�
	static void UserErrorCallbackFuncsion(const CriChar8* _errorId, CriUint32 _p1, CriUint32 _p2, CriUint32* const _array);
	// ���[�U��`�������m�ۊ֐�
	static void* UserAllocFuncsion(void* _object, CriUint32 _size);
	// ���[�U��`����������֐�
	static void UserFreeFuncsion(void* _object, void* _pointer);
};


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
ITCriAdx2Le::PimplOfITCriAdx2Le::PimplOfITCriAdx2Le(){
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ITCriAdx2Le::PimplOfITCriAdx2Le::~PimplOfITCriAdx2Le(){
	// �������Ȃ�
}


/// <summary>
/// �{�����[���ݒ�
/// </summary>
/// <param name="_volume">����</param>
void ITCriAdx2Le::PimplOfITCriAdx2Le::SetVolume(float _volume){
	criAtomExPlayer_SetVolume(this->playerHandle, _volume);
	criAtomExPlayer_UpdateAll(this->playerHandle);
}


/// <summary>
/// ������
/// </summary>
/// <param name="_acfFilepath">acf�t�@�C���p�X</param>
/// <param name="_acbFilepath">acb�t�@�C���p�X</param>
/// <param name="_awbFilepath">awb�t�@�C���p�X</param>
void ITCriAdx2Le::PimplOfITCriAdx2Le::Initialize(
	const char* const _acfFilepath,
	const char* const _acbFilepath,
	const char* const _awbFilepath
){
	// �G���[�R�[���o�b�N�֐��̓o�^
	criErr_SetCallback(UserErrorCallbackFuncsion);
	// �������A���P�[�^�̓o�^
	criAtomEx_SetUserAllocator(UserAllocFuncsion, UserFreeFuncsion, nullptr);

	// ���C�u����������
	criAtomEx_Initialize_WASAPI(nullptr, nullptr, 0);

	// �X�g���[�~���O�p�o�b�t�@�̍쐬
	this->dBasHandle = criAtomDbas_Create(nullptr, nullptr, 0);
	// �S�̐ݒ�t�@�C���̓o�^
	criAtomEx_RegisterAcfFile(nullptr, _acfFilepath, nullptr, 0);
	// DSP�o�X�ݒ�̓o�^
	criAtomEx_AttachDspBusSetting("DspBusSetting_0", nullptr, 0);
	// �{�C�X�v�[���̍쐬
	CriAtomExStandardVoicePoolConfig vpconfig;
	criAtomExVoicePool_SetDefaultConfigForStandardVoicePool(&vpconfig);
	vpconfig.player_config.streaming_flag = CRI_TRUE;
	this->voicePoolHandle = criAtomExVoicePool_AllocateStandardVoicePool(&vpconfig, nullptr, 0);
	// �T�E���h�f�[�^�̓ǂݍ���
	this->acbHandle = criAtomExAcb_LoadAcbFile(nullptr, _acbFilepath, nullptr, _awbFilepath, nullptr, 0);
	// �v���[���n���h���̍쐬
	this->playerHandle = criAtomExPlayer_Create(nullptr, nullptr, 0);
}

/// <summary>
/// �X�V
/// </summary>
void ITCriAdx2Le::PimplOfITCriAdx2Le::Update(){
	criAtomEx_ExecuteMain();
}

/// <summary>
/// �I��
/// </summary>
void ITCriAdx2Le::PimplOfITCriAdx2Le::Finalize(){
	// �v���[���n���h���̔j��
	criAtomExPlayer_Destroy(this->playerHandle);
	// ACB�n���h���̉��
	criAtomExAcb_Release(this->acbHandle);
	// �{�C�X�v�[���̉��
	criAtomExVoicePool_Free(this->voicePoolHandle);
	// �S�̐ݒ�t�@�C���̓o�^����
	criAtomEx_UnregisterAcf();
	// ���C�u�����̏I��
	criAtomEx_Finalize_WASAPI();
}


/// <summary>
/// acb�t�@�C���ύX
/// </summary>
/// <param name="_acbFilepath">acb�t�@�C���p�X</param>
/// <param name="_awbFilepath">awb�t�@�C���p�X</param>
void ITCriAdx2Le::PimplOfITCriAdx2Le::ChangeAcbFile(
	const char* const _acbFilepath,
	const char* const _awbFilepath
){
	// ACB�n���h���̉��
	criAtomExAcb_Release(this->acbHandle);
	// �T�E���h�f�[�^�̓ǂݍ���
	this->acbHandle = criAtomExAcb_LoadAcbFile(nullptr, _acbFilepath, nullptr, _awbFilepath, nullptr, 0);
}


/// <summary>
/// �Đ�
/// </summary>
/// <param name="_number">�Đ����鉹�̔ԍ�</param>
/// <returns>�Đ�ID</returns>
unsigned int ITCriAdx2Le::PimplOfITCriAdx2Le::Play(int _number){
	// �L���[�̎w��
	criAtomExPlayer_SetCueId(this->playerHandle, this->acbHandle, _number);
	// �L���[�̍Đ��J�n
	return criAtomExPlayer_Start(this->playerHandle);
}

/// <summary>
/// ��~
/// </summary>
/// <param name="_id">�Đ�ID</param>
void ITCriAdx2Le::PimplOfITCriAdx2Le::Stop(unsigned int _id){
	// �L���[�̍Đ��I��
	criAtomExPlayback_Stop(_id);
}


/// <summary>
/// ���[�U��`�G���[�R�[���o�b�N�֐�
/// </summary>
/// <param name="_errorId">�G���[ID</param>
/// <param name="_p1">�⑫���1</param>
/// <param name="_p2">�⑫���2</param>
/// <param name="_array">�z��f�[�^</param>
void ITCriAdx2Le::PimplOfITCriAdx2Le::UserErrorCallbackFuncsion(const CriChar8* _errorId, CriUint32 _p1, CriUint32 _p2, CriUint32* const _array){
	NOT_USING(_array);
	// �G���[�R�[�h����G���[������ɕϊ����ăR���\�[���o�͂���
	OutputDebugStringA(criErr_ConvertIdToMessage(_errorId, _p1, _p2));
}

/// <summary>
/// ���[�U��`�������m�ۊ֐�
/// </summary>
/// <param name="_object">�I�u�W�F�N�g</param>
/// <param name="_size">�m�ۂ��郁�����T�C�Y</param>
/// <returns>�m�ۂ����������ւ̃|�C���^</returns>
void* ITCriAdx2Le::PimplOfITCriAdx2Le::UserAllocFuncsion(void* _object, CriUint32 _size){
	NOT_USING(_object);
	return malloc(_size);
}

/// <summary>
/// ���[�U��`����������֐�
/// </summary>
/// <param name="_object">�I�u�W�F�N�g</param>
/// <param name="_pointer">�J�����郁�����ւ̃|�C���^</param>
void ITCriAdx2Le::PimplOfITCriAdx2Le::UserFreeFuncsion(void* _object, void* _pointer){
	NOT_USING(_object);
	free(_pointer);
}

#pragma endregion PimplOfITCriAdx2Le


/// <summary>
/// �f�t�H���g�R���X�g���N�^
/// </summary>
ITCriAdx2Le::ITCriAdx2Le() :
	itCriAdx2Le(new PimplOfITCriAdx2Le())
{
	// �������Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ITCriAdx2Le::~ITCriAdx2Le(){
	// Pimpl�f�[�^�폜
	if(this->itCriAdx2Le != nullptr){
		delete this->itCriAdx2Le;
		this->itCriAdx2Le = nullptr;
	}

	// �V���O���g���C���X�^���X�̃��Z�b�g
	instance = nullptr;
}


/// <summary>
/// �{�����[���ݒ�
/// </summary>
/// <param name="_volume">����</param>
void ITCriAdx2Le::SetVolume(float _volume){
	this->itCriAdx2Le->SetVolume(_volume);
}


/// <summary>
/// ������
/// </summary>
/// <param name="_acfFilepath">acf�t�@�C���p�X</param>
/// <param name="_acbFilepath">acb�t�@�C���p�X</param>
/// <param name="_awbFilepath">awb�t�@�C���p�X</param>
void ITCriAdx2Le::Initialize(
	const char* const _acfFilepath,
	const char* const _acbFilepath,
	const char* const _awbFilepath
){
	this->itCriAdx2Le->Initialize(_acfFilepath, _acbFilepath, _awbFilepath);
}

/// <summary>
/// �X�V
/// </summary>
void ITCriAdx2Le::Update(){
	this->itCriAdx2Le->Update();
}

/// <summary>
/// �I��
/// </summary>
void ITCriAdx2Le::Finalize(){
	this->itCriAdx2Le->Finalize();
}


/// <summary>
/// acb�t�@�C���ύX
/// </summary>
/// <param name="_acbFilepath">acb�t�@�C���p�X</param>
/// <param name="_awbFilepath">awb�t�@�C���p�X</param>
void ITCriAdx2Le::ChangeAcbFile(const char* const _acbFilepath, const char* const _awbFilepath){
	this->itCriAdx2Le->ChangeAcbFile(_acbFilepath, _awbFilepath);
}


/// <summary>
/// �Đ�
/// </summary>
/// <param name="_number">�Đ����鉹�̔ԍ�</param>
/// <returns>�Đ�ID</returns>
unsigned int ITCriAdx2Le::Play(int _number){
	return this->itCriAdx2Le->Play(_number);
}

/// <summary>
/// ��~
/// </summary>
/// <param name="_id">�Đ�ID</param>
void ITCriAdx2Le::Stop(unsigned int _id){
	this->itCriAdx2Le->Stop(_id);
}
