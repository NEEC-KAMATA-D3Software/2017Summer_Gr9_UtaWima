#pragma once
#include"IRenderer.h"


namespace framework {


	class CubeRenderer : public IRenderer
	{
	public:
		CubeRenderer();
		~CubeRenderer();


		// IRenderer ����Čp������܂���
		virtual void draw(util::Transform * drawTransList) override;

	private:


	};



}