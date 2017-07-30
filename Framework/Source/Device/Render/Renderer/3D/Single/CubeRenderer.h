#pragma once
#include"IRenderer.h"


namespace framework {


	class CubeRenderer : public IRenderer
	{
	public:
		CubeRenderer();
		~CubeRenderer();


		// IRenderer ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
		virtual void draw(util::Transform * drawTransList) override;

	private:


	};



}