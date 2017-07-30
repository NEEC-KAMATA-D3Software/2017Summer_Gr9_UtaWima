#pragma once
#include<Source\Component\Component.h>
#include<string>
#include<memory>
#include<Source\Component\Animation\AnimatorComponent.h>

/**
* @file	RigidInstanceRenderComponent.h
* @brief	�C���X�^���X�`��I�[�i�[�R���|�[�l���g�N���X
* @dital	���̃R���|�[�l���g���������Ă���I�u�W�F�N�g�̓C���X�^���X�`�悳���
* @authro	���{�D�P
* @date	2017/03/13
*/

namespace framework {
	class CubeRenderTrget;
	class CubeDepthTarget;
}

namespace component {

	class RenderOwnerComponent;

	class RenderClientComponent:public framework::Component
	{
	public:
		RenderClientComponent();
		~RenderClientComponent();

		const std::string& getModelName();

		/**
		* @brief		������
		* @Detail		���ׂẴR���|�[�l���g����ĂɌĂ΂��
		*/
		virtual void init()override;

		/** 
		* @brief			�p�����[�^�Z�b�g
		* @param param		������p�����[�^
		*/
		void setParam(const std::vector<std::string>& param)override;

		/**
		* @brief			���̃N���C�A���g�̃L���[�u�}�b�v�̏������݂��J�n����
		*/
		void cubMapWriteBegin();

		/**
		* @brief			���̃N���C�A���g�̃L���[�u�}�b�v�̏������݂��I��
		*/
		void cubMapWriteEnd();

		std::shared_ptr<framework::CubeRenderTrget> getCubeMap();

		/**
		* @brief			���̃N���C�A���g�̃L���[�u�}�b�v�̏������݂��I��
		*/
		std::weak_ptr<AnimatorComponent> getAnimator();

	private:
		std::weak_ptr<component::RenderOwnerComponent> findModelOwner();

	private:
		std::shared_ptr<framework::CubeRenderTrget> m_pCubeTarget;
		std::shared_ptr<framework::CubeDepthTarget> m_pCubeDepthTarget;
		std::weak_ptr<AnimatorComponent> m_pAnimator;

		//!�`�悷�郂�f���̖��O
		std::string m_ModelName;
	};



}