#pragma once
#include<Source\Component\UpdateComponent.h>
#include<string>
#include<unordered_map>
#include<memory>
#include<Source\Util\Template\Event.h>
#include<functional>
#include<Source\Util\Timer\Timer.h>

namespace framework {
	class Motion;

	struct AnimData
	{
		std::string name;
		//!�Đ����x
		int frameSpeed;
		//!�A�j���[�V�����̃C�x���g
		util::Event<std::string> animEvent;
		//!���[�v���邩�ǂ���
		bool isLoop;
		//!�Ō�̃t���[��
		int endFrame;
		//!�A�j���[�V�����̔ԍ�
		int id;
	};
}

namespace component {

	class AnimatorComponent : public framework::UpdateComponent
	{
	public:

		struct LoadData
		{
			std::string name;
			bool isLoop;
			int speed;
		};

		class EmitEvent
		{
		public:
			EmitEvent(std::weak_ptr<framework::Entity> entity, const std::string& animationName,const std::string& eventName,int emitFrame,int liveFrame );
			~EmitEvent();

			/**
			* @brief		�C�x���g����
			* @param currentAnime	���݂̃A�j���[�V�����̖��O
			* @param currentFrame	���݂̃t���[����
			*/
			void sourchEvent(const framework::AnimData& currentAnime, int currentFrame);

		private:
			/**
			* @brief		�C�x���g�̔��s�������ǂ���
			*/
			bool isFrameCondition(int currentFrame);

			/**
			* @brief		�C�x���g���s
			*/
			void emitEvent();

		private:
			//!�C�x���g�������Ă���G���e�B�e�B
			std::weak_ptr<framework::Entity> m_Entity;
			//!���s�t���[��
			const int m_EmitFrame;
			//!�����t���[����
			const int m_LiveFrame;
			//!�o�^����Ă��閼�O
			const std::string m_EventName;
			//!�o�^����Ă��閼�O
			const std::string m_AnimationName;
		};

		AnimatorComponent();
		~AnimatorComponent();

		/**
		* @brief		������
		*/
		void init();

		/**
		* @brief			�A�j���[�V�����o�^
		* @param stateName  ��Ԃ̖��O
		* @param data		�A�j���[�V�����̏��
		*/
		void addAnime(const std::string& stateName, framework::AnimData data);

		// UpdateComponent ����Čp������܂���
		virtual void update() override;

		/**
		* @brief		�N���A�[
		*/
		void clear();

		/**
		* @brief			�A�j���[�V�����������I�ɕύX����
		* @param stateName  �ύX����o�^������Ԃ̖��O
		*/
		void changeAnime(const std::string& stateName);


		/**
		* @brief		�p�����[�^���Z�b�g
		* @param param  ������p�����[�^
		*/
		virtual void setParam(const std::vector<std::string>& param);

		const int getFrame();

		const int getCurrentMotionID();

		bool isEnd();

		/**
		* @brief		���݂̃t���[������0~1�Ŏ擾
		*/
		float rate();

		/**
		* @brief		id�̏��ԂɃ\�[�g���ꂽ���[�V�����̖��O�擾
		*/
		std::list<std::string> getMotionNames();

	private:

		/**
		* @brief		�A�j���[�V�����̃t���[�����X�V����
		*/
		void updateMotion();

		void registData(const LoadData& data);

		/**
		* @brief		�C�x���g�o�^
		*/
		void registEvent(const std::vector<std::string> param);

		/**
		* @brief		�C�x���g�X�V
		*/
		void updateEvent();

	private:
		//�A�j���[�V�����̐�
		int m_AnimeCount;

		//!�A�j���[�V�����R���e�i
		std::unordered_map<std::string,framework::AnimData> m_AnimeDatas;

		//!���݂̃A�j���[�V����
		framework::AnimData m_CurrentAnime;

		//!���݂̃t���[��
		int m_CurrentFrame;

		//!���݂̃X�e�[�g�̖��O
		std::string m_CurrentName;

		std::vector<LoadData> m_AddList;

		//!�ŏ��̃��[�V�����̖��O
		std::string m_FirstName;

		//!�C�x���g���s�N���X�R���e�i
		std::vector<EmitEvent> m_EmitterContainer;
	};




}