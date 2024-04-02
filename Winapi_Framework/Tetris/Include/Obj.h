#pragma once

#include "Ref.h"
#include "Texture.h"
#include "Layer.h"
#include "Scene.h"

class CObj : 
	public CRef
{
public:
	friend class CLayer;

protected:
	CObj();
	CObj(const CObj& obj);
	virtual ~CObj();

private:
	static list<CObj*> m_ObjList; 
	static unordered_map<string, CObj*> m_mapPrototype;

public:
	static void AddObj(CObj* pObj); 
	static CObj* FindObject(const string& strTag);
	static void EraseObj(CObj* pObj);
	static void EraseObj(const string& strTag);
	static void EraseObj();
	static void ErasePrototype(const string& strTag);
	static void ErasePrototype();

protected:
	class CScene* m_pScene;
	class CLayer* m_pLayer;

public:
	void SetScene(class CScene* pScene) {
		m_pScene = pScene;
	}
	void SetLayer(class CLayer* pLayer) {
		m_pLayer = pLayer;
	}

	class CScene* GetScene() const {
		return m_pScene;
	}

	class CLayer* GetLayer() const {
		return m_pLayer;
	}

protected:
	string m_strTag;
	POSITION m_tPos;
	_SIZE m_tSize;
	POSITION m_tPivot; 
	class CTexture* m_pTexture;

public:
	string GetTag() const {
		return m_strTag;
	}

	POSITION GetPosition() const {
		return m_tPos;
	}

	_SIZE GetSize() const {
		return m_tSize;
	}

	POSITION GetPivot() const {
		return m_tPivot;
	}

public:
	void SetTag(const string& strTag) {
		m_strTag = strTag;
	}

	void SetPos(const POSITION& tPos) {
		m_tPos = tPos;
	}

	void SetPos(float x, float y) {
		m_tPos.x = x;
		m_tPos.y = y;
	}

	void SetPos(const POINT& tPos) {
		m_tPos = tPos;
	}

	void SetSize(const _SIZE& tSize) {
		m_tSize = tSize;
	}

	void SetSize(float x, float y) {
		m_tSize.x = x;
		m_tSize.y = y;
	}

	void SetSize(const POINT& tSize) {
		m_tSize = tSize;
	}

	void SetPivot(const POSITION& tPivot) {
		m_tPivot = tPivot;
	}

	void SetPivot(float x, float y) {
		m_tPivot.x = x;
		m_tPivot.y = y;
	}

	void SetPivot(const POINT& tPivot) {
		m_tPivot = tPivot;
	}

public:
	void SetTexture(class CTexture* pTexture);
	void SetTexture(const string& strKey, const wchar_t* pFileName = NULL, const string& strPathKey = TEXTURE_PATH);


public:
	virtual bool Init() = 0;
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
	virtual CObj* Clone() = 0;

public:
	static CObj* CreateObj(CObj* pObj, const string& strTag, class CLayer* pLayer);

	template<typename T>
	static T* CreatePrototype(const string& strTag) {
		T* pObj = new T;

		pObj->SetTag(strTag);

		if (!pObj->Init()) {
			SAFE_RELEASE(pObj);
			return NULL;
		}

		pObj->AddRef();
		m_mapPrototype.insert(make_pair(strTag, pObj));


		return pObj;
	}

	static CObj* CreateCloneObj(const string& strPrototypeKey, const string& strTag, class CLayer* pLayer = NULL);


private:
	static CObj* FindPrototype(const string& strKey);

};