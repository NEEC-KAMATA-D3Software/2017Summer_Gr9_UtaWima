#pragma once
#include"../Type.h"
#include<memory>
#include<Source\Util\Math\Math.h>

namespace util {


	struct Transform
	{
	public:
		/**
		* @brief	 	 �����t���R���X�g���N�^
		* @dital		 �X�P�[���ȊO�̒l��0�ŏ���������� �X�P�[����1
		*/
		Transform();
		/**
		* @brief	 	 �����t���R���X�g���N�^
		* @pram	position ���W
		* @pram	angle    ����
		* @pram	scale    �傫��
		* @dital		 angle��Y���ɂȂ��Ă���
		*/
		Transform(util::Vec3 position,float angle,Vec3 scale);

		/**
		* @brief	 	 �����t���R���X�g���N�^
		* @pram	position ���W
		* @pram	angle    ����
		* @pram	scale    �傫��
		*/
		Transform(util::Vec3 position, util::Vec3 angle, Vec3 scale);


		~Transform();

		/**
		* @brief	 	 �R�s�[�R���X�g���N�^
		*/
		Transform(Transform& other);

		///**
		//* @brief		�N�I�[�^�j�I��
		//* @pram	angle	�N�I�[�^�j�I����]�l
		//*/
		//void rotate(Quaternion& angle);

		/**
		* @brief		��]�l�擾
		*/
		float getAngle();

		/**
		* @brief		�X�P�[���̕ύX
		* @dital		���ׂē����l�ŕύX�����
		*/
		void scaling(float scale);

		/**
		* @brief		�����o�ϐ�����}�g���N�X���쐬
		* @dital		�������d���̂Œ��ӂ��K�v��������Ȃ�
		*/
		util::Mat4 toMatrix();

		/**
		* @brief		��]�}�g���N�X���쐬
		* @dital		toMatrix()�Ə������d�����Ă���̂őΉ����K�v��������Ȃ�
		*/
		util::Mat4 toRotateMatrix();

		/**
		* @brief		�w�肵���ΏۂɌ�������
		* @param dir    ��������������
		*/
		void lookAt(util::Vec3& target,util::Vec3& up);

		/**
		* @brief		�w�肵���ΏۂɌ�������
		* @param dir    ��������������
		*/
		void lookAt(util::Vec3& vec);

		/**
		* @brief		Z�����擾
		*/
		util::Vec3 front();

		/**
		* @brief		Y�����擾
		*/
		util::Vec3 up();

		/**
		* @brief		X�����擾
		*/
		util::Vec3 left();

	private:
		/**
		* @brief		xyz�̊e����]�l����Z����
		* @pram	angle	�e���̃A���O��
		*/
		XMVECTOR rotate(const util::Vec3& angle);

		/**
		* @brief		����]
		* @pram	axis	��
		* @pram	angle   �p�x(�f�B�O���[)
		*/
		XMVECTOR rotate(const util::Vec3& axis, Degree angle);

	public:
		util::Vec3 m_Position;
		util::Vec3 m_Scale;
		util::Vec3 m_Rotation;
		//!�N�I�[�^�j�I��
		//Quaternion m_Rotation;
	};


}