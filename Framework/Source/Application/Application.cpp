#include"Application.h"
#include<assert.h>
#include<Source\Resource\ResourceManager.h>
#include<Source\Util\Timer\ProcessTimer.h>
#include<Source\Util\Win\WinFunc.h>
#include<Source\Util\CSharp\SharpAPI.h>
#include<Source\Util\AntTweak\AntWeak.h>
#include<Source\Util\IO\OggLoader.h>
#include<Source\Util\Win\WinFunc.h>
#include<Source\Device\Sound\SoundPlayer.h>



namespace framework {

	bool Application::m_Exit = false;

	Application::Application(HINSTANCE instance)
		:m_hInstance(instance)
	{
		//util::OggLoader loader("Resource/Sound/test.ogg");
		util::SharpAPI::initAPI();
		//util::AntWeak::apiInit(TW_DIRECT3D11);
	}

	Application::~Application()
	{
		util::SharpAPI::endAPI();
		util::AntWeak::apiEnd();
	}

	void Application::init()
	{
		HWND hWind = CreateWindow(TEXT("STATIC"), TEXT("test"),
			WS_CAPTION,
			100, 100, 200, 200, NULL, NULL, m_hInstance, NULL);

		assert(hWind != NULL && "ウィンドウ生成失敗");

		ShowWindow(hWind, SW_SHOW);
	}

	int Application::run()
	{
		//!メッセージ取得よう変数
		static MSG msg;

		std::shared_ptr<SoundPlayer> player = std::make_shared<SoundPlayer>();
		ResourceManager::getInstance()->setSoundDevice(player);

		init();

		player->init(m_hWnd);


		while (messageProcess(&msg) && !m_Exit)
		{
			//util::ProcessTimer timer;
#ifdef _MDEBUG
			//timer.begin();
#endif
			m_FpsTimer.update();
			update();
			draw();

			auto waiteTime = m_FpsTimer.waiteTime();
			if (waiteTime > 0) {
				Sleep(waiteTime);
			}

#ifdef _MDEBUG
			//timer.end();
			util::WinFunc::OutLog("1ループあたり : " + std::to_string(m_FpsTimer.getFPS()));
#endif
		}

		return (int)msg.wParam;
	}

	bool Application::messageProcess(MSG* msg)
	{
		if (PeekMessage(msg, NULL, 0, 0, PM_NOREMOVE)) {
			if (!GetMessage(msg, NULL, 0, 0)) {
				return false;
			}
			TranslateMessage(msg);//メッセージ変換
			DispatchMessage(msg);//メッセージ送信
			return true;
		}

		//メッセージが来ないときは常にtrueを返す
		return true;
	}

	void Application::importMaterial()
	{
		//このシェーダーは必ず読み込まれている必要があるのでハードコーディング
		//ポストエフェクト用は別途で読み込み可能
		loadMaterial();
	}



	void loadMaterial()
	{
		ResourceManager::getInstance()->importStandardMaterial("TestMaterial", "Resource/Script/Shader/BaseShader.hlsl");
		ResourceManager::getInstance()->importStandardMaterial("SpriteMaterial", "Resource/Script/Shader/SpriteShader.hlsl");
		ResourceManager::getInstance()->importStandardMaterial("RigidInstance", "Resource/Script/Shader/RigidInstance.hlsl");
		ResourceManager::getInstance()->importGeometryMaterial("SkinningInstance", "Resource/Script/Shader/SkinningInstance.hlsl");
		//ResourceManager::getInstance()->importStandardMaterial("SkinningInstance", "Resource/Script/Shader/SkinningInstance.hlsl");
		ResourceManager::getInstance()->importGeometryMaterial("RigidInstanceEnvironment", "Resource/Script/Shader/RigidInstanceEnvironment.hlsl");
		ResourceManager::getInstance()->importStandardMaterial("DeferredLighting", "Resource/Script/Shader/DeferredLighting.hlsl");
		ResourceManager::getInstance()->importStandardMaterial("BoxInstance", "Resource/Script/Shader/BoxInstance.hlsl");
		ResourceManager::getInstance()->importStandardMaterial("Bloom", "Resource/Script/Shader/Bloom.hlsl");
		ResourceManager::getInstance()->importStandardMaterial("Gradation", "Resource/Script/Shader/Gradation.hlsl");
		ResourceManager::getInstance()->importStandardMaterial("Blur", "Resource/Script/Shader/Blur.hlsl");		
	}

}