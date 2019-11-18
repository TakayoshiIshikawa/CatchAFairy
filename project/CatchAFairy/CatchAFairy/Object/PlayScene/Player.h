//=============================================================================
//	Player.h
//
//	プレイヤのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "CatchAFairy/Object/UpdateObjectInterface.h"
#include "CatchAFairy/Object/RenderObjectInterface.h"
#include "Ishikawa/DirectX/Device/DependentResource/ITDeviceDependentResourceBase.h"
#include "Ishikawa/Math/ITFloatN.h"
#include "Ishikawa/Math/ITMatrix.h"
#include "Ishikawa/DirectX/Texture/ITFileTexture.h"
#include "CatchAFairy/Data/EnemyFairyType.h"

namespace Ishikawa{
	namespace Model{
		namespace ItObject{
			struct VertexData;
		}
	}
	namespace Input{
		class Keyboard;
		enum class KeyCode : unsigned long;
	}
}

namespace CatchAFairy{
	namespace Shader{
		namespace User{
			class PlayCameraEyeFirst;
		}
	}
	namespace Scene{
		class Play;
	}

	namespace Object{
		namespace PlayScene{
			class PlayCamera;
			class PointLight;
			namespace GUI{
				class OperationMethodView;
			}

			/// <summary>
			/// プレイヤ
			/// </summary>
			class Player final :
				public ::Ishikawa::DirectX::Device::DependentResource::Base,
				public ::CatchAFairy::Object::UpdateInterface,
				public ::CatchAFairy::Object::RenderInterface
			{
			private:
				// 自作モデルデータファイルパス
				static const char* const IT_MODEL_DATA_FILEPATH;
				// 拡散反射テクスチャファイルパス
				static const wchar_t* const DIFFUSE_TEXTURE_FILEPATH;
				// 法線テクスチャファイルパス
				static const wchar_t* const NORMAL_TEXTURE_FILEPATH;
				// 妖精ID
				static const unsigned int FAIRY_ID;
				// 本体ライトのずれ
				static const ::Ishikawa::Math::Float3 FAIRY_LIGHT_ANCHOR;
				// 本体ライトの色
				static const ::Ishikawa::Math::Float4 FAIRY_LIGHT_COLOR;
				// 本体ライトの半径
				static const float FAIRY_LIGHT_RADIUS;
				// 本体ライトの光量
				static const float FAIRY_LIGHT_LUMEN;
				// 基本のマナライトの色
				static const ::Ishikawa::Math::Float4 DEFAULT_MANA_LIGHT_COLOR;
				// 各妖精を捕まえたときのマナライトの色
				static const ::Ishikawa::Math::Float4 CATCHED_FAIRY_MANA_LIGHT_COLOR[static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)];
				// マナライトの半径
				static const float MANA_LIGHT_RADIUS;
				// マナライトの光量
				static const float MANA_LIGHT_LUMEN;
				// 最小のマナライト生成カウント増分比率
				static const float MINIMUM_MANA_LIGHT_GENERATE_COUNT_ADDITIONAL_RATIO;
				// 最大のマナライト生成カウント増分比率
				static const float MAXIMUM_MANA_LIGHT_GENERATE_COUNT_ADDITIONAL_RATIO;
				// 開始位置
				static const ::Ishikawa::Math::Float3 START_POSITION;
				// 移動速度
				static const float MOVE_SPEED;
				// 回転速度
				static const float ROTATE_SPEED;
				// 捕獲間隔
				static const float CATCH_INTERVAL;

				// 妖精を捕まえられる最低高さ
				static const float FAIRY_CATCHABLE_HEIGHT_MIN;
				// 妖精を捕まえられる最高高さ
				static const float FAIRY_CATCHABLE_HEIGHT_MAX;
				// 妖精を捕まえられる半径
				static const float FAIRY_CATCHABLE_RADIUS;
				// 妖精を捕まえられる扇形半径
				static const float FAIRY_CATCHABLE_ANGLE_RADIUS;
				// 妖精を捕まえられる角度のコサイン値
				static const float FAIRY_CATCHABLE_ANGLE_COS;

				// 前進キー
				static const ::Ishikawa::Input::KeyCode FRONT_MOVE_KEY;
				// 後進キー
				static const ::Ishikawa::Input::KeyCode BACK_MOVE_KEY;
				// 右進キー
				static const ::Ishikawa::Input::KeyCode RIGHT_MOVE_KEY;
				// 左進キー
				static const ::Ishikawa::Input::KeyCode LEFT_MOVE_KEY;
				// 上進キー
				static const ::Ishikawa::Input::KeyCode UP_MOVE_KEY;
				// 下進キー
				static const ::Ishikawa::Input::KeyCode DOWN_MOVE_KEY;
				// 右旋回キー
				static const ::Ishikawa::Input::KeyCode RIGHT_ROTATE_KEY;
				// 左旋回キー
				static const ::Ishikawa::Input::KeyCode LEFT_ROTATE_KEY;
				// 捕獲キー
				static const ::Ishikawa::Input::KeyCode CATCH_KEY;

			private:
				/// <summary>利用しているシェーダ</summary>
				const ::CatchAFairy::Shader::User::PlayCameraEyeFirst* useShader;
				/// <summary>自作オブジェクト頂点配列</summary>
				::Ishikawa::Model::ItObject::VertexData* itObjectVertices;
				/// <summary>自作オブジェクト頂点数</summary>
				unsigned int itObjectVertexCount;
				/// <summary>自作オブジェクトインデックス配列</summary>
				unsigned long* itObjectIndices;
				/// <summary>自作オブジェクトインデックス数</summary>
				unsigned int itObjectIndexCount;
				/// <summary>トポロジータイプ</summary>
				::CatchAFairy::Shader::User::VertexTopology topology;
				/// <summary>頂点バッファ</summary>
				ID3D11Buffer* vertexBuffer;
				/// <summary>インデックスバッファ</summary>
				ID3D11Buffer* indexBuffer;
				/// <summary>インデックス数</summary>
				unsigned int indexCount;
				/// <summary>外接円中心座標</summary>
				::Ishikawa::Math::Float3 circumscribedCircleCenterPosition;
				/// <summary>外接円半径二乗</summary>
				float squareCircumscribedCircleRadius;
				/// <summary>ワールド行列</summary>
				::Ishikawa::Math::Matrix world;
				/// <summary>拡散反射テクスチャ</summary>
				::Ishikawa::DirectX::Texture::FileTexture diffuseTexture;
				/// <summary>法線テクスチャ</summary>
				::Ishikawa::DirectX::Texture::FileTexture normalTexture;
				/// <summary>プレイカメラ</summary>
				PlayCamera* playCamera;
				/// <summary>プレイシーン</summary>
				::CatchAFairy::Scene::Play* scenePlay;
				/// <summary>操作方法表示</summary>
				GUI::OperationMethodView* operationMethodView;
				/// <summary>捕獲時間</summary>
				float catchTime;
				/// <summary>ポイントライト</summary>
				PointLight* pointLight;
				/// <summary>マナライト生成カウント増分比率</summary>
				float manaLightGenerationCountAdditionalRatio;
				/// <summary>マナライト生成カウント</summary>
				float manaLightGenerationCount;
				/// <summary>マナライト生成色</summary>
				::Ishikawa::Math::Float4 manaLightGenerationColor;
				/// <summary>種類ごとの捕獲妖精数</summary>
				unsigned int catchedFairyCount[static_cast<unsigned int>(CatchAFairy::Data::EnemyFairyType::Count)];


			public:
				// デフォルトコンストラクタ
				Player();
				// デストラクタ
				virtual ~Player() override final;

			public:
				// トポロジータイプの取得
				virtual ::CatchAFairy::Shader::User::VertexTopology GetTopology() const override final;
				// 頂点バッファの取得
				virtual ID3D11Buffer* GetVertexBuffer() const override final;
				// インデックスバッファの取得
				virtual ID3D11Buffer* GetIndexBuffer() const override final;
				// インデックス数の取得
				virtual unsigned int GetIndexCount() const override final;
				// 外接円中心座標の取得
				virtual const ::Ishikawa::Math::Float3& GetCircumscribedCircleCenterPosition() const override final;
				// 外接円半径二乗の取得
				virtual float GetSquareCircumscribedCircleRadius() const override final;
				// 拡散反射シェーダリソースビューの取得
				virtual ID3D11ShaderResourceView* GetDiffuseShaderResourceView() const override final;
				// 法線シェーダリソースビューの取得
				virtual ID3D11ShaderResourceView* GetNormalShaderResourceView() const override final;
				// ワールド変換行列の取得
				virtual const ::Ishikawa::Math::Matrix& GetWorld() const override final;

				/// <summary>妖精捕獲数の取得</summary>
				/// <param name="_type">妖精の種類</param>
				/// <returns>捕獲数</returns>
				unsigned int GetCatchedFairyCount(CatchAFairy::Data::EnemyFairyType _type) const {
					if(_type == CatchAFairy::Data::EnemyFairyType::Count) return 0U;
					return this->catchedFairyCount[static_cast<unsigned int>(_type)];
				}

			private:
				// キーボードの取得
				const ::Ishikawa::Input::Keyboard& GetKeyboard() const;

			public:
				// プレイカメラの設定
				void SetPlayCamera(PlayCamera* const _playCamera);
				//　ポイントライトの設定
				void SetPointLight(PointLight* const _pointLight);
				/// <summary>プレイシーンの設定</summary>
				/// <param name="_scenePlay">プレイシーン</param>
				void SetScenePlay(::CatchAFairy::Scene::Play* const _scenePlay){
					this->scenePlay = _scenePlay;
				}
				/// <summary>操作方法表示の設定</summary>
				/// <param name="_operationMethodView">操作方法表示</param>
				void SetOperationMethodView(GUI::OperationMethodView* const _operationMethodView){
					this->operationMethodView = _operationMethodView;
				}

			private:
				// ワールド変換行列の設定
				void SetWorld(const ::Ishikawa::Math::Matrix& _world);

			public:
				// デバイスに読み込まれた
				virtual void OnDeviceLoaded() override final;
				// デバイスが復元された
				virtual void OnDeviceRestored() override final;
				// デバイスが破棄された
				virtual void OnDeviceLost() override final;

			private:
				// 妖精を捕まえた
				void OnCaughtAFairy(CatchAFairy::Data::EnemyFairyType _catchedFairyType);

			public:
				// 初期化
				void Initialize();
				// 更新
				virtual void Update(float _elapsedTime) override final;

				// 頂点バッファとインデックスバッファ作成
				void CreateVertexBufferAndIndexBuffer(
					const ::CatchAFairy::Shader::User::PlayCameraEyeFirst* const _useShader
				);
				// 頂点バッファとインデックスバッファ開放
				void ReleaseVertexBufferAndIndexBuffer();

			private:
				// 自作オブジェクトデータ読み込み
				void LoadItObjectData();
				// 自作オブジェクトデータ開放
				void ReleaseItObjectData();

				// 移動
				void Move(float _elapsedTime);
				// 回転
				void Rotate(float _elapsedTime);
				// 捕まえる
				void Catch();

				// ポイントライトの更新
				void UpdatePointLight(float _elapsedTime);

				// 妖精を捕まえようとする
				void TryCatchingFairies();
			};
		}
	}
}

#endif
