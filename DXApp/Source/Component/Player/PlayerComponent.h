#pragma once

#include<Source\Component\UpdateComponent.h>
#include<Source\Actor\PlayerStates.h>
#include<memory>
#include<Source\State\State.h>
#include<unordered_map>
#include<assert.h>
#include<Source\Util\Type.h>
#include<Source\Device\Render\Renderer\Effekseer\EffectManager.h>

namespace framework{
	class Entity;
}

namespace component {
	class RigidBody;
}

struct PlayerData
{
	PlayerState currentState;
	std::unordered_map<PlayerState, std::string> motionNames;
	std::vector<std::vector<util::Mat4>> colliderMat;
	//!�R���g���[���̔ԍ�
	int controlID;
	std::weak_ptr<framework::Effect> dethEffect;
	std::weak_ptr<framework::Effect> arrowEffect;
	std::weak_ptr<framework::Effect> rendaEffect;


	std::unordered_map<std::string, PlayerState> convertData
		= {std::make_pair("Idle",PlayerState::idle),
		std::make_pair("Move",PlayerState::move),
		std::make_pair("Attack",PlayerState::attack),
		std::make_pair("Damage",PlayerState::damage),
		std::make_pair("Step",PlayerState::step),
		std::make_pair("Slide",PlayerState::slide), };

	PlayerState convert(const std::string& stateName){
		assert(convertData.find(stateName) != convertData.end() && "�L�[���o�^����Ă��܂���");
		return convertData[stateName];
	}
};

namespace component {

	class RigidBody;
	class CircleCollider;
	class ReadyComponent;

	class PlayerComponent : public framework::UpdateComponent
	{
	public:
		PlayerComponent();
		~PlayerComponent();

		void init()override;

		/**
		* @brief						��������������Ă���Entity�ɃR���|�[�l���g��ǉ�����
		* @param 	componentInitalizer	����vector�Ɋi�[���ꂽ�R���|�[�l���g�͈�Ăɏ����������
		*/
		virtual void componentSet(std::vector<std::weak_ptr<Component>>* componentInitalizer)override;

		const PlayerState& getSate();

		/**
		* @brief		�p�����[�^���Z�b�g
		* @param param  ������p�����[�^
		*/
		virtual void setParam(const std::vector<std::string>& param);


		/**
		* @brief		�X�V
		*/
		virtual void update();

		/**
		* @brief		�v���C���[�f�[�^�擾
		*/
		std::weak_ptr<PlayerData> getPlayerData();

		/**
		* @brief		�Փ˂����Ƃ��̏���
		* @param other	�Փ˂�������I�u�W�F�N�g
		*/
		virtual void onCollisionEnter(const std::weak_ptr<framework::Entity>& other)override;

		/**
		* @brief		�����蔻���L��
		*/
		void activeCollision();

		/**
		* @brief		�����蔻��𖳌�
		*/
		void deActiveCollision();

		bool isEffectEnd();

	private:
		bool isBorer();

		void effectUpdate();

	public:
		static const float BORDER;

	private:
		std::shared_ptr<PlayerData> m_PlayerData;
		std::shared_ptr<State<PlayerComponent>> m_States;
		std::weak_ptr<RigidBody> m_RigidBody;
		std::weak_ptr<CircleCollider> m_CircleCollider;
		framework::WeakEntity m_pAttackComponent;		
		bool isEnd;
		std::weak_ptr<ReadyComponent> m_pReady;
	};

}
