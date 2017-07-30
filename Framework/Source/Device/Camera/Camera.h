#pragma once
#include<Source\Util\Type.h>

namespace framework {

	class Camera
	{
	public:
		Camera();
		Camera(util::Vec3 eyePos,util::Vec3 lookPos);
		~Camera();

		void setEyePosition(util::Vec3 eyePos);

		void setLookAt(util::Vec3 lookPos);

		/**
		* @brief					�r���[�s��ɕϊ��y�ю擾
		* @return					�r���[�ϊ��s��
		*/
		util::Mat4 toViewMatrix();

		/**
		* @brief					�r���[�s��ɕϊ��ςݍs��擾
		* @detail					toViewMatrix���Ă΂�Ă��Ȃ���ΌÂ��s��ɂȂ��Ă��܂��̂Œ���
		* @return					�r���[�ϊ��s��
		*/
		util::Mat4 getViewMatrix();

	private:
		//!���_���W
		util::Vec3 m_EyePosition;
		//!�����_���W
		util::Vec3 m_LookPosition;
		//!�r���[�ϊ��s��
		util::Mat4 m_ViewMatrix;
	};

}