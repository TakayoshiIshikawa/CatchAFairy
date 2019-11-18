//=============================================================================
//	ITKeyboard.h
//
//	����L�[�{�[�h�̃w�b�_�t�@�C��
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __IT_KEYBOARD_H__
#define __IT_KEYBOARD_H__

namespace Ishikawa{
	namespace Input{
		/// <summary>�L�[�R�[�h</summary>
		enum class KeyCode : unsigned long{
			/// <summary>�G�X�P�[�v</summary>
			Escape,
			/// <summary>�G���^�[</summary>
			Enter,
			/// <summary>�X�y�[�X</summary>
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
			/// <summary>�V�t�g</summary>
			Shift,
			/// <summary>�����</summary>
			LeftArrow,
			/// <summary>�E���</summary>
			RightArrow,
			/// <summary>����</summary>
			UpArrow,
			/// <summary>�����</summary>
			DownArrow,

			/// <summary>�P�̃L�[�̐�</summary>
			SingleKeyCount,

			// ----- ��������͕����L�[ -----

			/// <summary>�ϊ��ł��Ȃ���������</summary>
			None = 0xffffffff
		};

		/// <summary>
		/// ����L�[�{�[�h
		/// </summary>
		class Keyboard final{
		private:
			/// <summary>
			/// �L�[���
			/// </summary>
			class KeyState final{
			private:
				/// <summary>�X�e�[�g�̌^</summary>
				typedef unsigned long StateType;


			private:
				static const unsigned int STATE_TYPE_BIT_COUNT = 8U * sizeof(StateType);
				static const unsigned int STATE_ARRAY_SIZE = (static_cast<StateType>(KeyCode::SingleKeyCount) / STATE_TYPE_BIT_COUNT) + 1U;
				static_assert(STATE_ARRAY_SIZE > 0U, "Key state array size error.\n");

			private:
				// �e�L�[�̏�Ԃ��i�[����z��
				StateType state[STATE_ARRAY_SIZE];


			public:
				// �L�[�����Ԃ��擾���邽�߂̃C���f�b�N�X�ƃ}�X�N�f�[�^�ɕϊ�
				static void ConvertKeyToKeyIndexAndMask(
					unsigned int* const _index,
					StateType* const _mask,
					KeyCode _key
				);


			public:
				// �f�t�H���g�R���X�g���N�^
				KeyState();
				// �f�X�g���N�^
				~KeyState();

			public:
				// ������Z�q
				KeyState& operator=(const KeyState& _original);

			public:
				// �L�[��Ԃ̎擾
				bool GetState(KeyCode _key) const;

			public:
				// �L�[�������ꂽ
				void Pressed(KeyCode _key);
				// �L�[�������ꂽ
				void Released(KeyCode _key);
			};


		private:
			/// <summary>�O��̏��</summary>
			KeyState beforeState;
			/// <summary>���݂̏��</summary>
			KeyState currentState;


		private:
			// W�p�����[�^����L�[�R�[�h�ɕϊ�
			static Keyboard::KeyCode ConvertWPalamToKeyCode(unsigned int _wParam);


		public:
			// �f�t�H���g�R���X�g���N�^
			Keyboard();
			// �f�X�g���N�^
			~Keyboard();

		public:
			// �L�[��������Ă��邩?
			bool IsPressing(KeyCode _key) const;
			// �L�[��������Ă��邩?
			bool IsReleasing(KeyCode _key) const;
			// �L�[�������ꂽ��?
			bool IsPressed(KeyCode _key) const;
			// �L�[�������ꂽ��?
			bool IsReleased(KeyCode _key) const;

		public:
			// ��Ԃ��ύX����
			void OnChangedState(unsigned int _message, unsigned int _wParam, long _lParam);

		public:
			// �X�V
			void Update();
		};
	}
}

#endif
