#pragma once

#include<Source\Component\Component.h>
#include<functional>
#include<memory>
#include<Source\Util\Type.h>

/**
* @file	DirectionalLightComponent.h
* @brief	�G���e�B�e�B�[���f�B���N�V���i�����C�g�ɂ���R���|�[�l���g
* @authro	���{�D�P
* @date	2017/04/12
*/

namespace framework {
	class MSAAMultiRenderTarget;
}

namespace component {

	class CameraComponent;

	class DirectionalLightComponent : public framework::Component
	{
	public:
		DirectionalLightComponent();
		~DirectionalLightComponent();


		/**
		* @brief		���̃R���|�[���g���A�N�e�B�u�ɂ���
		*/
		virtual void active()override;

		/**
		* @brief		���̃R���|�[���g���f�B�A�N�e�B�u�ɂ���
		*/
		virtual void deActive()override;

		/**
		* @brief						��������������Ă���Entity�ɃR���|�[�l���g��ǉ�����
		* @param 	componentInitalizer	����vector�Ɋi�[���ꂽ�R���|�[�l���g�͈�Ăɏ����������
		*/
		virtual void componentSet(std::vector<std::weak_ptr<Component>>* componentInitalizer) override;

		/**
		* @brief		���C�g�̃|�W�V�������擾
		*/
		static util::Vec3 getLightPosition();

		/**
		* @brief		���C�g�̃J���[�擾
		*/
		static util::Vec3 getLightColor();

		/**
		* @brief		���C�g����̃J�������擾
		*/
		static std::weak_ptr<CameraComponent> getCamera();

		/**
		* @brief		���C�g����̃J�������擾
		*/
		//static util::Transform* getTransform();

		

		/**
		* @brief		�p�����[�^���Z�b�g
		* @param param  ������p�����[�^
		*/
		virtual void setParam(const std::vector<std::string>& param);
	private:

		/**
		* @brief		DirectionalLightComponent�������Ă���G���e�B�e�B�[�擾
		*/
		static void haveDirectionalLightEntityLoop(std::function<void(framework::WeakEntity)> entityAction);

		/**
		* @brief		�����̐F�擾�p
		*/
		util::Vec3 getMColor();
	private:
		//!�P�������݂��Ȃ����C�g
		static std::weak_ptr<DirectionalLightComponent> m_DerectionalLight;

		std::weak_ptr<CameraComponent> m_pCamera;

		util::Vec3 m_Color;
	};




}
