#include "Core.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR IpCmdLine,
	_In_ int nCmdShow){

	if (!CCore::GetInst()->Init(hInstance, L"MultiCore", 720, 720)) {
		CCore::DestroyInst();
		return 0;
	}

	int iRev = CCore::GetInst()->Run();

	CCore::DestroyInst();

	return iRev;
}
