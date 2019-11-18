//=============================================================================
//	Player.cpp
//
//	プレイヤのソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "Player.h"
#include "Ishikawa/Common/Exception/ITExceptions.h"
#include "Ishikawa/Model/ITObjectLoader.h"
#include "Ishikawa/DirectX/Device/ITDevice.h"
#include "CatchAFairy/Shader/User/ShaderVertexTopology.h"
#include "CatchAFairy/Shader/User/PlayCameraEyeFirstShaderUser.h"
#include "CatchAFairy/Game.h"
#include "CatchAFairy/Scene/ScenePlay.h"
#include "CatchAFairy/Object/PlayScene/GUI/OperationMethodView.h"
#include "PlayCamera.h"

using namespace CatchAFairy::Object::PlayScene;


/// <summary>自作モデルデータファイルパス</summary>
const char* const Player::IT_MODEL_DATA_FILEPATH = "Resources/Models/TestFairy001.itobj";
/// <summary>拡散反射テクスチャファイルパス</summary>
const wchar_t* const Player::DIFFUSE_TEXTURE_FILEPATH = L"Resources/Textures/White.png";
/// <summary>法線テクスチャファイルパス</summary>
const wchar_t* const Player::NORMAL_TEXTURE_FILEPATH = L"Resources/Textures/DefaultNormal.png";
/// <summary>妖精ID</summary>
const unsigned int Player::FAIRY_ID = 0U;
/// <summary>本体ライトのずれ</summary>
const ::Ishikawa::Math::Float3 Player::FAIRY_LIGHT_ANCHOR(0.0f, 0.1f, 0.0f);
/// <summary>本体ライトの色</summary>
const ::Ishikawa::Math::Float4 Player::FAIRY_LIGHT_COLOR(0.9f, 0.9f, 1.0f, 0.7f);
/// <summary>本体ライトの半径</summary>
const float Player::FAIRY_LIGHT_RADIUS = 0.03f;
/// <summary>本体ライトの光量</summary>
const float Player::FAIRY_LIGHT_LUMEN = 0.4f;
/// <summary>基本のマナライトの色</summary>
const ::Ishikawa::Math::Float4 Player::DEFAULT_MANA_LIGHT_COLOR(0.9f, 0.9f, 1.0f, 0.6f);
/// <summary>各妖精を捕まえたときのマナライトの色</summary>
const ::Ishikawa::Math::Float4 Player::CATCHED_FAIRY_MANA_LIGHT_COLOR[static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)] = {
	::Ishikawa::Math::Float4(0.0f, 1.0f, 0.0f, 0.9f),
	::Ishikawa::Math::Float4(1.0f, 1.0f, 0.0f, 0.9f),
	::Ishikawa::Math::Float4(0.0f, 0.0f, 1.0f, 0.9f)
};
/// <summary>マナライトの半径</summary>
const float Player::MANA_LIGHT_RADIUS = 0.002f;
/// <summary>マナライトの光量</summary>
const float Player::MANA_LIGHT_LUMEN = 0.04f;
/// <summary>最小のマナライト生成カウント増分比率</summary>
const float Player::MINIMUM_MANA_LIGHT_GENERATE_COUNT_ADDITIONAL_RATIO = 0.5f;
/// <summary>最大のマナライト生成カウント増分比率</summary>
const float Player::MAXIMUM_MANA_LIGHT_GENERATE_COUNT_ADDITIONAL_RATIO = 10.0f;
/// <summary>開始位置</summary>
const ::Ishikawa::Math::Float3 Player::START_POSITION(0.0f, 1.5f, 0.0f);
/// <summary>移動速度</summary>
const float Player::MOVE_SPEED = 3.0f;
/// <summary>回転速度</summary>
const float Player::ROTATE_SPEED = 2.5f;
/// <summary>捕獲間隔</summary>
const float Player::CATCH_INTERVAL = 1.5f;

/// <summary>妖精を捕まえられる最低高さ</summary>
const float Player::FAIRY_CATCHABLE_HEIGHT_MIN = -0.5f;
/// <summary>妖精を捕まえられる最高高さ</summary>
const float Player::FAIRY_CATCHABLE_HEIGHT_MAX = 0.5f;
/// <summary>妖精を捕まえられる半径</summary>
const float Player::FAIRY_CATCHABLE_RADIUS = 0.5f;
/// <summary>妖精を捕まえられる扇形半径</summary>
const float Player::FAIRY_CATCHABLE_ANGLE_RADIUS = 1.5f;
/// <summary>妖精を捕まえられる角度のコサイン値</summary>
const float Player::FAIRY_CATCHABLE_ANGLE_COS = 0.5f;

/// <summary>前進キー</summary>
const ::Ishikawa::Input::KeyCode Player::FRONT_MOVE_KEY = ::Ishikawa::Input::KeyCode::Z;
/// <summary>後進キー [使用なし]</summary>
const ::Ishikawa::Input::KeyCode Player::BACK_MOVE_KEY = ::Ishikawa::Input::KeyCode::DownArrow;
/// <summary>右進キー [使用なし]</summary>
const ::Ishikawa::Input::KeyCode Player::RIGHT_MOVE_KEY = ::Ishikawa::Input::KeyCode::A;
/// <summary>左進キー [使用なし]</summary>
const ::Ishikawa::Input::KeyCode Player::LEFT_MOVE_KEY = ::Ishikawa::Input::KeyCode::D;
/// <summary>上進キー</summary>
const ::Ishikawa::Input::KeyCode Player::UP_MOVE_KEY = ::Ishikawa::Input::KeyCode::UpArrow;
/// <summary>下進キー</summary>
const ::Ishikawa::Input::KeyCode Player::DOWN_MOVE_KEY = ::Ishikawa::Input::KeyCode::DownArrow;
/// <summary>右旋回キー</summary>
const ::Ishikawa::Input::KeyCode Player::RIGHT_ROTATE_KEY = ::Ishikawa::Input::KeyCode::RightArrow;
/// <summary>左旋回キー</summary>
const ::Ishikawa::Input::KeyCode Player::LEFT_ROTATE_KEY = ::Ishikawa::Input::KeyCode::LeftArrow;
/// <summary>捕獲キー</summary>
const ::Ishikawa::Input::KeyCode Player::CATCH_KEY = ::Ishikawa::Input::KeyCode::X;


/// <summary>
/// デフォルトコンストラクタ
/// </summary>
Player::Player() :
	::Ishikawa::DirectX::Device::DependentResource::Base(),
	::CatchAFairy::Object::UpdateInterface(),
	::CatchAFairy::Object::RenderInterface(),
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
	normalTexture(),
	playCamera(nullptr),
	scenePlay(nullptr),
	operationMethodView(nullptr),
	catchTime(0.0f),
	pointLight(nullptr),
	manaLightGenerationCountAdditionalRatio(Player::MINIMUM_MANA_LIGHT_GENERATE_COUNT_ADDITIONAL_RATIO),
	manaLightGenerationCount(0.0f),
	manaLightGenerationColor(Player::DEFAULT_MANA_LIGHT_COLOR),
	catchedFairyCount{0U}
{
	// 自作オブジェクトデータ読み込み
	this->LoadItObjectData();
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player(){
	// 自作オブジェクトデータ開放
	this->ReleaseItObjectData();
}


/// <summary>
/// トポロジータイプの取得
/// </summary>
/// <returns>トポロジータイプ</returns>
::CatchAFairy::Shader::User::VertexTopology Player::GetTopology() const {
	return this->topology;
}

/// <summary>
/// 頂点バッファの取得
/// </summary>
/// <returns>頂点バッファ</returns>
ID3D11Buffer* Player::GetVertexBuffer() const {
	return this->vertexBuffer;
}

/// <summary>
/// インデックスバッファの取得
/// </summary>
/// <returns>インデックスバッファ</returns>
ID3D11Buffer* Player::GetIndexBuffer() const {
	return this->indexBuffer;
}

/// <summary>
/// インデックス数の取得
/// </summary>
/// <returns>インデックス数</returns>
unsigned int Player::GetIndexCount() const {
	return this->indexCount;
}

/// <summary>
/// 外接円中心座標の取得
/// </summary>
/// <returns>外接円中心座標</returns>
const ::Ishikawa::Math::Float3& Player::GetCircumscribedCircleCenterPosition() const {
	return this->circumscribedCircleCenterPosition;
}

/// <summary>
/// 外接円半径二乗の取得
/// </summary>
/// <returns>外接円半径二乗</returns>
float Player::GetSquareCircumscribedCircleRadius() const {
	return this->squareCircumscribedCircleRadius;
}

/// <summary>
/// 拡散反射シェーダリソースビューの取得
/// </summary>
/// <returns>拡散反射シェーダリソースビュー</returns>
ID3D11ShaderResourceView* Player::GetDiffuseShaderResourceView() const {
	return this->diffuseTexture.GetShaderResourceView();
}

/// <summary>
/// 法線シェーダリソースビューの取得
/// </summary>
/// <returns>法線シェーダリソースビュー</returns>
ID3D11ShaderResourceView* Player::GetNormalShaderResourceView() const {
	return this->normalTexture.GetShaderResourceView();
}

/// <summary>
/// ワールド変換行列の取得
/// </summary>
/// <returns>ワールド変換行列</returns>
const ::Ishikawa::Math::Matrix& Player::GetWorld() const {
	return this->world;
}

/// <summary>
/// キーボードの取得
/// </summary>
/// <returns>キーボード</returns>
const ::Ishikawa::Input::Keyboard& Player::GetKeyboard() const {
	if(this->scenePlay == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("scenePlay is null.");
	}

	::CatchAFairy::Scene::Manager* const sceneManager = this->scenePlay->GetSceneManager();
	if(sceneManager == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("sceneManager is null.");
	}

	::CatchAFairy::Game* const game = sceneManager->GetGame();
	if(game == nullptr){
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("game is null.");
	}

	return game->GetKeyboard();
}


/// <summary>
/// プレイカメラの設定
/// </summary>
/// <param name="_playCamera">プレイカメラ</param>
void Player::SetPlayCamera(PlayCamera* const _playCamera){
	this->playCamera = _playCamera;

	if(this->playCamera != nullptr){
		this->playCamera->OnMoveFollowingTheObject(this->world);
	}
}

/// <summary>
/// ポイントライトの設定
/// </summary>
/// <param name="_pointLight">ポイントライト</param>
void Player::SetPointLight(PointLight* const _pointLight){
	this->pointLight = _pointLight;
	if(this->pointLight != nullptr){
		this->pointLight->SetFairyBodyLightPosition(
			Player::FAIRY_ID,
			this->world._41 + Player::FAIRY_LIGHT_ANCHOR.x,
			this->world._42 + Player::FAIRY_LIGHT_ANCHOR.y,
			this->world._43 + Player::FAIRY_LIGHT_ANCHOR.z
		);
		this->pointLight->SetFairyBodyLightColor(
			Player::FAIRY_ID,
			Player::FAIRY_LIGHT_COLOR.x,
			Player::FAIRY_LIGHT_COLOR.y,
			Player::FAIRY_LIGHT_COLOR.z,
			Player::FAIRY_LIGHT_COLOR.w
		);
		this->pointLight->SetFairyBodyLightRadius(Player::FAIRY_ID, Player::FAIRY_LIGHT_RADIUS);
		this->pointLight->SetFairyBodyLightLumen(Player::FAIRY_ID, Player::FAIRY_LIGHT_LUMEN);

		this->manaLightGenerationColor = Player::DEFAULT_MANA_LIGHT_COLOR;
		this->pointLight->SetFairyManaLightColor(
			Player::FAIRY_ID,
			Player::DEFAULT_MANA_LIGHT_COLOR.x,
			Player::DEFAULT_MANA_LIGHT_COLOR.y,
			Player::DEFAULT_MANA_LIGHT_COLOR.z,
			Player::DEFAULT_MANA_LIGHT_COLOR.w
		);
		this->pointLight->SetFairyManaLightRadius(Player::FAIRY_ID, Player::MANA_LIGHT_RADIUS);
		this->pointLight->SetFairyManaLightLumen(Player::FAIRY_ID, Player::MANA_LIGHT_LUMEN);
	}
}

/// <summary>
/// ワールド変換行列の設定
/// </summary>
/// <param name="_world">ワールド変換行列</param>
void Player::SetWorld(const ::Ishikawa::Math::Matrix& _world){
	this->world = _world;

	if(this->playCamera != nullptr){
		this->playCamera->OnMoveFollowingTheObject(this->world);
	}
}


/// <summary>
/// デバイスに読み込まれた
/// </summary>
void Player::OnDeviceLoaded(){
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
void Player::OnDeviceRestored(){
	// 何もしない
}

/// <summary>
/// デバイスが破棄された
/// </summary>
void Player::OnDeviceLost(){
	// 何もしない
}


/// <summary>
/// 妖精を捕まえた
/// </summary>
/// <param name="_catchedFairyType">捕まえた妖精の種類</param>
void Player::OnCaughtAFairy(CatchAFairy::Data::EnemyFairyType _catchedFairyType){
	if(_catchedFairyType == CatchAFairy::Data::EnemyFairyType::Count){
		return;
	}

	// 捕まえた種類の捕獲数を増やす
	++(this->catchedFairyCount[static_cast<unsigned int>(_catchedFairyType)]);

	// マナライト生成量を増やす
	this->manaLightGenerationCountAdditionalRatio =
		Player::MAXIMUM_MANA_LIGHT_GENERATE_COUNT_ADDITIONAL_RATIO;
	// マナライトの色を捕まえた妖精に対応する色に近づける
	this->manaLightGenerationColor =
		(this->manaLightGenerationColor * 0.3f) +
		(Player::CATCHED_FAIRY_MANA_LIGHT_COLOR[static_cast<unsigned int>(_catchedFairyType)] * 0.7f);
}


/// <summary>
/// 初期化
/// </summary>
void Player::Initialize(){
	// 基底の初期化
	this->InitializeITDeviceDependentResource();

	// メンバの初期化
	this->diffuseTexture.Initialize();
	this->normalTexture.Initialize();

	// 初期位置設定
	this->world._41 = START_POSITION.x;
	this->world._42 = START_POSITION.y;
	this->world._43 = START_POSITION.z;
}


/// <summary>
/// 更新
/// </summary>
/// <param name="_elapsedTime">更新間隔</param>
void Player::Update(float _elapsedTime){
	this->Move(_elapsedTime);
	this->Rotate(_elapsedTime);

	if(this->catchTime > 0.0f){
		this->catchTime -= _elapsedTime;
	}

	this->Catch();

	this->UpdatePointLight(_elapsedTime);
}


/// <summary>
/// 頂点バッファとインデックスバッファ作成
/// </summary>
/// <param name="_useShader">利用するシェーダ</param>
void Player::CreateVertexBufferAndIndexBuffer(
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
void Player::ReleaseVertexBufferAndIndexBuffer(){
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
void Player::LoadItObjectData(){
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
void Player::ReleaseItObjectData(){
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


/// <summary>
/// 移動
/// </summary>
/// <param name="_elapsedTime">更新間隔</param>
void Player::Move(float _elapsedTime){
	const ::Ishikawa::Input::Keyboard& keyboard = this->GetKeyboard();

	float deltaMoveDistance = _elapsedTime * MOVE_SPEED;
	int moveDirectionNumber = 13;
	if(keyboard.IsPressing(FRONT_MOVE_KEY)){
		// 前
		moveDirectionNumber += 3;
	}
//	if(keyboard.IsPressing(BACK_MOVE_KEY)){
//		// 後ろ
//		moveDirectionNumber -= 3;
//	}
//	if(keyboard.IsPressing(LEFT_MOVE_KEY)){
//		// 左
//		moveDirectionNumber -= 1;
//	}
//	if(keyboard.IsPressing(RIGHT_MOVE_KEY)){
//		// 右
//		moveDirectionNumber += 1;
//	}
	if(keyboard.IsPressing(UP_MOVE_KEY)){
		// 上
		moveDirectionNumber += 9;
	}
	if(keyboard.IsPressing(DOWN_MOVE_KEY)){
		// 下
		moveDirectionNumber -= 9;
	}

	if(moveDirectionNumber != 13){
		int deltaX = moveDirectionNumber%3 - 1;
		int deltaY = moveDirectionNumber/9 - 1;
		int deltaZ = (moveDirectionNumber%9)/3 - 1;

		::Ishikawa::Math::Float3 velocity = ::Ishikawa::Math::Float3::ZERO;
		velocity.x = deltaMoveDistance * deltaX;
		velocity.y = deltaMoveDistance * deltaY;
		velocity.z = deltaMoveDistance * deltaZ;

		int check = deltaX*deltaX + deltaY*deltaY + deltaZ* deltaZ;

		// 斜め移動の場合は速度を合わせる
		switch(check){
		case 2:
			velocity *= 0.7071067812f;
		case 3:
			velocity *= 0.5773502692f;
		}

		// ステージとの判定で移動ベクトルを調整
		velocity = this->scenePlay->CollideMoveVectorAtStage(
			::Ishikawa::Math::Float3::ZERO,
			velocity,
			this->world,
			0.001f
		);

		::Ishikawa::Math::Matrix moveMatrix = ::Ishikawa::Math::Matrix::IDENTITY;
		moveMatrix._41 = velocity.x;  moveMatrix._42 = velocity.y;  moveMatrix._43 = velocity.z;
		this->SetWorld(moveMatrix * this->world);

		if(this->operationMethodView != nullptr){
			this->operationMethodView->OnActPlayer();
		}
	}
}

/// <summary>
/// 回転
/// </summary>
/// <param name="_elapsedTime">更新間隔</param>
void Player::Rotate(float _elapsedTime){
	const ::Ishikawa::Input::Keyboard& keyboard = this->GetKeyboard();

	float deltaRotateRadian = _elapsedTime * ROTATE_SPEED;
	int rotateNumber = 0;
	if(keyboard.IsPressing(LEFT_ROTATE_KEY)){
		rotateNumber += 1;
	}
	if(keyboard.IsPressing(RIGHT_ROTATE_KEY)){
		rotateNumber -= 1;
	}

	if(rotateNumber != 0){
		::Ishikawa::Math::Matrix rotateMatrix = ::Ishikawa::Math::CreateRotationMatrixOfAxisY(rotateNumber * deltaRotateRadian);
		this->SetWorld(rotateMatrix * this->world);

		if(this->operationMethodView != nullptr){
			this->operationMethodView->OnActPlayer();
		}
	}
}

/// <summary>
/// 捕まえる
/// </summary>
void Player::Catch(){
	const ::Ishikawa::Input::Keyboard& keyboard = this->GetKeyboard();

	if( keyboard.IsPressed(CATCH_KEY) ||
		(keyboard.IsPressing(CATCH_KEY) && (this->catchTime <= 0.0f))
	){
		this->catchTime = CATCH_INTERVAL;

		this->TryCatchingFairies();

		if(this->operationMethodView != nullptr){
			this->operationMethodView->OnActPlayer();
		}
	}
}


/// <summary>
/// ポイントライトの更新
/// </summary>
/// <param name="_elapsedTime">更新間隔</param>
void Player::UpdatePointLight(float _elapsedTime){
	if(this->pointLight == nullptr) return;

	// 本体ライトの移動
	this->pointLight->SetFairyBodyLightPosition(
		Player::FAIRY_ID,
		this->world._41 + Player::FAIRY_LIGHT_ANCHOR.x,
		this->world._42 + Player::FAIRY_LIGHT_ANCHOR.y,
		this->world._43 + Player::FAIRY_LIGHT_ANCHOR.z
	);

	this->manaLightGenerationCount += _elapsedTime * this->manaLightGenerationCountAdditionalRatio;
	while(this->manaLightGenerationCount >= 1.0f){
		this->manaLightGenerationCount -= 1.0f;

		// マナライト生成カウント増分比率を徐々に減らす
		this->manaLightGenerationCountAdditionalRatio =
			(0.9f * this->manaLightGenerationCountAdditionalRatio) +
			(0.1f * Player::MINIMUM_MANA_LIGHT_GENERATE_COUNT_ADDITIONAL_RATIO);

		// 徐々に基本色に近づける
		this->manaLightGenerationColor =
			(this->manaLightGenerationColor * 0.9f) +
			(Player::DEFAULT_MANA_LIGHT_COLOR * 0.1f);
		this->pointLight->SetFairyManaLightColor(
			Player::FAIRY_ID,
			this->manaLightGenerationColor.x,
			this->manaLightGenerationColor.y,
			this->manaLightGenerationColor.z,
			this->manaLightGenerationColor.w
		);

		// 新規マナライト生成
		this->pointLight->GenerateFairyManaLight(
			Player::FAIRY_ID,
			this->world._41 + Player::FAIRY_LIGHT_ANCHOR.x,
			this->world._42 + Player::FAIRY_LIGHT_ANCHOR.y,
			this->world._43 + Player::FAIRY_LIGHT_ANCHOR.z
		);
	}
}

/// <summary>
/// 妖精を捕まえようとする
/// </summary>
void Player::TryCatchingFairies(){
	if(this->scenePlay != nullptr){
		// 腕を振る音
		this->scenePlay->OnPlaySoundOfKaraburi();

		::CatchAFairy::Object::PlayScene::EnemyFairies& enemyFairies = this->scenePlay->GetEnemyFairies();
		const unsigned int ENEMY_FAIRY_COUNT = enemyFairies.GetWorldCount();
		const ::Ishikawa::Math::Matrix* const ENEMY_FAIRY_WORLD = enemyFairies.GetWorldArray();
		for(unsigned int i=0U; i<ENEMY_FAIRY_COUNT; ++i){
			// 妖精のワールド座標
			const ::Ishikawa::Math::Float4& fairyWorldPosition = ENEMY_FAIRY_WORLD[i].row4;
			// プレイヤから見た妖精の座標
			::Ishikawa::Math::Float4 fairyPositionAtPlayer = fairyWorldPosition * this->world.Invert();

			// 高さ的に捕まえられるかどうか
			if((fairyPositionAtPlayer.y < FAIRY_CATCHABLE_HEIGHT_MIN)
				|| (fairyPositionAtPlayer.y > FAIRY_CATCHABLE_HEIGHT_MAX)
				){
				// 届かない
				continue;
			}
			// 距離的に捕まえられるかどうか
			::Ishikawa::Math::Float2 distanceOfPlayerLooksFairy(fairyPositionAtPlayer.x, fairyPositionAtPlayer.z);
			float lengthSquaredOfPlayerToFairy = distanceOfPlayerLooksFairy.LengthSquared();
			if(lengthSquaredOfPlayerToFairy < (FAIRY_CATCHABLE_RADIUS * FAIRY_CATCHABLE_RADIUS)){
				// 確保音
				this->scenePlay->OnPlaySoundOfCatched();
				// 確保を通知
				this->OnCaughtAFairy(enemyFairies.GetFairyType(i));
				enemyFairies.OnCatched(i);
				continue;
			}
			if(lengthSquaredOfPlayerToFairy > (FAIRY_CATCHABLE_ANGLE_RADIUS * FAIRY_CATCHABLE_ANGLE_RADIUS)){
				// 届かない
				continue;
			}
			// 角度的に捕まえられるかどうか
			::Ishikawa::Math::Float2 directionOfPlayerLooksFairy;
			distanceOfPlayerLooksFairy.Normalize(&directionOfPlayerLooksFairy);
			if(directionOfPlayerLooksFairy.y < FAIRY_CATCHABLE_ANGLE_COS) continue;


			// すべての条件を通過したので捕まえられる

			// 確保音
			this->scenePlay->OnPlaySoundOfCatched();
			// 確保を通知
			this->OnCaughtAFairy(enemyFairies.GetFairyType(i));
			enemyFairies.OnCatched(i);
		}
	}
}
