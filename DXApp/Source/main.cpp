#define WIN32_LEAN_AND_MEAN


#include<Windows.h>
#include<D3DX11.h>
#include<Source\Application\TestApp.h>

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int) {
	
	TestApp app(hInst);

	auto msg = app.run();

	return msg;
}