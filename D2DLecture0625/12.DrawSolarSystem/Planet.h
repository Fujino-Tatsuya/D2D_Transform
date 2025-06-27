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

        m_transform.SetPivotPreset(D2DTM::PivotPreset::Center, size); // �ǹ� ��ġ�� ũ�⸸ ������ �˾Ƽ� ���ִ±��� ��¾��
        //m_transform.SetPivotPreset(D2DTM::PivotPreset::BottomLeft, size);
        //m_transform.SetPivotPreset(D2DTM::PivotPreset::TopLeft, size);
    }

    ~Planet() = default;

    void Update(float deltaTime) // ȸ��
    {
        if (m_isSelfRotation)
        {
            m_transform.Rotate(deltaTime * 36.f);
        }
    }

    void Draw(TestRenderer& testRender, D2D1::Matrix3x2F viewTM)
    {
        //check1: ��ü���� ũ�� �ٸ��� �Ϸ��� static rect ���ܽ�����
        D2D1::Matrix3x2F worldTM = m_transform.GetWorldMatrix();
        D2D1::Matrix3x2F finalTM = m_renderTM * worldTM * viewTM;
        // render�� ��ü ���� �Ӽ�, world�� �� ���� ��ġ, viewTM�� ī�޶� ��ġ?

        D2D1::ColorF boxColor = D2D1::ColorF::White;

        if (m_isLeader) boxColor = D2D1::ColorF::Yellow;
        else if (m_isSelected) boxColor = D2D1::ColorF::Green;

        testRender.SetTransform(finalTM);
        testRender.DrawRectangle(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom, D2D1::ColorF::Pink);
        testRender.DrawBitmap(m_BitmapPtr.Get(), m_rect);

        //D2D1_RECT_F dest = D2D1::RectF(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom); //���� ����� ���� rect
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

    bool IsHitTest(D2D1_POINT_2F worldPoint, D2D1::Matrix3x2F viewTM)// �¾�迡�� ���� ��� �Ǵ� �Լ��� �ϴ� ����ġ��
    {
        // 
        //Ŭ���� ��ũ�� ��ǥ -> ���� ��ǥ
        D2D1::Matrix3x2F worldTM = m_transform.GetWorldMatrix();
        D2D1::Matrix3x2F finalTM = m_renderTM * worldTM * viewTM;

        finalTM.Invert();
        // �ð��� �޷��� world ��ǥ�� �ɼ� �ִٸ�~ // �� ���� ���� �ȵǴµ� �����ؼ� ���� �ؾ��ҵ�

        D2D1_POINT_2F localPt = finalTM.TransformPoint(worldPoint); // ??? ��¥ ���� �ؾ��ҵ�

        // 3) ���� �簢�� ����
        // (0,0) ~ (width, height) ������ �ִٸ� ��Ʈ!
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

        m_transform.SetParent(parent->GetTransform());//parent ������Ʈ�� transform ������Ʈ ���
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
