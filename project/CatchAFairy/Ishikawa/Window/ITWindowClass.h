//=============================================================================
//	ITWindowClass.h
//
//	����E�B���h�E�N���X�̃w�b�_�t�@�C��
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
		/// ����E�B���h�E�N���X
		/// </summary>
		class WindowClass final{
		public:
			typedef long (__stdcall *ProcedureType)(HWND__*, unsigned int, unsigned int, long);


		private:
			/// <summary>���O</summary>
			const wchar_t* name;

		public:
			// �f�t�H���g�R���X�g���N�^
			WindowClass();
			// �f�X�g���N�^
			~WindowClass();

		public:
			/// <summary>���O�̎擾</summary>
			/// <returns>���O</returns>
			const wchar_t* GetName() const { return this->name; }

		public:
			// �쐬
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
