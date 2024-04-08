#include "FpsUI.h"
#include "Timer.h"

CFpsUI::CFpsUI()
{
}

CFpsUI::CFpsUI(int x, int y, int font) : m_iX(x), m_iY(y), m_iFont(font)
{
	
}

CFpsUI::CFpsUI(const CFpsUI& obj) : m_iX(obj.m_iX), m_iY(obj.m_iY), m_iFont(obj.m_iFont)
{

}

bool CFpsUI::Init()
{
	return true;
}

void CFpsUI::Input(float fDeltaTime)
{
}

void CFpsUI::Render(HDC hDC, float fDeltaTime)
{
	wchar_t buffer[20];
	swprintf_s(buffer, L"%f", 1.f / CTimer::GetInst()->GetDeltaTime());
	TextOut(hDC, 10, 10, (LPCWSTR)buffer, 5);
}

CFpsUI* CFpsUI::Clone()
{
	return new CFpsUI(*this);
}
