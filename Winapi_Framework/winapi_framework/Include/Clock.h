#pragma once
#include <chrono>
#include <Windows.h>
#include <fstream>
#include "Game.h"
#include "ThreadManager.h"

class Clock
{
DECLARE_SINGLE(Clock)
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_pStart;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_pEnd;
public:
	void start() {
		m_pStart = std::chrono::high_resolution_clock::now();
	}
	void end() {
		m_pEnd = std::chrono::high_resolution_clock::now();
	}
	double get() {
		std::chrono::duration<double> duration = m_pEnd - m_pStart;
		return duration.count();
	}
	void writeLog(const string& message, int map_size) {
		std::ofstream file("log.txt", std::ios::app);
		if (!file.is_open()) {
			file.open("example.txt", std::ios::out);
		}
		if (file.is_open()) {
			int thread_count = ThreadManager::GetInst()->getThreadCount();
			file << "[" << message << "] (thread: " << (message == "Serial" ? 1 : thread_count) << ", map_size: " << map_size << ", time: " << get() << ")\n";
			file.close();
		}
		else {
			MessageBox(NULL, L"Cannot Open Log", L"Clock", MB_ICONEXCLAMATION | MB_OK);
		}
	}

	void clearLog() {
		std::ofstream file("example.txt", std::ios::trunc);
		file.close();
	}

	void message(){
		double result = get();
		wchar_t buffer[20];
		swprintf_s(buffer, L"%f", result);
		MessageBox(NULL, (LPCWSTR)buffer, L"Clock", MB_ICONEXCLAMATION | MB_OK);
	}
	void debug(double value) {
		wchar_t buffer[20];
		swprintf_s(buffer, L"%f", value);
		MessageBox(NULL, (LPCWSTR)buffer, L"Debug", MB_ICONEXCLAMATION | MB_OK);
	}
};

