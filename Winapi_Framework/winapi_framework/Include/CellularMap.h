#pragma once
#include "Map.h"
#include "Input.h"
#include "Clock.h"
#include <omp.h>

class CellularMap :
    public CMap
{
private:
	float m_fCurTime = 0.0f;
	int m_iSeed = 1;
public:
	CellularMap(HDC hDC, int width, int height, int seed = 1) : CMap(hDC, width, height), m_iSeed(seed) {}

	virtual void Input(float fDeltaTime)
    {
        if (GET_SINGE(CInput)->KeyDown("Activate")) {
			Clock::GetInst()->start();
            CalCellular();
			Clock::GetInst()->end();
			Clock::GetInst()->message();
			DrawMapParallel();
        }
		if (GET_SINGE(CInput)->KeyDown("ActivateParallel")) {
			CalCellularParallel();
			Clock::GetInst()->start();
			DrawMapParallel();
			Clock::GetInst()->end();
			Clock::GetInst()->message();
		}
    }
	virtual int Update(float fDeltaTime) {
		m_fCurTime += fDeltaTime;

		if (m_fCurTime >= 1.0f) {
			m_fCurTime = 0;
			CalCellular();
			DrawMapParallel();
		}
		return 0;
	}

    virtual CMap* Clone() {
        return new CellularMap(*this);
    }
private:
	virtual void GenerateMap() {
		SetupRandomMap();
		DrawMapParallel();
	}

    bool IsMap(int x, int y)
    {
        return x < m_iWidth && y < m_iHeight && x >= 0 && y >= 0;
    }

	void SetupRandomMap() {
		std::random_device rd;
		std::mt19937 gen(m_iSeed);
		std::uniform_int_distribution<int> dis(0, 100);
		int count = 0;

		for (int y = 0; y < m_iHeight; y++)
		{
			for (int x = 0; x < m_iWidth; x++)
			{
				if (dis(gen) > 60)
				{
					m_2DMap[y][x] = TILE_TYPE::LAND;
					count++;
				}
			}
		}
	}
	inline int CountSurround(int x, int y) {
		int counter = 0;
		for (int dy = -1; dy <= 1; dy++)
		{
			for (int dx = -1; dx <= 1; dx++)
			{
				int cur_x = x + dx;
				int cur_y = y + dy;
				if (!IsMap(cur_x, cur_y)) {
					counter++;
				}
				else if (m_2DMap[cur_y][cur_x] == TILE_TYPE::LAND)
				{
					counter++;
				}
			}
		}
		return counter;
	}

	inline int CountSurroundGame(int x, int y) {
		int counter = 0;
		for (int dy = -1; dy <= 1; dy++)
		{
			for (int dx = -1; dx <= 1; dx++)
			{
				int cur_x = x + dx;
				int cur_y = y + dy;

				if (dy == 0 && dx == 0) continue;

				if (!IsMap(cur_x, cur_y)) {
					counter++;
				}
				else if (m_2DMap[cur_y][cur_x] == TILE_TYPE::LAND)
				{
					counter++;
				}
			}
		}
		return counter;
	}

	inline void CellularRule(int x, int y) {
		int counter = CountSurround(x, y);
		if (counter > 4)		m_2DMap[y][x] = TILE_TYPE::LAND;
		else if (counter < 4)	m_2DMap[y][x] = TILE_TYPE::AIR;
	}

	inline void CellularGameRule(int x, int y) {
		int counter = CountSurroundGame(x, y);
		if (counter < 2)
		{
			m_2DMap[y][x] = TILE_TYPE::AIR;
		}
		else if (counter == 3 || counter == 2)
		{
			m_2DMap[y][x] = TILE_TYPE::LAND;
		}
		else if (counter > 3)
		{
			m_2DMap[y][x] = TILE_TYPE::AIR;
		}
	}

	void CalCellular() {
		for (int y = 0; y < m_iHeight; y++)
		{
			for (int x = 0; x < m_iWidth; x++)
			{
				CellularRule(x, y);
			}
		}
	}

	void CalCellularParallel() {
		int thread_count = ThreadManager::GetInst()->getThreadCount();
#pragma omp parallel num_threads(thread_count)
		{
			int n = m_iHeight;
			double d_thread_count = (double)thread_count;
			int thread_id = omp_get_thread_num();
			double d_thread_id = (double)thread_id;

			double d_start_height = d_thread_id * n / d_thread_count;
			int start_height = (int)d_start_height;
			double d_end_height = (thread_count == thread_id - 1) ? n : d_start_height + n / d_thread_count;
			int end_height = (int)d_end_height;
			for (int y = start_height; y < end_height - 1; y++)
			{
				for (int x = 0; x < m_iWidth; x++)
				{
					CellularRule(x, y);
				}
			}
#pragma omp barrier
			int y = end_height - 1;
			for (int x = 0; x < m_iWidth; x++)
			{
				CellularRule(x, y);
			}
		}
	}
};

