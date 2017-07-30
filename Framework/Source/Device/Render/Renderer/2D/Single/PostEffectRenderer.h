#pragma once
#include"ISpriteRenderer.h"
#include<Source\Util\Type.h>
#include<xnamath.h>

namespace framework {

	//struct MyStruct
	//{
	//	XMMATRIX world;
	//	util::Vec2 pixSize;
	//};

	//struct DefaultBuffer {
	//	XMMATRIX world;
	//	util::Vec2 pixSize;
	//	util::Vec2 damy;
	//};

	struct PostSometime {
		XMMATRIX projection;
	};

	//Buffer 構造体の中には必ずworldが必要
	template<typename Buffer>
	class PostEffectRenderer : public ISpriteRenderer<Buffer, PostSometime>
	{
	public:
		PostEffectRenderer(const std::string& materialName)
			:ISpriteRenderer(materialName)
		{}

		~PostEffectRenderer() {}

		void setBuffer(Buffer buffer) {
			this->m_Buffer = buffer;
		}


	private:
		virtual void setPipeline()override {
	
			m_Buffer.world = m_WoldMat.toXMMatrix();
			util::updateConstantBuffer<Buffer>(m_pAlwaysBuffer.p, m_Buffer);

			PostSometime sb;
			sb.projection = m_Projection.toXMMatrix();

			util::updateConstantBuffer<PostSometime>(m_pSometimeBuffer.p, sb);

		};

	private:
		Buffer m_Buffer;


	};

}