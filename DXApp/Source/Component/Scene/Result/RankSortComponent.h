#pragma once
#include<Framework.h>
#include<unordered_map>
#include<string>

namespace component {

	class FadeComponent;

	class RankSortComponent : public framework::UpdateComponent
	{
	public:
		RankSortComponent();
		~RankSortComponent();

		void init()override;

		/**
		* @brief						��������������Ă���Entity�ɃR���|�[�l���g��ǉ�����
		* @param 	componentInitalizer	����vector�Ɋi�[���ꂽ�R���|�[�l���g�͈�Ăɏ����������
		*/
		virtual void componentSet(std::vector<std::weak_ptr<Component>>* componentInitalizer)override;

		// UpdateComponent ����Čp������܂���
		virtual void update() override;

	private:
		class RankConverter
		{
		public:
			RankConverter() {
				m_RegistData[0] = "1PUI";
				m_RegistData[1] = "2PUI";
				m_RegistData[2] = "3PUI";
				m_RegistData[3] = "4PUI";
			}

			std::string get(int id) {
				return m_RegistData[id];
			}
		private:
			//playerID : �e�N�X�`����
			std::unordered_map<int, std::string> m_RegistData;

		};

		//�����L���O���ɃR���|�[�l���g��ڒ�����
		void connectComponent(std::vector<std::weak_ptr<Component>>* componentInitalizer);

		void firstAction();


	private:
		std::vector<framework::WeakEntity> m_RankList;
		util::Timer m_Timer;
		const float m_LastPos;
		const float m_MediumPos;
		const float m_FirstPos;
		std::weak_ptr<FadeComponent> m_pFade;
		bool m_IsFirst;

	};



}