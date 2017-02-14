#include <iostream>

#include <Windows.h>
#include <DirectXMath.h>

#include "engine/app.h"
#include "engine/engine.h"
#include "engine/utils.h"

#include "scene_objects/quad.h"
#include "scene_objects/particle_system.h"

using namespace std;
using namespace DirectX;

void InitSceneObjects()
{
	/*XMVECTORF32 quad_points[4] = {
		{ -5, 0, 5, 1 },
		{ 5, 0, 5, 1 },
		{ -5, 0, -5, 1 },
		{ 5, 0, -5, 1 },
	};
	Engine::Instance().AddTopLevelObject(new Quad(
		L"quad",
		quad_points[0], quad_points[1], quad_points[2], quad_points[3],
		L"resources/madoka.dds"));*/

	Engine::Instance().AddTopLevelObject(new ParticleSystem(
		L"particle_system", L"resources/particle.dds", 0, 1, 0.5, 3, 3600000));
}

int wmain()
{
	try
	{
		App::Instance().Init(L"D3D11 Playground", 800, 600);
		InitSceneObjects();
		App::Instance().EnterMainLoop();
	}
	catch (const AppError& e)
	{
		wcerr << e.reason() << endl;
		MessageBox(0, e.reason().c_str(), L"error", MB_OK | MB_ICONERROR);
	}
	App::Instance().Dispose();
}