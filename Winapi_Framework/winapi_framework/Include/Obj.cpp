#include "Obj.h"
#include "Layer.h"
#include "Texture.h"
#include "ResourceManager.h"

list<CObj*> CObj::m_ObjList;
unordered_map<string, CObj*> CObj::m_mapPrototype;

CObj::CObj() :
	m_pTexture(NULL)
{
}

CObj::CObj(const CObj& obj)
{
	*this = obj;

	if (m_pTexture)
		m_pTexture->AddRef();
}

CObj::~CObj() {
	SAFE_RELEASE(m_pTexture);
}

void CObj::AddObj(CObj* pObj)
{
	pObj->AddRef();
	m_ObjList.push_back(pObj);
}

CObj* CObj::FindObject(const string& strTag)
{
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter) {
		if ((*iter)->GetTag() == strTag) {
			(*iter)->AddRef();
			return *iter;
		}
	}

	return NULL;
}

void CObj::EraseObj(CObj* pObj)
{
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter) {
		if ((*iter) == pObj) {
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			return;
		}
	}
}

void CObj::EraseObj(const string& strTag)
{
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; ++iter) {
		if ((*iter)->GetTag() == strTag) {
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			return;
		}
	}
}

void CObj::EraseObj()
{
	Safe_Release_VecList(m_ObjList);
}

void CObj::ErasePrototype(const string& strTag)
{
	unordered_map<string, CObj*>::iterator iter = m_mapPrototype.find(strTag);
	
	if (iter->second)
		return;

	SAFE_RELEASE(iter->second);
	m_mapPrototype.erase(iter);

}

void CObj::ErasePrototype()
{
	Safe_Release_Map(m_mapPrototype);
}

void CObj::SetTexture(CTexture* pTexture)
{
	SAFE_RELEASE(m_pTexture);
	m_pTexture = pTexture;

	if (pTexture)
		pTexture->AddRef();
}

void CObj::SetTexture(const string& strKey, const wchar_t* pFileName, const string& strPathKey)
{
	SAFE_RELEASE(m_pTexture);
	m_pTexture = GET_SINGE(CResourceManager)->LoadTexture(strKey, pFileName, strPathKey);
}

void CObj::Input(float fDeltaTime)
{
}

int CObj::Update(float fDeltaTime)
{
	return 0;
}

int CObj::LateUpdate(float fDeltaTime)
{
	return 0;
}

void CObj::Collision(float fDeltaTime)
{
}


void CObj::Render(HDC hDC, float fDeltaTime)
{
	if (m_pTexture) {
		BitBlt(hDC, m_tPos.x, m_tPos.y, m_tSize.x, m_tSize.y, m_pTexture->GetDC(), 0, 0, SRCCOPY);
	}
}

CObj* CObj::CreateObj(CObj* pClone, const string& strTag, CLayer* pLayer)
{
	CObj* pObj = pClone->Clone();

	pObj->SetTag(strTag);

	if (pLayer) {
		pLayer->AddObject(pObj);
	}

	AddObj(pObj);

	return pObj;
}

CObj* CObj::CreateCloneObj(const string& strPrototypeKey, const string& strTag, class CLayer* pLayer)

{
	CObj* pPrototype = (CObj*)FindPrototype(strPrototypeKey);

	if (!pPrototype)
		return NULL;

	CObj* pObj = (CObj*)pPrototype->Clone();

	pObj->SetTag(strTag);

	if (pLayer) {
		pLayer->AddObject(pObj);
	}

	AddObj(pObj);

	return pObj;
}

CObj* CObj::FindPrototype(const string& strKey)
{
	unordered_map<string, CObj*>::iterator iter = m_mapPrototype.find(strKey);

	if (iter == m_mapPrototype.end())
		return NULL;
	
	return iter->second;

}
