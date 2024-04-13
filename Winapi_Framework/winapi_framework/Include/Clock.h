#pragma once
#include <chrono>
#include <Windows.h>
#include "Game.h"

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

