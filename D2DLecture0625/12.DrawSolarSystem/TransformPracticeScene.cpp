
#include "pch.h"
#include "InputManager.h"
#include "D2DTransform.h"
#include "SolarSystemRenderer.h"
#include "TransformPracticeScene.h"
#include "Planet.h"

using TestRenderer = myspace::D2DRenderer;
using Vec2 = MYHelper::Vector2F;

constexpr int planetCount = 10; //행성 갯수

TransformPracticeScene::~TransformPracticeScene()
{
	for (auto planets : m_PlanetObjects)
	{
		delete planets;
	}
}

void TransformPracticeScene::SetUp(HWND hWnd)
{
	m_PlanetObjects.reserve(planetCount);
	   m_BitmapList.resize(planetCount);

	m_hWnd = hWnd;

	SetWindowText(m_hWnd,
		L"가상의 태양계를 만들어 주세요.");

	std::cout << "태양은 자전을 해야 합니다." << std::endl;
	std::cout << "행성들은 자전을 하며 동시에 태영의 자전에 영향을 받아 공전하는 것처럼 보입니다." << std::endl;
	std::cout << "달은 자전을 하면서 동시에 지구의 자전에 영향을 받아 공전하는 것처럼 보입니다." << std::endl;
	std::cout << "회전 속도는 자유롭게 설정하세요." << std::endl;

	//이미지 설정
	//SolarSystemRenderer::Instance().CreateBitmapFromFile(L"../Resource/cat.png", *m_BitmapPtr.GetAddressOf());

	SolarSystemRenderer::Instance().CreateBitmapFromFile(L"../Resource/sun.png", *m_BitmapList[0].GetAddressOf());// 1: Sun
	SolarSystemRenderer::Instance().CreateBitmapFromFile(L"../Resource/mercury.png", *m_BitmapList[1].GetAddressOf());	  // 2: Mercury
	SolarSystemRenderer::Instance().CreateBitmapFromFile(L"../Resource/venus.png", *m_BitmapList[2].GetAddressOf());  // 3: Venus
	SolarSystemRenderer::Instance().CreateBitmapFromFile(L"../Resource/earth.png", *m_BitmapList[3].GetAddressOf());  // 4: Earth
	SolarSystemRenderer::Instance().CreateBitmapFromFile(L"../Resource/mars.png", *m_BitmapList[4].GetAddressOf());// 5: Mars
	SolarSystemRenderer::Instance().CreateBitmapFromFile(L"../Resource/jupiter.png", *m_BitmapList[5].GetAddressOf());	  // 6: Jupiter
	SolarSystemRenderer::Instance().CreateBitmapFromFile(L"../Resource/saturn.png", *m_BitmapList[6].GetAddressOf());	  // 7: Saturn
	SolarSystemRenderer::Instance().CreateBitmapFromFile(L"../Resource/uranus.png", *m_BitmapList[7].GetAddressOf());	  // 8: Uranus
	SolarSystemRenderer::Instance().CreateBitmapFromFile(L"../Resource/neptune.png", *m_BitmapList[8].GetAddressOf());	  // 9: Neptune
	SolarSystemRenderer::Instance().CreateBitmapFromFile(L"../Resource/moon.png", *m_BitmapList[9].GetAddressOf());//10: Luna

	//SolarSystemRenderer::Instance().CreateBitmapFromFile(L"../Resource/sun.png", *m_Bitmap1.GetAddressOf());
	//SolarSystemRenderer::Instance().CreateBitmapFromFile(L"../Resource/mercury.png", *m_Bitmap2.GetAddressOf());
	//SolarSystemRenderer::Instance().CreateBitmapFromFile(L"../Resource/venus.png", *m_Bitmap3.GetAddressOf());
	//SolarSystemRenderer::Instance().CreateBitmapFromFile(L"../Resource/earth.png", *m_Bitmap4.GetAddressOf());
	//SolarSystemRenderer::Instance().CreateBitmapFromFile(L"../Resource/mars.png", *m_Bitmap5.GetAddressOf());
	//SolarSystemRenderer::Instance().CreateBitmapFromFile(L"../Resource/jupiter.png", *m_Bitmap6.GetAddressOf());
	//SolarSystemRenderer::Instance().CreateBitmapFromFile(L"../Resource/saturn.png", *m_Bitmap7.GetAddressOf());
	//SolarSystemRenderer::Instance().CreateBitmapFromFile(L"../Resource/uranus.png", *m_Bitmap8.GetAddressOf());
	//SolarSystemRenderer::Instance().CreateBitmapFromFile(L"../Resource/neptune.png", *m_Bitmap9.GetAddressOf());
	//SolarSystemRenderer::Instance().CreateBitmapFromFile(L"../Resource/moon.png", *m_Bitmap10.GetAddressOf());
																					   
	InitPlanet();

	RECT rc;
	if (::GetClientRect(hWnd, &rc))
	{
		float w = static_cast<float>(rc.right - rc.left);
		float h = static_cast<float>(rc.bottom - rc.top);

		m_UnityCamera.SetScreenSize(w, h);
	}
}

void TransformPracticeScene::Tick(float deltaTime)
{
	for (auto planets : m_PlanetObjects)
	{
		planets->Update(deltaTime);
	}

	MAT3X2F cameraTM = m_UnityCamera.GetViewMatrix(); //화면 중심이 중점인 카메라
	//의 중심기준의 getviewMatrix

	MAT3X2F renderTM = MYTM::MakeRenderMatrix(true);

	MAT3X2F finalTM = renderTM * cameraTM;

	static myspace::D2DRenderer& globalRenderer = SolarSystemRenderer::Instance(); //CRTP curiously recurring t pat
	//직역: 기묘한 재귀 패턴

	globalRenderer.RenderBegin();

	globalRenderer.SetTransform(finalTM);

	globalRenderer.DrawRectangle(-10.f, 10.f, 10.f, -10.f, D2D1::ColorF::Red);
	globalRenderer.DrawCircle(0.f, 0.f, 5.f, D2D1::ColorF::Red);

	for (auto source : m_PlanetObjects)
	{
		source->Draw(globalRenderer, cameraTM);
	}

	globalRenderer.RenderEnd();
}

void TransformPracticeScene::OnResize(int width, int height)
{
	m_UnityCamera.SetScreenSize(width, height);
}

void TransformPracticeScene::ProcessKeyboardEvents()
{
	//키 입력은 굳이 안해도 되잖슴~
	//근데 읽고 따라치긴 해야할듯 암튼 대음
}

void TransformPracticeScene::InitPlanet()
{
	for(int i = 0; i < planetCount; i++)
	{
		Planet* bigbang = new Planet(m_BitmapList[i]);

		bigbang->SetPosition(Vec2(0, i * -80));
		m_PlanetObjects.push_back(bigbang);

	}

		//Planet* bigbang = new Planet(m_Bitmap1);
		//bigbang->SetPosition(Vec2(0, 1 * -0.5f));
		//m_PlanetObjects.push_back(bigbang);
	

}

