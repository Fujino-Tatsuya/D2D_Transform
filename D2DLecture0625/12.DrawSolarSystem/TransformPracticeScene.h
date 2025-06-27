#pragma once
#include "OnlyForTestScene.h"
#include "Camera2D.h"
#include <list>
#include <vector>
#include <wrl/client.h>
#include <d2d1_1.h>

class Planet;


class TransformPracticeScene : public OnlyForTestScene
{
public:
	TransformPracticeScene() = default;
	virtual ~TransformPracticeScene();

	void SetUp(HWND hWnd) override;

	void Tick(float deltaTime) override;

	void OnResize(int width, int height) override;

private:

	void ProcessKeyboardEvents();

	void InitPlanet();

	UnityCamera m_UnityCamera;
	//ComPtr<ID2D1Bitmap1> m_BitmapPtr;
	std::vector<ComPtr<ID2D1Bitmap1>> m_BitmapList;

	//ComPtr<ID2D1Bitmap1> m_Bitmap1;      // 1: Sun
	//ComPtr<ID2D1Bitmap1> m_Bitmap2;     // 2: Mercury
	//ComPtr<ID2D1Bitmap1> m_Bitmap3;     // 3: Venus
	//ComPtr<ID2D1Bitmap1> m_Bitmap4;     // 4: Earth
	//ComPtr<ID2D1Bitmap1> m_Bitmap5;     // 5: Mars
	//ComPtr<ID2D1Bitmap1> m_Bitmap6;     // 6: Jupiter
	//ComPtr<ID2D1Bitmap1> m_Bitmap7;     // 7: Saturn
	//ComPtr<ID2D1Bitmap1> m_Bitmap8;     // 8: Uranus
	//ComPtr<ID2D1Bitmap1> m_Bitmap9;     // 9: Neptune
	//ComPtr<ID2D1Bitmap1> m_Bitmap10;       //10: Luna

	std::vector<Planet*> m_PlanetObjects;
};
