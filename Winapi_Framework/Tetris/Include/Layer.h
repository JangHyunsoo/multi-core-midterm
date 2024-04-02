#pragma once
#include "Game.h"
#include "Obj.h"

class CLayer
{
private:
	friend class CScene;

public:
	CLayer();
	~CLayer();
private:
	class CScene* m_pScene; // InGameScene
	string m_strTag;
	int m_iZOrder;
	bool m_bEnable;
	bool m_bLife;

public:
	list<class CObj*> m_ObjList;

public:
	void SetTag(const string& strTag) {
		m_strTag = strTag;
	}

	void SetZOrder(int iZOrber) {
		m_iZOrder;
	}
	
	void SetScene(class CScene* pScene) {
		m_pScene = pScene;
	}

	string GetTag() const {
		return m_strTag;
	}

	int GetZOrder() const {
		return m_iZOrder;
	}

	class CScene* GetScene() const {
		return m_pScene;
	}

	void SetEnable(bool bEnable) {
		m_bEnable = bEnable;
	}

	void Die() {
		m_bLife = false;
	}

	bool GetEnable() const {
		return m_bEnable;
	}

	bool GetLife() const {
		return m_bLife;
	}

public:
	void AddObject(class CObj* pObj);

public:
	void Input(float fDeltaTime);
	int Update(float fDeltaTime);
	int LateUpdate(float fDeltaTime);
	void Collision(float fDeltaTime);
	void Render(HDC hDC, float fDeltaTime);

};

