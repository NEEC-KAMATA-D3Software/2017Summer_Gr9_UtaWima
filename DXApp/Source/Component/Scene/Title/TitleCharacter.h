#pragma once
#include<Framework.h>
#include<Source\Component\Animation\AnimatorComponent.h>
#include<memory>

namespace component {

	class TitleCharacter : public framework::Component
	{
	public:
		TitleCharacter() {}
		~TitleCharacter() {}

		void init()override {
			auto animator = m_Entity.lock()->getComponent<AnimatorComponent>();
			animator.lock()->changeAnime(m_FirstName);
		}

		/**
		* @brief		�p�����[�^���Z�b�g
		* @param param  ������p�����[�^
		*/
		virtual void setParam(const std::vector<std::string>& param)override {
			m_FirstName = param[0];
		}

	private:
		std::string m_FirstName;
	};
}