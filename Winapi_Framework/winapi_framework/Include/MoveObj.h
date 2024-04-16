#pragma once
#include "Obj.h"

class CMoveObj :
	public CObj
{
protected:
	CMoveObj();
	CMoveObj(const CMoveObj& obj);
	virtual ~CMoveObj();
private:
	float m_fAngle;

public:
	void Move(float x, float y);
	void Move(float x, float y, float fDeltaTiem);
	void Move(const POSITION& tMove);
	void Move(const POSITION& tMove, float fDeltaTiem);

public:
	virtual bool Init() = 0;
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual	CMoveObj* Clone() = 0;
};
