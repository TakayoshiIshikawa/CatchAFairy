//=============================================================================
//	SceneBase.cpp
//
//	シーン基底のソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "SceneBase.h"

using namespace CatchAFairy::Scene;


/// <summary>更新フラグ</summary>
const unsigned long Base::SCENE_UPDATE_FLAG	= 0x00000001;
/// <summary>描画フラグ</summary>
const unsigned long Base::SCENE_RENDER_FLAG	= 0x00000002;


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
Base::Base() :
	nextScene(nullptr),
	beforeScene(nullptr)
{
	// 何もしない
}

/// <summary>
/// デストラクタ
/// </summary>
Base::~Base(){
	this->RemoveSceneList();
}


/// <summary>
/// 更新するか?
/// </summary>
/// <returns>true:更新する false:更新しない</returns>
bool Base::IsUpdateScene() const{
	return ((this->sceneFlag & SCENE_UPDATE_FLAG) != 0);
}

/// <summary>
/// 描画するか?
/// </summary>
/// <returns>true:描画する false:描画しない</returns>
bool Base::IsRenderScene() const{
	return ((this->sceneFlag & SCENE_RENDER_FLAG) != 0);
}


/// <summary>
/// 更新フラグの設定
/// </summary>
/// <param name="_flag">更新フラグ</param>
void Base::SetUpdateFlag(bool _flag){
	if(_flag)	this->sceneFlag |= SCENE_UPDATE_FLAG;
	else		this->sceneFlag &= (~SCENE_UPDATE_FLAG);
}

/// <summary>
/// 描画フラグの設定
/// </summary>
/// <param name="_flag">描画フラグ</param>
void Base::SetRenderFlag(bool _flag){
	if(_flag)	this->sceneFlag |= SCENE_RENDER_FLAG;
	else		this->sceneFlag &= (~SCENE_RENDER_FLAG);
}


/// <summary>
/// リストから外れる
/// </summary>
void Base::RemoveSceneList(){
	// 所属している循環リストから外れる
	Interface* const next = this->nextScene;
	Interface* const before = this->beforeScene;
	next->SetBeforeScene(before);
	before->SetNextScene(next);
	// 自身で循環させる
	this->nextScene = this;
	this->beforeScene = this;
}


/// <summary>
/// シーンの初期化
/// </summary>
void Base::InitializeScene(){
	// 自身で循環させる
	this->nextScene = this;
	this->beforeScene = this;
}
