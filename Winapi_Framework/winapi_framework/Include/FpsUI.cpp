#include "FpsUI.h"
#include "Timer.h"
#include <wchar.h>
#include "Player.h"

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
	m_fCurTime = 1;
	m_fCurCount = 1;
	return true;
}

void CFpsUI::Input(float fDeltaTime)
{
}

void CFpsUI::Render(HDC hDC, float fDeltaTime)
{
	calFPS(fDeltaTime);

	if (m_fCurTime > 0.5) {
		m_fCurFPS = m_fCurCount / m_fCurTime;
		m_fCurTime = 0;
		m_fCurCount = 0.01;
	}
	wchar_t buffer[20];
	swprintf_s(buffer, L"%.2f", m_fCurFPS);
	auto pos = Player::GetInst()->GetPos();
	TextOut(hDC, pos.x - 320, pos.y - 320, (LPCWSTR)buffer, wcslen(buffer));
}

CFpsUI* CFpsUI::Clone()
{
	return new CFpsUI(*this);
}
