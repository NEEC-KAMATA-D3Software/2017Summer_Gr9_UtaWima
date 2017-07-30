#pragma once
#include<Framework.h>
#include<list>

namespace component {

	class SceneChangeComponent;
	class SpriteRenderComponent;

	class RulesComponent : public framework::UpdateComponent
	{
	public:
		RulesComponent();
		~RulesComponent();

		void init()override;

		// UpdateComponent ����Čp������܂���
		virtual void update() override;


		void clear()override;
		
	private:
		//�v���[���̐����P�ɂȂ������ǂ���
		bool isPlayerOnes();

		bool isPlayerZero();

		void checkDelete();

	private:
		std::list<framework::WeakEntity> m_PlayerContainer;
		std::weak_ptr<SceneChangeComponent> m_pSceneChanger;
		util::Timer m_Timer;
		util::Timer m_SpriteTimer;
		framework::WeakEntity m_pSpriteEntity;
		std::weak_ptr<SpriteRenderComponent> m_pSprite;

	};



}