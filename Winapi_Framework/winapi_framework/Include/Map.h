#pragma once
#include "StaticObj.h"
#include "Flag.h"
#include "Texture.h"
#include "ThreadManager.h"
#include "Clock.h"
#include <vector>
#include <omp.h>
#include "SceneManager.h"
#include "Input.h"
#include "InGameScene.h"

using Array2D = std::vector<std::vector<TILE_TYPE>>;

struct ParallelMemDC
{
	HDC hMemDC;
	HBITMAP hMemBitmap;
	int iStart;
	int iEnd;
};

class CMap :
	public CStaticObj
{
public:
	CMap();
	CMap(HDC hDC, int width, int height, bool parallel);
	CMap(const CMap& obj);
	~CMap();
protected:
	const int m_iHeight = 100;
	const int m_iWidth = 100;
	const int m_iTileSize = 4;
	bool m_bParallel = false;
	Array2D m_2DMap;
	HDC m_hMemDC;
	HBITMAP m_hMemBitmap;
	vector<ParallelMemDC> m_vecMemDC;
	HBRUSH m_pWhite;
	HBRUSH m_pAir;
	HBRUSH m_pLand;
	HBRUSH m_pStone;
	HBRUSH m_pWater;
public:
	void SetCell(int x, int y, TILE_TYPE bValue);
	TILE_TYPE GetCell(int x, int y);
protected:
	bool IsMap(int x, int y);
	HBRUSH& getTile(TILE_TYPE tile_type);
	void InitTexture();
	virtual void GenerateMap() = 0;
	void DrawMap() {
		if (m_bParallel)
		{
			DrawMapParallel();
		}
		else {
			DrawMapSerial();
		}
	}
	void DrawMapSerial()
	{
		for (int x = 0; x < m_iWidth; x++) {
			for (int y = 0; y < m_iHeight; y++) {
				int dx = m_tPos.x + x * m_iTileSize;
				int dy = m_tPos.y + y * m_iTileSize;
				SelectObject(m_hMemDC, getTile(m_2DMap[y][x]));
				Rectangle(m_hMemDC, dx, dy, dx + m_iTileSize, dy + m_iTileSize);
			}
		}

		SelectObject(m_hMemDC, m_pAir);
	}
	void DrawMapParallel() {
		int thread_count = ThreadManager::GetInst()->getThreadCount();
#pragma omp parallel num_threads(thread_count)
		{
			int n = m_iHeight;
			double d_thread_count = (double)thread_count;
			int thread_id = omp_get_thread_num();
			double d_thread_id = (double)thread_id;

			double d_start_height = d_thread_id * n / d_thread_count;
			int start_height = (int)d_start_height;
			double d_end_height = (thread_count - 1 == thread_id) ? n : d_start_height + n / d_thread_count;
			int end_height = (int)d_end_height;

			for (int y = 0; y < end_height - start_height; y++)
			{
				for (int x = 0; x < m_iWidth; x++)
				{
					int dx = x * m_iTileSize;
					int dy = y * m_iTileSize;
					SelectObject(m_vecMemDC[thread_id].hMemDC, getTile(m_2DMap[y + start_height][x]));
					Rectangle(m_vecMemDC[thread_id].hMemDC, dx, dy, dx + m_iTileSize, dy + m_iTileSize);
				}
			}
		}
			
		for (int tID = 0; tID < thread_count; tID++)
		{
			BitBlt(m_hMemDC, 0, m_vecMemDC[tID].iStart * m_iTileSize, m_iWidth * m_iTileSize, (m_vecMemDC[tID].iEnd - m_vecMemDC[tID].iStart) * m_iTileSize, m_vecMemDC[tID].hMemDC, 0, 0, SRCCOPY);
		}

		SelectObject(m_hMemDC, m_pAir);
	}
	void InitParallel() {
		int thread_count = ThreadManager::GetInst()->getThreadCount();

		for (int tID = 0; tID < thread_count; tID++)
		{
			int n = m_iHeight;
			double d_thread_count = (double)thread_count;
			int thread_id = tID;
			double d_thread_id = (double)thread_id;

			double d_start_height = d_thread_id * n / d_thread_count;
			int start_height = (int)d_start_height;
			double d_end_height = (thread_count - 1 == thread_id) ? n : d_start_height + n / d_thread_count;
			int end_height = (int)d_end_height;

			auto subDC = CreateSubMemDC(m_hMemDC, m_iWidth, start_height, end_height);
			m_vecMemDC.push_back(subDC);
		}
	}
	ParallelMemDC CreateSubMemDC(HDC parentDC, int width, int start, int end) {
		ParallelMemDC subDC;
		subDC.iStart = start;
		subDC.iEnd = end;

		subDC.hMemDC = CreateCompatibleDC(parentDC);
		subDC.hMemBitmap = CreateCompatibleBitmap(parentDC, width * m_iTileSize, (end - start) * m_iTileSize);
		SelectObject(subDC.hMemDC, subDC.hMemBitmap);

		return subDC;
	}
	void clearMemDC() {
		DeleteObject(m_hMemBitmap);
		DeleteDC(m_hMemDC);
		for (auto item : m_vecMemDC) {
			DeleteObject(item.hMemBitmap);
			DeleteDC(item.hMemDC);
		}
	}
	void clearMap() {
		m_2DMap.clear();
	}
	
public:
	void clear() {
		clearMemDC();
		clearMap();
	}
	virtual bool Init();
	virtual void Render(HDC hDC, float fDeltaTime);
};