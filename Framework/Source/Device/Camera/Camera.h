#pragma once
#include<Source\Util\Type.h>

namespace framework {

	class Camera
	{
	public:
		Camera();
		Camera(util::Vec3 eyePos,util::Vec3 lookPos);
		~Camera();

		void setEyePosition(util::Vec3 eyePos);

		void setLookAt(util::Vec3 lookPos);

		/**
		* @brief					ビュー行列に変換及び取得
		* @return					ビュー変換行列
		*/
		util::Mat4 toViewMatrix();

		/**
		* @brief					ビュー行列に変換済み行列取得
		* @detail					toViewMatrixが呼ばれていなければ古い行列になってしまうので注意
		* @return					ビュー変換行列
		*/
		util::Mat4 getViewMatrix();

	private:
		//!視点座標
		util::Vec3 m_EyePosition;
		//!注視点座標
		util::Vec3 m_LookPosition;
		//!ビュー変換行列
		util::Mat4 m_ViewMatrix;
	};

}