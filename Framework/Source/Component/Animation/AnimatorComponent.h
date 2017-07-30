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
		//!再生速度
		int frameSpeed;
		//!アニメーションのイベント
		util::Event<std::string> animEvent;
		//!ループするかどうか
		bool isLoop;
		//!最後のフレーム
		int endFrame;
		//!アニメーションの番号
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
			* @brief		イベント検索
			* @param currentAnime	現在のアニメーションの名前
			* @param currentFrame	現在のフレーム数
			*/
			void sourchEvent(const framework::AnimData& currentAnime, int currentFrame);

		private:
			/**
			* @brief		イベントの発行条件かどうか
			*/
			bool isFrameCondition(int currentFrame);

			/**
			* @brief		イベント発行
			*/
			void emitEvent();

		private:
			//!イベントを持っているエンティティ
			std::weak_ptr<framework::Entity> m_Entity;
			//!発行フレーム
			const int m_EmitFrame;
			//!生存フレーム数
			const int m_LiveFrame;
			//!登録されている名前
			const std::string m_EventName;
			//!登録されている名前
			const std::string m_AnimationName;
		};

		AnimatorComponent();
		~AnimatorComponent();

		/**
		* @brief		初期化
		*/
		void init();

		/**
		* @brief			アニメーション登録
		* @param stateName  状態の名前
		* @param data		アニメーションの情報
		*/
		void addAnime(const std::string& stateName, framework::AnimData data);

		// UpdateComponent を介して継承されました
		virtual void update() override;

		/**
		* @brief		クリアー
		*/
		void clear();

		/**
		* @brief			アニメーションを強制的に変更する
		* @param stateName  変更する登録した状態の名前
		*/
		void changeAnime(const std::string& stateName);


		/**
		* @brief		パラメータをセット
		* @param param  文字列パラメータ
		*/
		virtual void setParam(const std::vector<std::string>& param);

		const int getFrame();

		const int getCurrentMotionID();

		bool isEnd();

		/**
		* @brief		現在のフレーム数を0~1で取得
		*/
		float rate();

		/**
		* @brief		idの順番にソートされたモーションの名前取得
		*/
		std::list<std::string> getMotionNames();

	private:

		/**
		* @brief		アニメーションのフレームを更新する
		*/
		void updateMotion();

		void registData(const LoadData& data);

		/**
		* @brief		イベント登録
		*/
		void registEvent(const std::vector<std::string> param);

		/**
		* @brief		イベント更新
		*/
		void updateEvent();

	private:
		//アニメーションの数
		int m_AnimeCount;

		//!アニメーションコンテナ
		std::unordered_map<std::string,framework::AnimData> m_AnimeDatas;

		//!現在のアニメーション
		framework::AnimData m_CurrentAnime;

		//!現在のフレーム
		int m_CurrentFrame;

		//!現在のステートの名前
		std::string m_CurrentName;

		std::vector<LoadData> m_AddList;

		//!最初のモーションの名前
		std::string m_FirstName;

		//!イベント発行クラスコンテナ
		std::vector<EmitEvent> m_EmitterContainer;
	};




}