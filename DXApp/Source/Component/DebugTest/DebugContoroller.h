#pragma once
#include<Source\Component\UpdateComponent.h>
#include<Source\Component\Animation\AnimatorComponent.h>


/**
* @file	DebugContoroller.h
* @brief	�f�B���N�V���i�����C�g�𑀍삷��R���|�[�l���g
* @authro	���{�D�P
* @date	2017/04/14
*/


namespace component {

	class DebugContoroller : public framework::UpdateComponent
	{
	public:
		DebugContoroller();
		~DebugContoroller();

		void init()override;

		// UpdateComponent ����Čp������܂���
		virtual void update() override;

	private:
		std::weak_ptr<AnimatorComponent> m_Animator;

	};

}