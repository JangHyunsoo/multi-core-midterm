#include "Input.h"

DEFINITION_SINGLE(CInput)

CInput::CInput() :
	m_pCreateKey(NULL)
{
}

CInput::~CInput()
{
	Safe_Delete_Map(m_mapKey);
}

bool CInput::Init(HWND hWnd)
{
	m_hWnd = hWnd;

	AddKey(VK_LEFT, "Left");
	AddKey(VK_RIGHT, "Right");
	AddKey(VK_UP, "Forward");
	AddKey(VK_DOWN, "Back");
	//AddKey("A", "Left");
	//AddKey("D", "Right");
	//AddKey("W", "Forward");
	//AddKey("S", "Back");
	AddKey('F', "Down");
	AddKey('R', "Up");
	AddKey('B', "Activate");
	AddKey('V', "ActivateParallel");
	AddKey(VK_SPACE, "Reset");
	AddKey('1', "ThreadCount");
	AddKey('2', "Parallel");
	AddKey('3', "Algorithm");
	AddKey('4', "MapSize");

	return true;
}

void CInput::Update(float fDeltaTime)
{
	unordered_map<string, PKEYINFO>::iterator iter;
	unordered_map<string, PKEYINFO>::iterator iterEnd = m_mapKey.end();

	for (iter = m_mapKey.begin(); iter != iterEnd; ++iter) {
		int iPushCount = 0;
		for (size_t i = 0; i < iter->second->vecKey.size(); i++)
		{
			if (GetAsyncKeyState(iter->second->vecKey[i]) & 0x8000) {
				++iPushCount;
			}

			if (iPushCount == iter->second->vecKey.size()) {
				if (!iter->second->bDown && !iter->second->bPress) {
					iter->second->bDown = true;
					iter->second->bUp = false;
					iter->second->bPress = false;
				}
				else if(iter->second->bDown)
				{
					iter->second->bDown = false;
					iter->second->bPress = true;
					iter->second->bDown = false;
				}
			}
			else {
				if (iter->second->bDown || iter->second->bPress)
				{
					iter->second->bUp = true;
					iter->second->bPress = false;
					iter->second->bDown = false;
				}
				else if(iter->second->bUp) {
					iter->second->bUp = false;
					iter->second->bPress = false;
					iter->second->bDown = false;
				}
			}
		}
	}
}

bool CInput::KeyDown(const string& strKey) const
{
	PKEYINFO pInfo = FindKey(strKey);

	if (!pInfo)
		return false;

	return pInfo->bDown;
}

bool CInput::KeyPress(const string& strKey) const
{
	PKEYINFO pInfo = FindKey(strKey);

	if (!pInfo)
		return false;

	return pInfo->bPress;
}

bool CInput::KeyUp(const string& strKey) const
{
	PKEYINFO pInfo = FindKey(strKey);

	if (!pInfo)
		return false;

	return pInfo->bUp;
}

PKEYINFO CInput::FindKey(const string& strKey) const
{
	unordered_map<string, PKEYINFO>::const_iterator iter = m_mapKey.find(strKey);

	if (iter == m_mapKey.end()) 
		return NULL;

	return iter->second;
}
