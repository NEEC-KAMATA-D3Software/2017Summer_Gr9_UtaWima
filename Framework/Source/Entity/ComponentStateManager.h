#pragma once
#include<unordered_map>
#include<Source\Component\Component.h>
#include<memory>


/**
* @file	ComponentStateManager.h
* @brief	�R���|�[�l���g�̏�Ԃ��L������N���X
* @authro	���{�D�P
* @date	2017/02/23
*/

namespace framework {

	enum class ComponentState
	{
		deActive = 0,
		active = 1,
		notFind,
	};

	class ComponentStateManager
	{
	public:
		ComponentStateManager();
		~ComponentStateManager();

		/**
		* @brief			��Ԃ�ێ�������R���|�[�l���g�o�^
		* @param component  ��Ԃ�ێ�������R���|�[�l���g�̎�Q��
		*/
		void registComponent(std::weak_ptr<Component> component);

		/**
		* @brief			��Ԃ�ێ��������R���|�[�l���g�j���Ə�Ԏ擾
		* @param component  ��Ԃ�ێ��������R���|�[�l���g�̎�Q��
		* @return			���
		*/
		ComponentState popComponentState(std::weak_ptr<Component> component);

		void clear();

	private:
		//�R���|�[�l���g��ԃL���[
		std::unordered_map<Component*,bool> m_ComponentMemory;
	};


}