//=============================================================================
//	ITCriAdx2Le.h
//
//	自作CRI_ADX2_Le利用のヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#ifndef __IT_CRI_ADX2_LE_H__
#define __IT_CRI_ADX2_LE_H__

#include "Ishikawa/Common/PatternBase/SingletonBase.h"

namespace Ishikawa{
	namespace CriAdx2Le{
		/// <summary>
		/// 自作CRI_ADX2_Le利用
		/// </summary>
		class ITCriAdx2Le final : public ::Ishikawa::Common::SingletonBase{
		public:
			// GetInstance<ITCriAdx2Le>()でアクセスするためフレンド化
			friend class Ishikawa::Common::SingletonManager;


		private:
			// Pimplを利用して隠蔽
			class PimplOfITCriAdx2Le;


		private:
			// シングルトンインスタンス
			static ITCriAdx2Le* instance;

		private:
			/// <summary>隠蔽データ</summary>
			PimplOfITCriAdx2Le* itCriAdx2Le;


		private:
			// デフォルトコンストラクタ
			ITCriAdx2Le();
			// デストラクタ
			virtual ~ITCriAdx2Le() override final;
		private:
			/// <summary>コピーコンストラクタ[削除]</summary>
			ITCriAdx2Le(const ITCriAdx2Le&) = delete;
			/// <summary>代入演算子[削除]</summary>
			ITCriAdx2Le& operator=(const ITCriAdx2Le&) = delete;

		public:
			// ボリューム設定
			void SetVolume(float _volume);

		public:
			// 初期化
			void Initialize(const char* const _acfFilepath, const char* const _acbFilepath, const char* const _awbFilepath);
			// 更新
			void Update();
			// 終了
			void Finalize();

			// acbファイル変更
			void ChangeAcbFile(const char* const _acbFilepath, const char* const _awbFilepath);

			// 再生
			unsigned int Play(int _number);
			// 停止
			void Stop(unsigned int _id);
		};
	}
}

#endif
