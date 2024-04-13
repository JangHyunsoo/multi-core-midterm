#pragma once
#include "StaticObj.h"
class CFpsUI :
    public CStaticObj
{
private:
    int m_iX;
    int m_iY;
    int m_iFont;
    double m_fCurTime{ 0 };
    double m_fCurCount{ 0 };
    double m_fCurFPS{ 0 };
public:
    CFpsUI();
    CFpsUI(int x, int y, int font);
    CFpsUI(const CFpsUI& obj);
public:
    virtual bool Init();
    virtual void Input(float fDeltaTime);
    virtual void Render(HDC hDC, float fDeltaTime);
    virtual CFpsUI* Clone();
private:
    void calFPS(float fDeltaTime) {
        if (fDeltaTime > 0.5) {
            m_fCurTime = fDeltaTime;
            m_fCurCount = 1;
        }
        m_fCurTime += fDeltaTime;
        m_fCurCount += 1;
    }
};

