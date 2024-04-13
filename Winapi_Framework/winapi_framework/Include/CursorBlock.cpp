#include "CursorBlock.h"
#include "Input.h"
#include "BlockDataBase.h"
#include "RuleManager.h"

CCursorBlock::CCursorBlock()
{
}

CCursorBlock::CCursorBlock(const CCursorBlock& obj) :
	CMoveObj(obj),
	m_fSpeed(obj.m_fSpeed),
	m_fDelay(obj.m_fDelay),
	m_iRot(obj.m_iRot),
	m_cType(obj.m_cType)

{
}

CCursorBlock::~CCursorBlock()
{
}

void CCursorBlock::MakeOrigin(char cType)
{
	m_fDelay = 0;
	m_iRot = 0;
	m_cType = cType;
	SetTexture(CBlockDataBase::GetData(cType)->GetTexture());
	m_tGamePos = POSITION(5, 1);
	SetPos(GET_SINGE(CRuleManager)->ToWorldPos(m_tGamePos));
}

bool CCursorBlock::Init()
{
	SetSpeed(1.0f);
	SetDelay(0.0f);
	SetRotation(0);
	SetTexture("Black", L"Black.bmp");
	SetTexture("Gray", L"Gray.bmp");
	SetTexture("Red", L"Red.bmp");
	SetTexture("Yellow", L"Yellow.bmp");

	return true;
}

void CCursorBlock::Input(float fDeltaTime)
{
	CMoveObj::Input(fDeltaTime);
	if (GET_SINGE(CInput)->KeyDown("MoveLeft")) {
		if (GET_SINGE(CRuleManager)->CheckMove(m_tGamePos.x - 1, m_tGamePos.y, m_cType, m_iRot)){
			MoveGame(-1, 0);
			Move(-GetSize().x, 0);
			m_fDelay *= 0.9f;
		}
	}

	if (GET_SINGE(CInput)->KeyDown("MoveRight")) {
		if (GET_SINGE(CRuleManager)->CheckMove(m_tGamePos.x + 1, m_tGamePos.y, m_cType, m_iRot)) {
			MoveGame(1, 0);
			Move(GetSize().x, 0);
			m_fDelay *= 0.9f;
		}
	}

	if (GET_SINGE(CInput)->KeyPress("Down") || GET_SINGE(CInput)->KeyDown("Down")) {
		AddDelay(fDeltaTime * 10);
	}

	if (GET_SINGE(CInput)->KeyDown("TurnLeft")) {
		if (GET_SINGE(CRuleManager)->CheckMove(m_tGamePos.x, m_tGamePos.y, m_cType, m_iRot + 1)) {
			TurnLeft();
		}
	}
	
	if (GET_SINGE(CInput)->KeyDown("TurnRight")) {
		if (GET_SINGE(CRuleManager)->CheckMove(m_tGamePos.x, m_tGamePos.y, m_cType, m_iRot - 1)) {
			TurnRight();
		}
	}
}

int CCursorBlock::Update(float fDeltaTime)
{
	CMoveObj::Update(fDeltaTime);
	AddDelay(fDeltaTime);
	
	return 0;
}

int CCursorBlock::LateUpdate(float fDeltaTime)
{
	CMoveObj::LateUpdate(fDeltaTime);
	if (m_fDelay >= m_fSpeed) {
		if (GET_SINGE(CRuleManager)->CheckMove(m_tGamePos.x, m_tGamePos.y + 1, m_cType, m_iRot)) {
			MoveGame(0, 1);
			Move(0, GetSize().x);
			m_fDelay = 0;
		}
		else {
			GET_SINGE(CRuleManager)->SetBlock(m_tGamePos.x, m_tGamePos.y, m_cType, m_iRot);
			MakeOrigin(GET_SINGE(CRuleManager)->GetBlockQueue()->PopQueueBlock());
			if (!GET_SINGE(CRuleManager)->CheckMove(m_tGamePos.x, m_tGamePos.y, m_cType, m_iRot))
				GET_SINGE(CRuleManager)->GameOver();
		}
	}

	return 0;
}

void CCursorBlock::Collision(float fDeltaTime)
{
	CMoveObj::Collision(fDeltaTime);
}

void CCursorBlock::Render(HDC hDC, float fDeltaTime)
{
	//CMoveObj::Render(hDC, fDeltaTime);
	vector<POSITION> arrData = CBlockDataBase::GetData(m_cType)->GetData(m_iRot % 4);

	for (int i = 0; i < arrData.size(); i++) {
		int x = m_tPos.x + (arrData[i].x * m_tSize.x);
		int y = m_tPos.y + (arrData[i].y * m_tSize.y);
		BitBlt(hDC, x, y, m_tSize.x, m_tSize.y, m_pTexture->GetDC(), 0, 0, SRCCOPY);
	}
	/*
	wchar_t buffer[256];
	wsprintfW(buffer, L"%d", m_iRot);

	TextOut(hDC, 500, 100, buffer, 12);
	*/
}

CCursorBlock* CCursorBlock::Clone()
{
	return new CCursorBlock(*this);
}
