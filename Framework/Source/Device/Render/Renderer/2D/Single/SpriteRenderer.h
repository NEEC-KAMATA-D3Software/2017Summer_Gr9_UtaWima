#pragma once
#include<Source\Util\Render\DXFunc.h>
#include<atlbase.h>
#include<Source\Util\Type.h>
#include<vector>
#include"ISpriteRenderer.h"

namespace util {
	struct Transform;
}

namespace framework {

	struct AlwaysBuffer
	{
		XMMATRIX world;
		float alpha;
		util::Vec3 damy;
	};

	struct SometimeBuffer {
		XMMATRIX projection;
	};


	class SpriteRenderer : public ISpriteRenderer<AlwaysBuffer, SometimeBuffer>
	{
	public:
		SpriteRenderer();
		~SpriteRenderer();



		void changeMaterial(const std::string& materialName);

		void setAlpha(float alpha);

	private:
		virtual void begin() override;
		virtual void end() override;
		virtual void setPipeline() override;

	private:
		float m_Alpha;

	};




}