#pragma once
#include "Map.h"
#include "PerlinNoise.h"
#include "Input.h"
#include "Clock.h"
#include <omp.h>

class PerlinNoiseMap :
	public CMap
{
private:
	PerlinNoise m_cPerlinNoise;
public:
	PerlinNoiseMap(HDC hDC, int width, int height, bool parallel, int seed = -1) : CMap(hDC, width, height, parallel) {
		m_cPerlinNoise = PerlinNoise(seed);
	}
	virtual PerlinNoiseMap* Clone() {
		return new PerlinNoiseMap(*this);
	}
private:
	void GenerateMap() {
		Clock::GetInst()->start();
		setUpMapParallel();
		DrawMap();
		Clock::GetInst()->end();
		Clock::GetInst()->message();
	}
	void setUpMap() {
		std::vector<std::vector<double>> perlin_value(m_iHeight, vector<double>(m_iWidth, 0));
		const int GRID_SIZE = 50;

		for (int x = 0; x < m_iWidth; x++)
		{
			for (int y = 0; y < m_iHeight; y++)
			{
				int index = (y * m_iWidth + x) * 4;
				float val = 0;

				float freq = 1;
				float amp = 1;

				for (int i = 0; i < 12; i++)
				{
					val += m_cPerlinNoise.perlin(x * freq / GRID_SIZE, y * freq / GRID_SIZE) * amp;

					freq *= 2;
					amp /= 2;

				}

				val *= 1.2;

				if (val > 1.0f)
					val = 1.0f;
				else if (val < -1.0f)
					val = -1.0f;

				int result = (int)(((val + 1.0f) * 0.5f) * 255);


				if (result > 122) {
					m_2DMap[y][x] = TILE_TYPE::LAND;
				}
				else {
					m_2DMap[y][x] = TILE_TYPE::AIR;
				};
			}
		}
	}

	void setUpMapParallel() {
		std::vector<std::vector<double>> perlin_value(m_iHeight, vector<double>(m_iWidth, 0));
		const int GRID_SIZE = 50;
		int thread_count = ThreadManager::GetInst()->getThreadCount();
#pragma omp parallel num_threads(thread_count)
		{
			int n = m_iHeight;
			int thread_count = omp_get_num_threads();
			double d_thread_count = (double)thread_count;
			int thread_id = omp_get_thread_num();
			double d_thread_id = (double)thread_id;

			double d_start_height = d_thread_id * n / d_thread_count;
			int start_height = (int)d_start_height;
			double d_end_height = (thread_count == thread_id - 1) ? n : d_start_height + n / d_thread_count;
			int end_height = (int)d_end_height;
			for (int y = start_height; y < end_height; y++)
			{
				for (int x = 0; x < m_iWidth; x++)
				{
					int index = (y * m_iWidth + x) * 4;
					float val = 0;

					float freq = 1;
					float amp = 1;

					for (int i = 0; i < 12; i++)
					{
						val += m_cPerlinNoise.perlin(x * freq / GRID_SIZE, y * freq / GRID_SIZE) * amp;

						freq *= 2;
						amp /= 2;

					}

					val *= 1.2;

					if (val > 1.0f)
						val = 1.0f;
					else if (val < -1.0f)
						val = -1.0f;

					int result = (int)(((val + 1.0f) * 0.5f) * 255);

					if (result > 122) {
						m_2DMap[y][x] = TILE_TYPE::LAND;
					}
					else {
						m_2DMap[y][x] = TILE_TYPE::AIR;
					};
				}
			}
		}
	}
};