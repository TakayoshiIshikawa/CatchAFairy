//=============================================================================
//	ITWindowClass.h
//
//	自作ウィンドウクラスのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_WINDOW_CLASS_H__
#define __IT_WINDOW_CLASS_H__

struct HINSTANCE__;
struct HWND__;
struct HICON__;
struct HBRUSH__;


namespace Ishikawa{
	namespace Window{
		/// <summary>
		/// 自作ウィンドウクラス
		/// </summary>
		class WindowClass final{
		public:
			typedef long (__stdcall *ProcedureType)(HWND__*, unsigned int, unsigned int, long);


		private:
			/// <summary>名前</summary>
			const wchar_t* name;

		public:
			// デフォルトコンストラクタ
			WindowClass();
			// デストラクタ
			~WindowClass();

		public:
			/// <summary>名前の取得</summary>
			/// <returns>名前</returns>
			const wchar_t* GetName() const { return this->name; }

		public:
			// 作成
			void Create(
				const wchar_t* const _name,
				const wchar_t* const _menuName,
				const unsigned long _style,
				const long _classExtra,
				const long _windowExtra,
				HINSTANCE__* const _hInstance,
				HICON__* const _icon,
				HICON__* const _smallIcon,
				HICON__* const _cursor,
				HBRUSH__* const _backgroundBrush,
				ProcedureType _procedure
			);
		};
	}
}

#endif
