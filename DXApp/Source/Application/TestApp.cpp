#include"TestApp.h"
#include<Framework\Source\Application\WindowFactory\WindowFactory.h>
#include<Framework\Source\Util\Type.h>
#include<Framework\Source\Device\Render\DirectXInstance.h>
#include<Source\Application\Screen\Screen.h>
#include<Framework\Source\Util\Render\DXFunc.h>
#include<stdio.h>
#include<resource.h>
#include<Framework\Source\Util\Event\WindowEvent.h>
#include<Framework\Source\Util\Win\WinFunc.h>
#include<Framework\Source\Device\Render\Shader\StandardMaterial.h>
#include<Framework\Source\Task\TaskManager.h>
#include<Framework\Source\Entity\Entity.h>
#include<Framework\Source\Resource\ResourceManager.h>
#include<Framework\Source\Application\Scene\SceneThread.h>
#include<Framework\Source\Application\Screen\Screen.h>
#include<Framework\Source\Util\WrapFunc.h>
#include<Framework\Source\Device\Input\Input.h>
#include<AntTweakBar.h>
#include<Source\Device\Render\Renderer\Effekseer\EffectManager.h>


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {

	if (TwEventWin(hwnd, msg, wp, lp))
		return 0;

	switch (msg)
	{
	case WM_COMMAND:
		util::WindowEvent::getInstance()->onEvent(LOWORD(wp));
		break;
	case WM_DESTROY:        // 閉じるボタンをクリックした時
		PostQuitMessage(0); // WM_QUITメッセージを発行
		break;
	case WM_CLOSE:
#ifdef RELEASE
		int isOK = util::WinFunc::DrawQuestion(hwnd, TEXT(""), TEXT("終了しますか？"));
		if (isOK != IDOK) {
			return 0;
		}
#endif
		break;
	}

	return DefWindowProc(hwnd, msg, wp, lp);
}


TestApp::TestApp(HINSTANCE hInst)
	:Application(hInst)
{
	util::CSVLoader loader("Resource/Confing.csv");

	auto param = loader.load();


	int widht = atoi(param[0][1].c_str());
	int height = atoi(param[0][2].c_str());
	bool isWindowMode = (bool)atoi(param[1][1].c_str());

	framework::Screen::WINDOW_WIDTH = Screen::WINDOW_WIDTH = widht;
	framework::Screen::WINDOW_HEIGHT = Screen::WINDOW_HEIGHT = height;
	framework::Screen::WINDOW_WIDTH_HALF = Screen::WINDOW_WIDTH_HALF = framework::Screen::WINDOW_WIDTH/2;
	framework::Screen::WINDOW_HEIGHT_HALF = Screen::WINDOW_HEIGHT_HALF = framework::Screen::WINDOW_HEIGHT/2;
	framework::Screen::PERSPECTIVE = Screen::PERSPECTIVE;
	framework::Screen::FAR_ = Screen::FAR_;

	framework::Screen::UI_WIDTH = Screen::UI_WIDTH = widht;
	framework::Screen::UI_HEIGHT = Screen::UI_HEIGHT = height;

	framework::Screen::PIXEL_WIDTH = Screen::PIXEL_WIDTH = widht;
	framework::Screen::PIXEL_HEIGHT = Screen::PIXEL_HEIGHT = height;
	framework::Screen::PIXEL_WIDTH_HALF = Screen::PIXEL_WIDTH_HALF = framework::Screen::PIXEL_WIDTH/2;
	framework::Screen::PIXEL_HEIGHT_HALF = Screen::PIXEL_HEIGHT_HALF = framework::Screen::PIXEL_HEIGHT/2;



	//アプリケーションのウィンドウを生成する

	framework::WindowFactory wf(m_hInstance);
	//リリース用ウィンドウクラス作成
	wf.setClassName(TEXT("Release"));
	wf.setProc(WndProc);
	wf.setStyle(CS_HREDRAW | CS_VREDRAW);
	wf.regist();

	//デバッグ用ウィンドウクラス作成
	wf.setClassName(TEXT("Debug"));
	wf.setMenuName(IDR_MENU1);
	wf.regist();

#ifdef _MDEBUG
	m_hWnd = wf.createWnd(TEXT("Debug"), TEXT("App"), util::Vec2(0, 0), util::Vec2(Screen::WINDOW_WIDTH, Screen::WINDOW_HEIGHT));
#else
	m_hWnd = wf.createWnd(TEXT("Release"), TEXT("App"), util::Vec2(100, 100), util::Vec2(Screen::WINDOW_WIDTH, Screen::WINDOW_HEIGHT));
#endif

	ShowWindow(m_hWnd, SW_SHOW);

	// デバイスとスワップ・チェインの作成
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));    // 構造体の値を初期化(必要な場合)
	sd.BufferCount = 1;       // バック・バッファ数
	sd.BufferDesc.Width = Screen::PIXEL_WIDTH;     // バック・バッファの幅
	sd.BufferDesc.Height = Screen::PIXEL_HEIGHT;     // バック・バッファの高さ
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // フォーマット
	sd.BufferDesc.RefreshRate.Numerator = 60;  // リフレッシュ・レート(分子)
	sd.BufferDesc.RefreshRate.Denominator = 1; // リフレッシュ・レート(分母)
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // バック・バッファの使用法
	sd.OutputWindow = m_hWnd;    // 関連付けるウインドウ
	sd.SampleDesc.Count = 1;        // マルチ・サンプルの数
	sd.SampleDesc.Quality = 0;      // マルチ・サンプルのクオリティ
	sd.Windowed = isWindowMode;             // ウインドウ・モード
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // モード自動切り替え

	HRESULT hr = framework::DirectXInstance::getInstance()->create(sd);

	//HRESULT hr = framework::DirectXInstance::getInstance()->createReferenceMode(sd);


	//APIがPCの解像度を超えた値を指定していると自動でそれいかに変更するので最終的なサイズに更新

	//TODO バグの元　しかしレノボでフルＨＤ設定だとバグるので修正必要
	//RECT rc;
	//GetClientRect(m_hWnd, &rc);
	//util::Vec2 size( rc.right - rc.left, rc.bottom - rc.top);



	util::WindowEvent::getInstance()->addEvent(ID_RESET, [&]() {
		scene.init();
	});


	framework::EffectManager::getInstance()->clear();
	framework::EffectManager::getInstance()->create(1000);
	framework::EffectManager::getInstance()->registPath("Resource/Effect/Path.csv");
}

TestApp::~TestApp()
{
}

void TestApp::init()
{
	importMaterial();

	util::CSVLoader loader("Resource/FirstScene.csv");
	auto firstName = loader.load();

	//コメントの行削除
	firstName.erase(firstName.begin());

	scene.setScenePath(firstName[0][0]);
	scene.resourceLoad();

	scene.construction();
	scene.init();

	framework::TaskManager::getInstance()->init();

}

void TestApp::update()
{
	framework::Input::update();
	framework::TaskManager::getInstance()->update();
	framework::TaskManager::getInstance()->physicsUpdate();

}

void TestApp::draw()
{
	framework::TaskManager::getInstance()->draw();
}
