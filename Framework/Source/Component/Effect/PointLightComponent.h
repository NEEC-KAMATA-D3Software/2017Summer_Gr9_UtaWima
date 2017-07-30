#pragma once
#include<Source\Component\UpdateComponent.h>
#include<Source\Util\Type.h>
#include<unordered_map>
#include<utility>
#include<string>

namespace component {

	struct PointLightElement {
		PointLightElement()
			:position(), color(), power(0),
			near_(0), far_(0)
		{
		}

		PointLightElement(util::Vec3 pos, util::Vec3 color_, float power_, float near_, float far_) {
			this->position = pos;
			this->color = color_;
			this->power = power_;
			this->near_ = near_;
			this->far_ = far_;
		}

		PointLightElement(const PointLightElement& other)
			:position(other.position), color(other.color), power(other.power),
			near_(other.near_), far_(other.far_)
		{
		}

		util::Vec3 position;
		util::Vec3 color;
		float power;//!ポイントライトの強さ
		float near_;//!ポイントライトに近いものへの影響度
		float far_;//!ポイントライトに遠いものへの影響度
	};

	using LightList = std::unordered_map<std::string, PointLightElement*>;

	class PointLightComponent : public framework::UpdateComponent
	{
	public:
		PointLightComponent();
		~PointLightComponent();

		/**
		* @brief		アクティブにする
		*/
		virtual void active() override;

		/**
		* @brief		ディアクティブにする
		*/
		virtual void deActive() override;

		/**
		* @brief		パラメータをセット
		* @param param  文字列パラメータ
		*/
		virtual void setParam(const std::vector<std::string>& param);

		///**
		//* @brief		ライトのパワーをセット
		//* @param power  文字列パラメータ
		//*/
		//virtual void setPower(const util::Vec3& power);

		/**
		* @brief		パラメータ取得
		*/
		virtual PointLightElement* getAttrib();

		/**
		* @brief		全体のポイントライトの数取得
		*/
		static const int getTotalLightNum();

		/**
		* @brief		全体のポイントライトの取得
		*/
		static std::shared_ptr<PointLightElement*> getLightArray();

		// UpdateComponent を介して継承されました
		virtual void update() override;

		/**
		* @brief		開放処理
		*/
		virtual void remove()override;

	private:
		//!全体のポイントライト
		static LightList m_PointLightList;
		static std::shared_ptr<PointLightElement*> m_Result;

		//!ポイントライトの要素
		PointLightElement m_LightAttribute;




	};


}