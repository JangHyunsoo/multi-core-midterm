#pragma once
#include "MoveObj.h"
#include "Input.h"

class Player :
    public CMoveObj
{
	DECLARE_SINGLE(Player)
private:
	float m_fSpeed = 500;
	HBRUSH m_pBrush;
	HBRUSH m_pWhite;
public:
	virtual bool Init() {
		//MessageBox(NULL, L"Test", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		SetPos(500, 500);
		SetSize(8, 8);
		m_pWhite = CreateSolidBrush(RGB(255, 255, 255));
		m_pBrush = CreateSolidBrush(RGB(200, 0, 0));
		return true;
	}
	virtual void Input(float fDeltaTime) {
		POSITION pos{ 0, 0 };
		if (GET_SINGE(CInput)->KeyPress("Left")) {
			pos.x = -1;
		}
		if (GET_SINGE(CInput)->KeyPress("Right")) {
			pos.x = 1;
		}
		if (GET_SINGE(CInput)->KeyPress("Forward")) {
			pos.y = -1;
		}
		if (GET_SINGE(CInput)->KeyPress("Back")) {
			pos.y = 1;
		}
		Move(pos * m_fSpeed * fDeltaTime);
	}
	virtual void Render(HDC hDC, float fDeltaTime) {
		SelectObject(hDC, m_pBrush);
		Rectangle(hDC, m_tPos.x - m_tSize.x / 2, m_tPos.y - m_tSize.y / 2, m_tPos.x + m_tSize.x / 2, m_tPos.y + m_tSize.y / 2);
		SelectObject(hDC, m_pWhite);
	}
	virtual	CMoveObj* Clone() {
		return new Player(*this);
	}
	void SetSpeed(float speed) {
		m_fSpeed = speed;
	}
};

