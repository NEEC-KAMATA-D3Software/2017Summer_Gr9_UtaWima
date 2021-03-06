#pragma once

#include"ISpriteRenderer.h"


#define MAX_POINT_LIGHT_NUM 61

namespace framework {

	struct DeferredAlways
	{
		XMMATRIX world;
		util::Vec4 directionalLightPosition;
		util::Vec4 directionalLightColor;
		util::Vec4 lightPos[MAX_POINT_LIGHT_NUM];
		util::Vec4 attribu[MAX_POINT_LIGHT_NUM];
		util::Vec4 pointLightColor[MAX_POINT_LIGHT_NUM];
		int activeLightNum;//有効なライトの数
		util::Vec3 cameraPos;
	};

	struct DeferredSometime {
		XMMATRIX projection;
	};

	class DeferredLightRenderer : public ISpriteRenderer<DeferredAlways, DeferredSometime>
	{
	public:
		DeferredLightRenderer();
		~DeferredLightRenderer();

	private:

		// ISpriteRenderer を介して継承されました
		virtual void begin() override;

		virtual void end() override;

		virtual void setPipeline() override;

	};



}