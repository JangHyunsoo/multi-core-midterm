#include "Core.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "PathManager.h"
#include "Timer.h"
#include "Input.h"
#include "Obj.h"

DEFINITION_SINGLE(CCore)
bool CCore::m_bLoop = true;

CCore::CCore()
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(214);
}

CCore::~CCore()
{
	DESTROY_SINGE(CSceneManager);
	DESTROY_SINGE(CInput);
	DESTROY_SINGE(CPathManager);
	DESTROY_SINGE(CResourceManager);
	DESTROY_SINGE(CTimer);

	ReleaseDC(m_hWnd, m_hDC);
}

bool CCore::Init(HINSTANCE hInst, const wchar_t* strName, int iWidth, int iHeight)
{
	this->m_hInst = hInst;

	m_strName = strName;
	m_tRS.iW = iWidth;
	m_tRS.iH = iHeight;

	MyRegisterClass();

	Create();

	m_hDC = GetDC(m_hWnd); // 그림그릴때

	if (!GET_SINGE(CTimer)->Init())
		return false;

	if (!GET_SINGE(CPathManager)->Init())
		return false;

	if (!GET_SINGE(CResourceManager)->Init(m_hInst, m_hDC))
		return false;

	if (!GET_SINGE(CInput)->Init(m_hWnd))
		return false;

	if (!GET_SINGE(CSceneManager)->Init())
		return false;

	return true;
}

BOOL CCore::Create()
{
	m_hWnd = CreateWindowW(m_strName, m_strName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInst, nullptr);

	if (!m_hWnd) {
		return FALSE;
	}

	RECT rc = { 0, 0, (LONG)m_tRS.iW, (LONG)m_tRS.iH };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	SetWindowPos(m_hWnd, HWND_TOPMOST, 100, 100, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return 0;
}


int CCore::Run()
{
	MSG msg;

	while (m_bLoop)
	{

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Logic();
		}
	}

	return (int)msg.wParam;
}

void CCore::Logic()
{
	GET_SINGE(CTimer)->Update();

	float fDeltaTime = GET_SINGE(CTimer)->GetDeltaTime();

	Input(fDeltaTime);
	Update(fDeltaTime);
	LateUpdate(fDeltaTime);
	Collision(fDeltaTime);
	Render(fDeltaTime);

}

void CCore::Input(float fDeltaTime)
{
	GET_SINGE(CInput)->Update(fDeltaTime);
	GET_SINGE(CSceneManager)->Input(fDeltaTime);
}

int CCore::Update(float fDeltaTime)
{
	GET_SINGE(CSceneManager)->Update(fDeltaTime);
	return 0;
}

int CCore::LateUpdate(float fDeltaTime)
{
	GET_SINGE(CSceneManager)->LateUpdate(fDeltaTime);
	return 0;
}

void CCore::Collision(float fDeltaTime)
{
	GET_SINGE(CSceneManager)->Collision(fDeltaTime);
}

void CCore::Render(float fDeltaTime)
{
	CTexture* pBackBuffer = GET_SINGE(CResourceManager)->GetBackBuffer();

	Rectangle(pBackBuffer->GetDC(), 0, 0, 1280, 1280);
	GET_SINGE(CSceneManager)->Render(pBackBuffer->GetDC(), fDeltaTime);

	BitBlt(m_hDC, 0, 0, m_tRS.iW, m_tRS.iH, pBackBuffer->GetDC(), 0, 0, SRCCOPY);

	SAFE_RELEASE(pBackBuffer);
}

LRESULT CCore::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		m_bLoop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}


ATOM CCore::MyRegisterClass() {
	WNDCLASSEX wcex; // 윈도우의 정보를 가지고 있음.

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW || CS_VREDRAW;
	wcex.lpfnWndProc = CCore::WndProc;
	wcex.cbClsExtra = 0; 
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = m_strName;
	wcex.hIconSm = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassExW(&wcex);
}