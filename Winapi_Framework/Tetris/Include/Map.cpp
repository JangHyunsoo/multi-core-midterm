#include "Map.h"
#include "ResourceManager.h"
#include "Flag.h"
#include "Function.h"
#include "Input.h"
#include <random>

CMap::CMap() {

}

CMap::CMap(int width, int height) : m_iWidth(width), m_iHeight(height)
{
	m_2DMap = Array2D(m_iHeight, std::vector<TILE_TYPE>(m_iWidth, TILE_TYPE::LAND));
	Init();
	GenerateMap();
}

CMap::CMap(const CMap& obj) :
	CStaticObj(obj), m_iWidth(obj.m_iWidth), m_iHeight(obj.m_iHeight)
{
	m_2DMap = Array2D(obj.m_iHeight, std::vector<TILE_TYPE>(obj.m_iWidth, TILE_TYPE::LAND));
	Init();
	GenerateMap();
}


CMap::~CMap()
{
}

void CMap::SetCell(int x, int y, TILE_TYPE bValue)
{
	if (IsMap(x, y)) {
		m_2DMap[y][x] = bValue;
	}
}

TILE_TYPE CMap::GetCell(int x, int y)
{
	return m_2DMap[y][x];
}

bool CMap::Init()
{
	SetPos(0, 0);
	InitTexture();

	return true;
}

void CMap::InitTexture() {
	m_pAir = CResourceManager::GetInst()->LoadTexture("AIR", L"AIR.bmp");
	m_pLand = CResourceManager::GetInst()->LoadTexture("LAND", L"LAND.bmp");
	m_pStone = CResourceManager::GetInst()->LoadTexture("STONE", L"STONE.bmp");
	m_pWater = CResourceManager::GetInst()->LoadTexture("WATER", L"WATER.bmp");
}

void CMap::GenerateMap() {
	SetupRandomMap();
}

void CMap::SetupRandomMap() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0, 1.0f);

	for (int y = 3; y < m_iHeight - 3; y++)
	{
		for (int x = 3; x < m_iWidth - 3; x++)
		{
			if(dis(gen) > 0.5)
			{
				m_2DMap[y][x] = TILE_TYPE::AIR;
			}
		}
	}
}

void CMap::CalCellular() {
	for (int y = 3; y < m_iHeight - 3; y++)
	{
		for (int x = 3; x < m_iWidth - 3; x++)
		{
			int counter = CountSurround(x, y);
			if (counter > 5) {
				m_2DMap[y][x] = TILE_TYPE::LAND;
			}
			else if (counter < 5) {
				m_2DMap[y][x] = TILE_TYPE::AIR;
			}
		}
	}
}

int CMap::CountSurround(int x, int y) {
	int counter = 0;
	for (int dy = -1; dy <= 1; dy++)
	{
		for (int dx = -1; dx <= 1; dx++)
		{
			if (m_2DMap[y + dy][x + dx] == TILE_TYPE::LAND)
			{
				counter++;
			}
		}
	}
	return counter;
}

CTexture* CMap::getTile(TILE_TYPE tile_type) {
	switch (tile_type)
	{
	case TILE_TYPE::AIR:
		return m_pAir;
	case TILE_TYPE::LAND:
		return m_pLand;
	case TILE_TYPE::STONE:
		return m_pStone;
	case TILE_TYPE::WATER:
		return m_pWater;
	}
	return nullptr;
}

bool CMap::IsMap(int x, int y)
{
	return x < m_iWidth && y < m_iHeight && x >= 0 && y >= 0;
}

void CMap::Input(float fDeltaTime)
{
	if (GET_SINGE(CInput)->KeyDown("Activate")) {
		CalCellular();
	}
}

void CMap::Render(HDC hDC, float fDeltaTime)
{
	for (int x = 0; x < m_iWidth; x++) {
		for (int y = 0; y < m_iHeight; y++) {
			int dx = m_tPos.x + x * m_iTileSize;
			int dy = m_tPos.y + y * m_iTileSize;
			BitBlt(hDC, dx, dy, dx + m_iTileSize, dy + m_iTileSize, getTile(m_2DMap[y][x])->GetDC(), 0, 0, SRCCOPY);
		}
	}
}

CMap* CMap::Clone()
{
	return new CMap(*this);
}
