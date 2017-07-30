#pragma once

#include<Windows.h>
#include<Framework\Source\Application\Application.h>
#include<memory>
#include<Framework\Source\Device\Input\GamePad.h>
#include<Framework\Source\Application\Scene\Scene.h>

namespace framework {
	class RenderTarget;
	class UpdateTask;
	class RenderTask;
}

class TestApp : public framework::Application
{
public:
	TestApp(HINSTANCE hInst);
	~TestApp();




private:

	void init()override;
	// Application ÇâÓÇµÇƒåpè≥Ç≥ÇÍÇ‹ÇµÇΩ
	virtual void update() override;

	virtual void draw() override;

private:
	float m_ClearColor[4];
	std::unique_ptr<framework::RenderTarget> m_pTarget;

	std::shared_ptr<framework::UpdateTask> m_pUpdateTask;
	std::shared_ptr<framework::RenderTask> m_pRenderTask;

	framework::GamePad pad;

	framework::Scene scene;
};

