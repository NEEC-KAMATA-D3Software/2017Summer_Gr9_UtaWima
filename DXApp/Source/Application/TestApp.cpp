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
	case WM_DESTROY:        // ����{�^�����N���b�N������
		PostQuitMessage(0); // WM_QUIT���b�Z�[�W�𔭍s
		break;
	case WM_CLOSE:
#ifdef RELEASE
		int isOK = util::WinFunc::DrawQuestion(hwnd, TEXT(""), TEXT("�I�����܂����H"));
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



	//�A�v���P�[�V�����̃E�B���h�E�𐶐�����

	framework::WindowFactory wf(m_hInstance);
	//�����[�X�p�E�B���h�E�N���X�쐬
	wf.setClassName(TEXT("Release"));
	wf.setProc(WndProc);
	wf.setStyle(CS_HREDRAW | CS_VREDRAW);
	wf.regist();

	//�f�o�b�O�p�E�B���h�E�N���X�쐬
	wf.setClassName(TEXT("Debug"));
	wf.setMenuName(IDR_MENU1);
	wf.regist();

#ifdef _MDEBUG
	m_hWnd = wf.createWnd(TEXT("Debug"), TEXT("App"), util::Vec2(0, 0), util::Vec2(Screen::WINDOW_WIDTH, Screen::WINDOW_HEIGHT));
#else
	m_hWnd = wf.createWnd(TEXT("Release"), TEXT("App"), util::Vec2(100, 100), util::Vec2(Screen::WINDOW_WIDTH, Screen::WINDOW_HEIGHT));
#endif

	ShowWindow(m_hWnd, SW_SHOW);

	// �f�o�C�X�ƃX���b�v�E�`�F�C���̍쐬
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));    // �\���̂̒l��������(�K�v�ȏꍇ)
	sd.BufferCount = 1;       // �o�b�N�E�o�b�t�@��
	sd.BufferDesc.Width = Screen::PIXEL_WIDTH;     // �o�b�N�E�o�b�t�@�̕�
	sd.BufferDesc.Height = Screen::PIXEL_HEIGHT;     // �o�b�N�E�o�b�t�@�̍���
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // �t�H�[�}�b�g
	sd.BufferDesc.RefreshRate.Numerator = 60;  // ���t���b�V���E���[�g(���q)
	sd.BufferDesc.RefreshRate.Denominator = 1; // ���t���b�V���E���[�g(����)
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // �o�b�N�E�o�b�t�@�̎g�p�@
	sd.OutputWindow = m_hWnd;    // �֘A�t����E�C���h�E
	sd.SampleDesc.Count = 1;        // �}���`�E�T���v���̐�
	sd.SampleDesc.Quality = 0;      // �}���`�E�T���v���̃N�I���e�B
	sd.Windowed = isWindowMode;             // �E�C���h�E�E���[�h
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // ���[�h�����؂�ւ�

	HRESULT hr = framework::DirectXInstance::getInstance()->create(sd);

	//HRESULT hr = framework::DirectXInstance::getInstance()->createReferenceMode(sd);


	//API��PC�̉𑜓x�𒴂����l���w�肵�Ă���Ǝ����ł��ꂢ���ɕύX����̂ōŏI�I�ȃT�C�Y�ɍX�V

	//TODO �o�O�̌��@���������m�{�Ńt���g�c�ݒ肾�ƃo�O��̂ŏC���K�v
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

	//�R�����g�̍s�폜
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
