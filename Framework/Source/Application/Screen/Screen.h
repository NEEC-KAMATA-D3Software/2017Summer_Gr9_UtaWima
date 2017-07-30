#pragma once
/**
* @file	    Screen.h
* @brief	�E�B���h�E�̃T�C�Y
* @dital	�t���[�����[�N�ł̉摜�T�C�Y�擾�p
* @authro	���{�D�P
* @date	2017/3/19
*/



namespace framework {
	struct Screen
	{
		//!�E�B���h�E�̕�
		static int WINDOW_WIDTH;
		//!�E�B���h�E�̍���
		static int WINDOW_HEIGHT;
		//!�E�B���h�E�̕��̔���
		static int WINDOW_WIDTH_HALF;
		//!�E�B���h�E�̍����̔���
		static int WINDOW_HEIGHT_HALF;
		//!����p
		static float PERSPECTIVE;
		//!�`�拗��
		static float FAR_;

		//!UI�̕\���̈�̕�
		static int UI_WIDTH;
		//!UI�̕\���̈�̕�
		static int UI_HEIGHT;


		//!�𑜓x�̕�
		static int PIXEL_WIDTH;
		//!�𑜓x�̍���
		static int PIXEL_HEIGHT;
		//!�𑜓x�̂̕��̔���
		static int PIXEL_WIDTH_HALF;
		//!�𑜓x�̍����̔���
		static int PIXEL_HEIGHT_HALF;
	};
}