#pragma once
#include "Game.h"

class CPathManager
{
private:
	unordered_map<string, wstring> m_mapPath;

public: 
	bool Init();
	bool CreatePath(const string& strKey, const wchar_t* pPath, const string& strBaseKey = ROOT_PATH);
	const wchar_t* FindPath(const string& strKey);

	DECLARE_SINGLE(CPathManager)
};

