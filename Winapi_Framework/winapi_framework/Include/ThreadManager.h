#pragma once
#include "Game.h"

class ThreadManager
{
	DECLARE_SINGLE(ThreadManager)
private:
	int m_iThreadCount = 4;
public:
	void setThreadCount(int thread_count) {
		m_iThreadCount = thread_count;
	}
	int getThreadCount() {
		return m_iThreadCount;
	}
};

