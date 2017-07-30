#include"WindowFactory.h"
#include<assert.h>

namespace framework {

	WindowFactory::WindowFactory(HINSTANCE hInstance)
	{
		//縦と横のサイズが変更される
		m_WndClass.style = CS_HREDRAW | CS_VREDRAW;
		//デフォルトのプロシージャ
		m_WndClass.lpfnWndProc = DefWindowProc;
		m_WndClass.cbClsExtra = m_WndClass.cbWndExtra = 0;
		m_WndClass.hInstance = hInstance;
		m_WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		m_WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		m_WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		m_WndClass.lpszMenuName = NULL;
		m_WndClass.lpszClassName = TEXT("default");

	}

	WindowFactory::~WindowFactory()
	{
	}

	HWND WindowFactory::createWnd(LPCSTR className, LPCSTR title, util::Vec2 size)
	{
		return createWnd(className,title, XMFLOAT2(50,50),size);
	}

	HWND WindowFactory::createWnd(LPCSTR className, LPCSTR title, util::Vec2 pos, util::Vec2 size)
	{
		HWND hWnd = CreateWindow(className,title, WS_OVERLAPPED | WS_SYSMENU,
									pos.x,pos.y, size.x, size.y,NULL,NULL,
									m_WndClass.hInstance,NULL);
		assert(hWnd != NULL && "ウィンドウハンドル作成失敗");

		//サイズの差
		auto differenceSize = clientSize(hWnd,size);

		SetWindowPos(hWnd,NULL,0,0,size.x + differenceSize.x, size.y + differenceSize.y, (SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE));

		UpdateWindow(hWnd);

		return hWnd;
	}

	void WindowFactory::regist()
	{
		auto success = RegisterClass(&m_WndClass);

		assert(success && "ウィンドウクラス登録失敗");
	}

	void WindowFactory::setClassName(LPCSTR className)
	{
		m_WndClass.lpszClassName = className;
	}

	void WindowFactory::setProc(Proc proc)
	{
		m_WndClass.lpfnWndProc = proc;
	}

	void WindowFactory::setStyle(UINT style)
	{
		m_WndClass.style = style;
	}

	void WindowFactory::setMenuName(LPSTR menuName)
	{
		m_WndClass.lpszMenuName = menuName;
	}

	void WindowFactory::setMenuName(UINT nemeName)
	{
		m_WndClass.lpszMenuName = MAKEINTRESOURCE(nemeName);
	}

	void WindowFactory::setIcon()
	{
	}

	void WindowFactory::setCursor()
	{
	}

	util::Vec2 WindowFactory::clientSize(HWND hwnd, util::Vec2& size)
	{
		RECT rc1;
		RECT rc2;

		GetWindowRect(hwnd, &rc1);
		GetClientRect(hwnd, &rc2);

		float x = (rc1.right - rc1.left) - (rc2.right - rc2.left);
		float y = (rc1.bottom - rc1.top) - (rc2.bottom - rc2.top);

		return util::Vec2(x,y);
	}

}
