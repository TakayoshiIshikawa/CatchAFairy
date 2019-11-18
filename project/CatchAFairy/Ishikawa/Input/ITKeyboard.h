//=============================================================================
//	ITKeyboard.h
//
//	自作キーボードのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_KEYBOARD_H__
#define __IT_KEYBOARD_H__

namespace Ishikawa{
	namespace Input{
		/// <summary>キーコード</summary>
		enum class KeyCode : unsigned long{
			/// <summary>エスケープ</summary>
			Escape,
			/// <summary>エンター</summary>
			Enter,
			/// <summary>スペース</summary>
			Space,
			/// <summary>'Z'</summary>
			Z,
			/// <summary>'X'</summary>
			X,
			/// <summary>'C'</summary>
			C,
			/// <summary>'A'</summary>
			A,
			/// <summary>'S'</summary>
			S,
			/// <summary>'D'</summary>
			D,
			/// <summary>'Q'</summary>
			Q,
			/// <summary>'W'</summary>
			W,
			/// <summary>'E'</summary>
			E,
			/// <summary>シフト</summary>
			Shift,
			/// <summary>左矢印</summary>
			LeftArrow,
			/// <summary>右矢印</summary>
			RightArrow,
			/// <summary>上矢印</summary>
			UpArrow,
			/// <summary>下矢印</summary>
			DownArrow,

			/// <summary>単体キーの数</summary>
			SingleKeyCount,

			// ----- ここからは複合キー -----

			/// <summary>変換できなかったもの</summary>
			None = 0xffffffff
		};

		/// <summary>
		/// 自作キーボード
		/// </summary>
		class Keyboard final{
		private:
			/// <summary>
			/// キー状態
			/// </summary>
			class KeyState final{
			private:
				/// <summary>ステートの型</summary>
				typedef unsigned long StateType;


			private:
				static const unsigned int STATE_TYPE_BIT_COUNT = 8U * sizeof(StateType);
				static const unsigned int STATE_ARRAY_SIZE = (static_cast<StateType>(KeyCode::SingleKeyCount) / STATE_TYPE_BIT_COUNT) + 1U;
				static_assert(STATE_ARRAY_SIZE > 0U, "Key state array size error.\n");

			private:
				// 各キーの状態を格納する配列
				StateType state[STATE_ARRAY_SIZE];


			public:
				// キーから状態を取得するためのインデックスとマスクデータに変換
				static void ConvertKeyToKeyIndexAndMask(
					unsigned int* const _index,
					StateType* const _mask,
					KeyCode _key
				);


			public:
				// デフォルトコンストラクタ
				KeyState();
				// デストラクタ
				~KeyState();

			public:
				// 代入演算子
				KeyState& operator=(const KeyState& _original);

			public:
				// キー状態の取得
				bool GetState(KeyCode _key) const;

			public:
				// キーが押された
				void Pressed(KeyCode _key);
				// キーが離された
				void Released(KeyCode _key);
			};


		private:
			/// <summary>前回の状態</summary>
			KeyState beforeState;
			/// <summary>現在の状態</summary>
			KeyState currentState;


		private:
			// Wパラメータからキーコードに変換
			static Keyboard::KeyCode ConvertWPalamToKeyCode(unsigned int _wParam);


		public:
			// デフォルトコンストラクタ
			Keyboard();
			// デストラクタ
			~Keyboard();

		public:
			// キーが押されているか?
			bool IsPressing(KeyCode _key) const;
			// キーが離されているか?
			bool IsReleasing(KeyCode _key) const;
			// キーが押されたか?
			bool IsPressed(KeyCode _key) const;
			// キーが離されたか?
			bool IsReleased(KeyCode _key) const;

		public:
			// 状態が変更した
			void OnChangedState(unsigned int _message, unsigned int _wParam, long _lParam);

		public:
			// 更新
			void Update();
		};
	}
}

#endif
