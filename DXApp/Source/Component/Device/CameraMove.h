#pragma once
#include<Framework.h>
#include<list>
#include<memory>

namespace component {

	class PlayerComponent;

	class CameraMove : public framework::UpdateComponent
	{
	public:
		CameraMove();
		~CameraMove();

		/**
		* @brief	初期化
		*/
		void init()override;

		/**
		* @brief	更新
		*/
		virtual void update() override;

		/**
		* @brief		パラメータをセット
		* @param param  文字列パラメータ
		*/
		virtual void setParam(const std::vector<std::string>& param);

	private:
		struct CameraDesc;
		/**
		* @brief	横移動
		*/
		void xMove(const CameraDesc& desc);

		/**
		* @brief	横移動
		*/
		void zMove(const CameraDesc& desc);

		/**
		* @brief	カメラの制御に必要な要素を取得する
		*/
		void getCameraDesc(CameraDesc* result);

		/**
		* @brief	カメラの範囲内からでたプレイヤーをリストから消す
		*/
		void checkDelete();

		/**
		* @brief	カメラを初期位置まで戻す
		*/
		void moveZero();

	private:
		struct CameraDesc
		{
			//!プレーヤー同士の距離
			float playerDistance = 0;
			//!一番右と左のプレイヤーの中間ポジション
			float avePos = 0;
			//!一番右にいるプレイヤーのｘポジション
			float maxPos = 0;
			//!一番左にいるプレイヤーのｘポジション
			float minPos = 0;
		};

		struct ZDesc
		{
			//最大のプレイヤー距離
			float maxDistance = 1000;
			//!最小のプレイヤー距離
			float minDistance = 330;
			//!一番近い位置
			float near_ = 200;
			//!一番遠い位置
			float far_ = 1000;
		};


		//!プレイヤーリスト
		std::list<std::weak_ptr<framework::Entity>> m_PlayerList;
		std::weak_ptr<framework::Entity> m_pCamera;
		std::weak_ptr<framework::Entity> m_pLookEntity;
		//!カメラ移動の摩擦
		float m_Friction = 0.05f;
		//!Z値操作のための構造体
		ZDesc m_ZDesc;
		//!カメラが追跡する境界の値
		float m_Boundary = 5000;
		//!カメラ初期座標
		util::Vec3 m_InitPos;
	};
}