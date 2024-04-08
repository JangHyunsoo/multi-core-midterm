#pragma once
#include "StaticObj.h"
class CFpsUI :
    public CStaticObj
{
private:
    int m_iX;
    int m_iY;
    int m_iFont;
public:
    CFpsUI();
    CFpsUI(int x, int y, int font);
    CFpsUI(const CFpsUI& obj);
public:
    virtual bool Init();
    virtual void Input(float fDeltaTime);
    virtual void Render(HDC hDC, float fDeltaTime);
    virtual CFpsUI* Clone();
};

