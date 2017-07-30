#pragma once
#include"../Type.h"
#include<memory>
#include<Source\Util\Math\Math.h>

namespace util {


	struct Transform
	{
	public:
		/**
		* @brief	 	 引数付きコンストラクタ
		* @dital		 スケール以外の値が0で初期化される スケールは1
		*/
		Transform();
		/**
		* @brief	 	 引数付きコンストラクタ
		* @pram	position 座標
		* @pram	angle    向き
		* @pram	scale    大きさ
		* @dital		 angleはY軸になっている
		*/
		Transform(util::Vec3 position,float angle,Vec3 scale);

		/**
		* @brief	 	 引数付きコンストラクタ
		* @pram	position 座標
		* @pram	angle    向き
		* @pram	scale    大きさ
		*/
		Transform(util::Vec3 position, util::Vec3 angle, Vec3 scale);


		~Transform();

		/**
		* @brief	 	 コピーコンストラクタ
		*/
		Transform(Transform& other);

		///**
		//* @brief		クオータニオン
		//* @pram	angle	クオータニオン回転値
		//*/
		//void rotate(Quaternion& angle);

		/**
		* @brief		回転値取得
		*/
		float getAngle();

		/**
		* @brief		スケールの変更
		* @dital		すべて同じ値で変更される
		*/
		void scaling(float scale);

		/**
		* @brief		メンバ変数からマトリクスを作成
		* @dital		処理が重いので注意が必要かもしれない
		*/
		util::Mat4 toMatrix();

		/**
		* @brief		回転マトリクスを作成
		* @dital		toMatrix()と処理が重複しているので対応が必要かもしれない
		*/
		util::Mat4 toRotateMatrix();

		/**
		* @brief		指定した対象に向かせる
		* @param dir    向かせたい方向
		*/
		void lookAt(util::Vec3& target,util::Vec3& up);

		/**
		* @brief		指定した対象に向かせる
		* @param dir    向かせたい方向
		*/
		void lookAt(util::Vec3& vec);

		/**
		* @brief		Z方向取得
		*/
		util::Vec3 front();

		/**
		* @brief		Y方向取得
		*/
		util::Vec3 up();

		/**
		* @brief		X方向取得
		*/
		util::Vec3 left();

	private:
		/**
		* @brief		xyzの各軸回転値を乗算する
		* @pram	angle	各軸のアングル
		*/
		XMVECTOR rotate(const util::Vec3& angle);

		/**
		* @brief		軸回転
		* @pram	axis	軸
		* @pram	angle   角度(ディグリー)
		*/
		XMVECTOR rotate(const util::Vec3& axis, Degree angle);

	public:
		util::Vec3 m_Position;
		util::Vec3 m_Scale;
		util::Vec3 m_Rotation;
		//!クオータニオン
		//Quaternion m_Rotation;
	};


}