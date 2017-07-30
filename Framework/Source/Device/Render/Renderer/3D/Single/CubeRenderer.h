#pragma once
#include"IRenderer.h"


namespace framework {


	class CubeRenderer : public IRenderer
	{
	public:
		CubeRenderer();
		~CubeRenderer();


		// IRenderer を介して継承されました
		virtual void draw(util::Transform * drawTransList) override;

	private:


	};



}