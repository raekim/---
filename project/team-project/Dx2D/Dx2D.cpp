// Dx2D.cpp: 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Dx2D.h"
#include "MainGame.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE	g_hInst;                            // 현재 인스턴스입니다.
HWND		g_hWnd;								// 윈도우 핸들
POINT		g_ptMouse;
D3DXVECTOR2 g_cameraPos;	// 카메라 포지션
bool		g_isDrawBorder;
bool		g_mouseLButtonDown;	// 마우스 왼쪽버튼 클릭
bool		g_isDrawCollider;

WCHAR		szTitle[MAX_LOADSTRING] = L"team-project";    // 제목 표시줄 텍스트입니다.
MainGame*	pMG;
POINT		ptWinSize;

// Dx11 전역 변수:
IDXGISwapChain*			SwapChain;		// 렌더 버퍼(모든 윈도우) 관리 변수 : 프레젠트
ID3D11Device*			Device;			// 하나의 장치(창)에 대한 버퍼, 텍스쳐 등 생성 관리를 위한 인터페이스 (CPU)
ID3D11DeviceContext*	DeviceContext;	// 생성 된 리소스를 관리하기 위한 인터페이스 (GPU -> 텍스쳐 렌더링)
ID3D11RenderTargetView*	RTV;			// 렌더타겟 지정 포인터

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
void                InitInstance(HINSTANCE, int);
void				InitDirectX(HINSTANCE);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	MyRegisterClass(hInstance);			// 클래스 등록 : 프로그램 등록
	InitInstance(hInstance, nCmdShow);	// 등록 된 프로그램 초기화
	InitDirectX(hInstance);				// 다이렉트X 초기화

	// 매니져 초기화
	g_pKeyManager->Init();
	g_pTimeManager->Init();
	//g_pSoundManager->Init();
	g_pTextManager->Init();

	pMG = new MainGame;
	//pMG->Init();

	ImGui::Create(g_hWnd, Device, DeviceContext);
	ImGui::StyleColorsDark();

	MSG msg;

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			ImGui::Update();
			g_pTimeManager->Update(60.0f);
			g_pTimeManager->Render();

			pMG->Update();
			pMG->Render();
		}
	}

	ImGui::Delete();
	delete pMG;

	// 매니져 해제
	g_pTextureManager->ReleaseInstance();
	g_pTimeManager->ReleaseInstance();
	g_pKeyManager->ReleaseInstance();
	//5g_pSoundManager->Update();

	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DX2D));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szTitle;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

void ActiveClipCursor()
{
	RECT rt;

	GetWindowRect(g_hWnd, &rt);
	rt.left += 8;
	rt.right -= 8;
	rt.bottom -= 8;
	rt.top = rt.bottom - WINSIZEY - 1;
	ClipCursor(&rt);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
void InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	int nRX = GetSystemMetrics(SM_CXSCREEN);
	int nRY = GetSystemMetrics(SM_CYSCREEN);

	int nWinPosX = nRX * 0.5f - WINSIZEX * 0.5f;
	int nWinPosY = nRY * 0.5f - WINSIZEY * 0.5f - 40;

	HWND hWnd = CreateWindowW(
		szTitle,
		szTitle,
		WS_OVERLAPPED,
		nWinPosX,
		nWinPosY,
		WINSIZEX,
		WINSIZEY,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	assert(hWnd);	// 창 생성에 문제가 있는 경우 프로그램이 종료 되고 오류메시지가 뜬다.

	g_hWnd = hWnd;

	// 창 사이즈 재설정
	RECT rt = { nWinPosX, nWinPosY, nWinPosX + WINSIZEX + 1, nWinPosY + WINSIZEY + 1 };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);

	ptWinSize.x = rt.right - rt.left;
	ptWinSize.y = rt.bottom - rt.top;

	MoveWindow(g_hWnd, nWinPosX, nWinPosY, ptWinSize.x, ptWinSize.y, true);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	ActiveClipCursor();
}

void InitDirectX(HINSTANCE)
{
	// Create Device and DeviceContext, SwapChain
	{
		DXGI_MODE_DESC bufferDesc; // DXGI : 다이렉트X 그래픽 인터페이스
		ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

		bufferDesc.Width = WINSIZEX;
		bufferDesc.Height = WINSIZEY;
		bufferDesc.RefreshRate.Numerator = 60;
		bufferDesc.RefreshRate.Denominator = 1;
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		//==================================================================

		DXGI_SWAP_CHAIN_DESC desc;
		ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

		desc.BufferDesc = bufferDesc;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 1;
		desc.OutputWindow = g_hWnd;
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		HRESULT hr = D3D11CreateDeviceAndSwapChain
		(
			NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
			D3D11_SDK_VERSION, &desc, &SwapChain, &Device, NULL, &DeviceContext
		);
		assert(SUCCEEDED(hr));
	}

	// Create BackBuffer
	{
		HRESULT hr;

		ID3D11Texture2D* BackBuffer;
		hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);
		assert(SUCCEEDED(hr));

		// 백버퍼의 렌더타겟 뷰 생성
		hr = Device->CreateRenderTargetView(BackBuffer, NULL, &RTV);
		assert(SUCCEEDED(hr));
		BackBuffer->Release();

		// 렌더타겟 뷰를 Output-Merger의 렌더 타겟으로 설정
		DeviceContext->OMSetRenderTargets(1, &RTV, NULL);
	}

	// Create Viewport
	{
		D3D11_VIEWPORT viewport = { 0, };

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = WINSIZEX;
		viewport.Height = WINSIZEY;

		DeviceContext->RSSetViewports(1, &viewport);
	}

	// Disable CullMode
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));

		desc.CullMode = D3D11_CULL_NONE;
		desc.FillMode = D3D11_FILL_SOLID;

		ID3D11RasterizerState* rasterState;
		Device->CreateRasterizerState(&desc, &rasterState);

		DeviceContext->RSSetState(rasterState);
		SAFE_RELEASE(rasterState);
	}
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui::WndProc(hWnd, message, wParam, lParam))
		return true;

	static float timeScale = g_pTimeManager->GetTimeScale();

	switch (message)
	{
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMaxTrackSize.x = ptWinSize.x;
		((MINMAXINFO*)lParam)->ptMaxTrackSize.y = ptWinSize.y;
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = ptWinSize.x;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = ptWinSize.y;
		break;
	case WM_MOUSEMOVE:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = WINSIZEY - HIWORD(lParam);
		break;
	case WM_ACTIVATE:	// 창의 활성화, 비활성화 상태값이 바뀔 때 호출
		if (LOWORD(wParam) == WA_INACTIVE)
			g_pTimeManager->SetTimeScale(0.0f);
		break;
	case WM_LBUTTONDOWN:
		g_mouseLButtonDown = true;
		if (g_pTimeManager->GetTimeScale() == 0.0f && LOWORD(wParam) == WA_ACTIVE)
		{
			g_pTimeManager->SetTimeScale(timeScale);
			ActiveClipCursor();
		}
		break;
	case WM_LBUTTONUP:
		g_mouseLButtonDown = false;
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_F11:
			if (g_pTimeManager->GetTimeScale() != 0.0f)
			{
				g_pTimeManager->SetTimeScale(0.0f);
				ClipCursor(NULL);
			}
			else
			{
				g_pTimeManager->SetTimeScale(timeScale);
				ActiveClipCursor();
			}
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}