#pragma once

#include<D3DX11.h>
#include<Source\Util\Type.h>
#include<Source\Util\Render\DXFunc.h>
#include<Source\Util\WrapFunc.h>
#include<vector>
#include<unordered_map>
#include<string>
#include<Source\Resource\Texture\BoneTexture.h>
#include<memory>

/**
* @file	    Motion.h
* @brief	���[�V�����\����
* @detail   Motion�͓����{�[���������Ă��Ȃ��Ƃ����Ȃ�
* @authro	���{�D�P
* @date	2017/05/04
*/

namespace framework {

	class BoneTexture;
	
	class Motion
	{
	public:
		Motion();
		Motion(const std::string& filePath);
		~Motion();
		/**
		* @brief		 ���ׂẴ��[�V�������폜����
		*/
		void clear();

		std::vector<util::Mat4> getFrameMotion(int frame);

		std::vector<util::Mat4> getFrameMotionWithName(const std::string& name, int frame);

		int getMaxFrame();

	private:
		//!�p���s��
		std::vector<std::vector<std::pair<std::string,util::Mat4>>> m_MotionMat;
		int endFrame;
	};

}