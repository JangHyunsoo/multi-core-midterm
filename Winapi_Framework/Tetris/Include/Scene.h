#pragma once

#include "Game.h"
#include "Layer.h"

class CScene
{
protected:
	friend class CSceneManager;
protected:
	CScene();
	virtual ~CScene() = 0;
protected:
	list<class CLayer*> m_LayerList;
public:
	class CLayer* CreateLayer(const string& strTag, int iZOrber); // FLOOR
	class CLayer* FindLayer(const string& strTag);

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);

public:
	static bool LayerSort(class CLayer* A, class CLayer* B);
};
