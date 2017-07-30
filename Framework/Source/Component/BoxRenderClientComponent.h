#pragma once
#include"Component.h"
#include<Source\Util\Type.h>

namespace component {

	class BoxRenderClientComponent : public framework::UpdateComponent
	{
	public:
		BoxRenderClientComponent();
		~BoxRenderClientComponent();

		void init()override;

		/**
		* @brief		パラメータをセット
		* @param param  文字列パラメータ
		*/
		virtual void setParam(const std::vector<std::string>& param);

		/**
		* @brief		色取得
		*/
		const util::Vec3 getColor();

		void setColor(util::Vec3& color);

		util::Vec3 getOffset();

		// UpdateComponent を介して継承されました
		virtual void update() override;

		util::Transform* getTransform();

	private:
		//!ボックスの色
		util::Vec3 m_Color;
		util::Vec3 m_Offset;
		util::Vec3 m_Scale = util::Vec3(1, 1, 1);
		util::Vec3 m_Angle = util::Vec3(0, 0, 0);


		util::Transform m_Transform;
	};

}