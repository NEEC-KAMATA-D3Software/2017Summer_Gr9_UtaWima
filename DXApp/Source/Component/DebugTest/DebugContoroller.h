#pragma once
#include<Source\Component\UpdateComponent.h>
#include<Source\Component\Animation\AnimatorComponent.h>


/**
* @file	DebugContoroller.h
* @brief	ディレクショナルライトを操作するコンポーネント
* @authro	高須優輝
* @date	2017/04/14
*/


namespace component {

	class DebugContoroller : public framework::UpdateComponent
	{
	public:
		DebugContoroller();
		~DebugContoroller();

		void init()override;

		// UpdateComponent を介して継承されました
		virtual void update() override;

	private:
		std::weak_ptr<AnimatorComponent> m_Animator;

	};

}