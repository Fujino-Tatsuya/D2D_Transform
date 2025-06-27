#pragma once

#include "pch.h"
#include "InputManager.h"
#include "D2DTransform.h"
#include "SolarSystemRenderer.h"
#include "TransformPracticeScene.h"

using TestRenderer = myspace::D2DRenderer;
using Vec2 = MYHelper::Vector2F;

class Planet
{
public:
    Planet(ComPtr<ID2D1Bitmap1>& bitmap)
    {
        m_BitmapPtr = bitmap;

        ++s_id;
        m_name += std::to_wstring(s_id);

        m_renderTM = MYTM::MakeRenderMatrix(true);// unity render true

        D2D1_SIZE_F size = { m_rect.right - m_rect.left, m_rect.bottom - m_rect.top }; //width, height

        m_transform.SetPivotPreset(D2DTM::PivotPreset::Center, size); // 피벗 위치랑 크기만 넣으면 알아서 해주는구나 개쩐다
        //m_transform.SetPivotPreset(D2DTM::PivotPreset::BottomLeft, size);
        //m_transform.SetPivotPreset(D2DTM::PivotPreset::TopLeft, size);
    }

    ~Planet() = default;

    void Update(float deltaTime) // 회전
    {
        if (m_isSelfRotation)
        {
            m_transform.Rotate(deltaTime * 36.f);
        }
    }

    void Draw(TestRenderer& testRender, D2D1::Matrix3x2F viewTM)
    {
        //check1: 객체마다 크기 다르게 하려고 static rect 제외시켰음
        D2D1::Matrix3x2F worldTM = m_transform.GetWorldMatrix();
        D2D1::Matrix3x2F finalTM = m_renderTM * worldTM * viewTM;
        // render는 객체 고유 속성, world는 논리 세상 위치, viewTM은 카메라 위치?

        D2D1::ColorF boxColor = D2D1::ColorF::White;

        if (m_isLeader) boxColor = D2D1::ColorF::Yellow;
        else if (m_isSelected) boxColor = D2D1::ColorF::Green;

        testRender.SetTransform(finalTM);
        testRender.DrawRectangle(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom, D2D1::ColorF::Pink);
        testRender.DrawBitmap(m_BitmapPtr.Get(), m_rect);

        //D2D1_RECT_F dest = D2D1::RectF(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom); //정보 출력을 위한 rect
    }

    void SetPosition(const Vec2& pos)
    {
        m_transform.SetPosition(pos);
    }

    void Move(const Vec2& offset)
    {
        m_transform.Translate(offset);
    }

    void Rotate(float angle)
    {
        m_transform.Rotate(angle);
    }

    void ToggleSelected()
    {
        m_isSelected = !m_isSelected;
    }

    bool IsSelected() const
    {
        return m_isSelected;
    }

    void ToggleSelfRotation()
    {
        m_isSelfRotation = !m_isSelfRotation;
    }

    D2DTM::Transform* GetTransform()
    {
        return &m_transform;
    }

    bool IsHitTest(D2D1_POINT_2F worldPoint, D2D1::Matrix3x2F viewTM)// 태양계에서 굳이 없어도 되는 함순데 일단 따라치기
    {
        // 
        //클릭시 스크린 좌표 -> 월드 좌표
        D2D1::Matrix3x2F worldTM = m_transform.GetWorldMatrix();
        D2D1::Matrix3x2F finalTM = m_renderTM * worldTM * viewTM;

        finalTM.Invert();
        // 시간을 달려서 world 좌표가 될수 있다면~ // 이 과정 이해 안되는데 공부해서 이해 해야할듯

        D2D1_POINT_2F localPt = finalTM.TransformPoint(worldPoint); // ??? 진짜 공부 해야할듯

        // 3) 로컬 사각형 정의
        // (0,0) ~ (width, height) 범위에 있다면 히트!
        // m_rect = D2D1::RectF(0.f, 0.f, 100.f, 100.f);

        return MYTM::IsPointInRect(localPt, m_rect);
    }

    void SetParent(Planet* parent)
    {
        assert(parent != nullptr);

        if (nullptr != m_transform.GetParent())
        {
            m_transform.DetachFromParent();
        }

        m_transform.SetParent(parent->GetTransform());//parent 오브젝트의 transform 컴포넌트 등록
    }

    void DetachFromParent()
    {
        m_transform.DetachFromParent(); //
    }

    void SetLeader(bool isLeader)
    {
        m_isLeader = isLeader;
    }

private:

    D2DTM::Transform m_transform;

    MAT3X2F m_renderTM;

    D2D1_RECT_F m_rect = D2D1::RectF(0.f, 0.f, 100.f, 100.f);

    std::wstring m_name = L"";

    bool m_isSelected = false;
    bool m_isLeader = false;

    bool m_isSelfRotation = false;

    ComPtr<ID2D1Bitmap1> m_BitmapPtr;

    static int s_id;
};
