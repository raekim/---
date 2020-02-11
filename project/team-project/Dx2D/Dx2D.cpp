// Dx2D.cpp: ?묒슜 ?꾨줈洹몃옩??吏꾩엯?먯쓣 ?뺤쓽?⑸땲??
//

#include "stdafx.h"
#include "Dx2D.h"
#include "MainGame.h"

#define MAX_LOADSTRING 100

// ?꾩뿭 蹂??
HINSTANCE	g_hInst;                            // ?꾩옱 ?몄뒪?댁뒪?낅땲??
HWND		g_hWnd;								// ?덈룄???몃뱾
POINT		g_ptMouse;
D3DXVECTOR2 g_cameraPos;
bool		g_isDrawBorder;
bool		g_mouseLButtonDown;	// 留덉슦???쇱そ踰꾪듉 ?대┃

WCHAR		szTitle[MAX_LOADSTRING] = L"team-project";    // ?쒕ぉ ?쒖떆以??띿뒪?몄엯?덈떎.
MainGame*	pMG;
POINT		ptWinSize;

// Dx11 ?꾩뿭 蹂??
IDXGISwapChain*			SwapChain;		// ?뚮뜑 踰꾪띁(紐⑤뱺 ?덈룄?? 愿由?蹂??: ?꾨젅?좏듃
ID3D11Device*			Device;			// ?섎굹???μ튂(李??????踰꾪띁, ?띿뒪爾????앹꽦 愿由щ? ?꾪븳 ?명꽣?섏씠??(CPU)
ID3D11DeviceContext*	DeviceContext;	// ?앹꽦 ??由ъ냼?ㅻ? 愿由ы븯湲??꾪븳 ?명꽣?섏씠??(GPU -> ?띿뒪爾??뚮뜑留?
ID3D11RenderTargetView*	RTV;			// ?뚮뜑?寃?吏???ъ씤??

// ??肄붾뱶 紐⑤뱢???ㅼ뼱 ?덈뒗 ?⑥닔???뺣갑???좎뼵?낅땲??
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
	MyRegisterClass(hInstance);			// ?대옒???깅줉 : ?꾨줈洹몃옩 ?깅줉
	InitInstance(hInstance, nCmdShow);	// ?깅줉 ???꾨줈洹몃옩 珥덇린??
	InitDirectX(hInstance);				// ?ㅼ씠?됲듃X 珥덇린??

	// 留ㅻ땲??珥덇린??
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

	// 留ㅻ땲???댁젣
	g_pTextureManager->ReleaseInstance();
	g_pTimeManager->ReleaseInstance();
	g_pKeyManager->ReleaseInstance();
	//5g_pSoundManager->Update();

    return (int) msg.wParam;
}



//
//  ?⑥닔: MyRegisterClass()
//
//  紐⑹쟻: 李??대옒?ㅻ? ?깅줉?⑸땲??
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DX2D));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
    wcex.lpszClassName  = szTitle;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
//   ?⑥닔: InitInstance(HINSTANCE, int)
//
//   紐⑹쟻: ?몄뒪?댁뒪 ?몃뱾????ν븯怨?二?李쎌쓣 留뚮벊?덈떎.
//
//   ?ㅻ챸:
//
//        ???⑥닔瑜??듯빐 ?몄뒪?댁뒪 ?몃뱾???꾩뿭 蹂?섏뿉 ??ν븯怨?
//        二??꾨줈洹몃옩 李쎌쓣 留뚮뱺 ?ㅼ쓬 ?쒖떆?⑸땲??
//
void InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   g_hInst = hInstance; // ?몄뒪?댁뒪 ?몃뱾???꾩뿭 蹂?섏뿉 ??ν빀?덈떎.

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

   assert(hWnd);	// 李??앹꽦??臾몄젣媛 ?덈뒗 寃쎌슦 ?꾨줈洹몃옩??醫낅즺 ?섍퀬 ?ㅻ쪟硫붿떆吏媛 ?щ떎.

   g_hWnd = hWnd;

   // 李??ъ씠利??ъ꽕??
   RECT rt = { nWinPosX, nWinPosY, nWinPosX + WINSIZEX + 1, nWinPosY + WINSIZEY + 1};
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
		DXGI_MODE_DESC bufferDesc; // DXGI : ?ㅼ씠?됲듃X 洹몃옒???명꽣?섏씠??
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

		// 諛깅쾭?쇱쓽 ?뚮뜑?寃?酉??앹꽦
		hr = Device->CreateRenderTargetView(BackBuffer, NULL, &RTV);
		assert(SUCCEEDED(hr));
		BackBuffer->Release();

		// ?뚮뜑?寃?酉곕? Output-Merger???뚮뜑 ?寃잛쑝濡??ㅼ젙
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
//  ?⑥닔: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  紐⑹쟻:  二?李쎌쓽 硫붿떆吏瑜?泥섎━?⑸땲??
//
//  WM_COMMAND  - ?묒슜 ?꾨줈洹몃옩 硫붾돱瑜?泥섎━?⑸땲??
//  WM_PAINT    - 二?李쎌쓣 洹몃┰?덈떎.
//  WM_DESTROY  - 醫낅즺 硫붿떆吏瑜?寃뚯떆?섍퀬 諛섑솚?⑸땲??
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
	case WM_ACTIVATE:	// 李쎌쓽 ?쒖꽦?? 鍮꾪솢?깊솕 ?곹깭媛믪씠 諛붾????몄텧
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