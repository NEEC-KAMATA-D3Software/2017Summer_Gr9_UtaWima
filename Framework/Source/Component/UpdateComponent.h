#pragma once
#include"Component.h"

/**
* @file	UpdateComponent.h
* @brief	�X�V�Ώۂ̃R���|�[�l���g
* @authro	���{�D�P
* @date	2017/02/23
*/

namespace framework {

	class UpdateComponent : public Component
	{
	public:
		UpdateComponent();
		~UpdateComponent();

		/**
		* @brief		������
		*/
		virtual void init();

		virtual void setup() {}

		/**
		* @brief		�X�V
		*/
		virtual void update() = 0;

		/**
		* @brief		�A�N�e�B�u�ɂ���
		*/
		virtual void active() override;

		/**
		* @brief		�f�B�A�N�e�B�u�ɂ���
		*/
		virtual void deActive() override;

	};



}