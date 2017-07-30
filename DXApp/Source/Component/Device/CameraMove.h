#pragma once
#include<Framework.h>
#include<list>
#include<memory>

namespace component {

	class PlayerComponent;

	class CameraMove : public framework::UpdateComponent
	{
	public:
		CameraMove();
		~CameraMove();

		/**
		* @brief	������
		*/
		void init()override;

		/**
		* @brief	�X�V
		*/
		virtual void update() override;

		/**
		* @brief		�p�����[�^���Z�b�g
		* @param param  ������p�����[�^
		*/
		virtual void setParam(const std::vector<std::string>& param);

	private:
		struct CameraDesc;
		/**
		* @brief	���ړ�
		*/
		void xMove(const CameraDesc& desc);

		/**
		* @brief	���ړ�
		*/
		void zMove(const CameraDesc& desc);

		/**
		* @brief	�J�����̐���ɕK�v�ȗv�f���擾����
		*/
		void getCameraDesc(CameraDesc* result);

		/**
		* @brief	�J�����͈͓̔�����ł��v���C���[�����X�g�������
		*/
		void checkDelete();

		/**
		* @brief	�J�����������ʒu�܂Ŗ߂�
		*/
		void moveZero();

	private:
		struct CameraDesc
		{
			//!�v���[���[���m�̋���
			float playerDistance = 0;
			//!��ԉE�ƍ��̃v���C���[�̒��ԃ|�W�V����
			float avePos = 0;
			//!��ԉE�ɂ���v���C���[�̂��|�W�V����
			float maxPos = 0;
			//!��ԍ��ɂ���v���C���[�̂��|�W�V����
			float minPos = 0;
		};

		struct ZDesc
		{
			//�ő�̃v���C���[����
			float maxDistance = 1000;
			//!�ŏ��̃v���C���[����
			float minDistance = 330;
			//!��ԋ߂��ʒu
			float near_ = 200;
			//!��ԉ����ʒu
			float far_ = 1000;
		};


		//!�v���C���[���X�g
		std::list<std::weak_ptr<framework::Entity>> m_PlayerList;
		std::weak_ptr<framework::Entity> m_pCamera;
		std::weak_ptr<framework::Entity> m_pLookEntity;
		//!�J�����ړ��̖��C
		float m_Friction = 0.05f;
		//!Z�l����̂��߂̍\����
		ZDesc m_ZDesc;
		//!�J�������ǐՂ��鋫�E�̒l
		float m_Boundary = 5000;
		//!�J�����������W
		util::Vec3 m_InitPos;
	};
}