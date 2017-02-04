#include <iostream>

#include <Windows.h>

#include "app.h"
#include "utils.h"

using namespace std;

int wmain()
{
	try
	{
		App::Instance().Init(L"D3D11 Playground", 800, 600);
		App::Instance().EnterMainLoop();
	}
	catch (const AppError& e)
	{
		wcerr << e.reason() << endl;
		MessageBox(0, e.reason().c_str(), L"error", MB_OK | MB_ICONERROR);
	}
	App::Instance().Dispose();
}