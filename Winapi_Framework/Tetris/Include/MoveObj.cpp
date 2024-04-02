#include "MoveObj.h"

CMoveObj::CMoveObj() : 
	m_fAngle(0.f)
{
}

CMoveObj::CMoveObj(const CMoveObj& obj) :
	CObj(obj)
{
	m_fAngle = obj.m_fAngle;
}

CMoveObj::~CMoveObj()
{
}

void CMoveObj::Move(float x, float y)
{
	m_tPos.x += x;
	m_tPos.y += y;
}

void CMoveObj::Move(float x, float y, float fDeltaTime)
{
	m_tPos.x += x * fDeltaTime;
	m_tPos.y += y * fDeltaTime;

}

void CMoveObj::Move(const POSITION& tMove)
{
	m_tPos.x += tMove.x;
	m_tPos.y += tMove.y;
}

void CMoveObj::Move(const POSITION& tMove, float fDeltaTime)
{
	m_tPos.x += tMove.x * fDeltaTime;
	m_tPos.y += tMove.y * fDeltaTime;
}

void CMoveObj::Input(float fDeltaTime)
{
	CObj::Input(fDeltaTime);
}

int CMoveObj::Update(float fDeltaTime)
{
	CObj::Update(fDeltaTime);
	return 0;
}

int CMoveObj::LateUpdate(float fDeltaTime)
{
	CObj::LateUpdate(fDeltaTime);
	return 0;
}

void CMoveObj::Collision(float fDeltaTime)
{
	CObj::Collision(fDeltaTime);
}

void CMoveObj::Render(HDC hDC, float fDeltaTime)
{
	CObj::Render(hDC, fDeltaTime);
}
