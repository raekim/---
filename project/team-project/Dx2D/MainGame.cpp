#include "stdafx.h"
#include "MainGame.h"


MainGame::MainGame()
{
	srand(time(NULL));

	SetViewProjectionMatrixes();
	SetBlendStates();
	CreateGameClasses();

	// 타일 이미 경계선 없애기 위한 샘플러 스테이트 설정
	// 참고 : https://gamedev.stackexchange.com/questions/25117/why-are-there-lines-in-between-my-tiles
	{
		// 샘플러 스테이트 만들기
		D3D11_SAMPLER_DESC  desc ;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.MinLOD = -FLT_MAX;
		desc.MaxLOD = FLT_MAX;
		desc.MipLODBias = 0.0f;
		desc.MaxAnisotropy = 1;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.BorderColor[0] = 1.0f;
		desc.BorderColor[1] = 1.0f;
		desc.BorderColor[2] = 1.0f;
		desc.BorderColor[3] = 1.0f;

		Device->CreateSamplerState(&desc, &samplerState);

		// 샘플러 스테이트 설정
		DeviceContext->VSSetSamplers(0, 1, &samplerState);
		DeviceContext->GSSetSamplers(0, 1, &samplerState);
		DeviceContext->PSSetSamplers(0, 1, &samplerState);
	}

	this->Init();
}

void MainGame::SetViewProjectionMatrixes()
{
	ViewProjectionDesc VPDesc;

	// 뷰 매트릭스 셋팅
	D3DXVECTOR3 eye(0, 0, -1);
	D3DXVECTOR3 lookAt(0, 0, 0);
	D3DXVECTOR3 up(0, 1, 0);
	D3DXMatrixLookAtLH(&VPDesc.View, &eye, &lookAt, &up);
	D3DXMatrixTranspose(&VPDesc.View, &VPDesc.View);
	// 셰이더에서 매트릭스의 행과 열의 방향이 바껴있기 때문에 트랜스포즈를 해주어야 한다.

	// 프로젝션 매트릭스 셋팅
	D3DXMatrixOrthoOffCenterLH(&VPDesc.Projection, 0, (float)WINSIZEX, 0, (float)WINSIZEY, -1, 1);
	D3DXMatrixTranspose(&VPDesc.Projection, &VPDesc.Projection);

	// 버퍼 생성
	D3D11_BUFFER_DESC desc = { 0, };
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(ViewProjectionDesc);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// 버퍼에 담을 데이터
	D3D11_SUBRESOURCE_DATA data = { 0, };
	data.pSysMem = &VPDesc;

	HRESULT hr = Device->CreateBuffer(&desc, &data, &m_pViewProjBuffer);
}

void MainGame::SetBlendStates()
{
	D3D11_BLEND_DESC desc = { 0, };

	desc.RenderTarget[0].BlendEnable = false;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	Device->CreateBlendState(&desc, &m_pNormalBlendState);

	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	Device->CreateBlendState(&desc, &m_pAlphaBlendState);
}

MainGame::~MainGame()
{
	this->Release();
}

void MainGame::Init()
{
	m_tileMapManager->Init();
	m_tileMapManager->LoadMap();
	g_cameraPos = { 0,0 };	// 카메라 위치 원위치로 세팅

	m_testCircle->Init();
	m_testCircle->SetPosition(WINSIZEX*0.5f, WINSIZEY*0.5f - 100);
	m_testCircle->SetColor({ 1,0,0,1 });
}

void MainGame::CreateGameClasses()
{
	m_tileMapManager = new TileMapManager;

	// 원 충돌 테스트용 원
	m_testCircle = new Circle;

	// 백그라운드 이미지 생성 및 크기조절, 초기화
	{
		g_pTextureManager->AddTexture(L"background-1", L"platformer-pack-redux-360-assets/PNG/Backgrounds/blue_desert.png");
		m_backgroundImg = new Sprite(L"background-1", 1, 1, 0);
		m_backgroundImg->Init();
		m_backgroundImg->SetConstantScale({ WINSIZEX ,WINSIZEY });
	}
}

void MainGame::DeleteGameClasses()
{
	//SAFE_DELETE
}

void MainGame::Update()
{
	if (g_pKeyManager->isOnceKeyDown(VK_F9)) // 게임 초기화
	{
		this->Init();
	}

	ImGui::Checkbox("Draw Map Colliders", &g_isDrawCollider);

	// 넘버패드로 카메라 움직임
	// 임시 코드임... 추후 카메라는 캐릭터 이동에 따라 움직이도록 변경
	{
		static float camDiffX = 0;
		static float speed = 500;	// 카메라 스피드
		if (g_pKeyManager->isStayKeyDown(VK_NUMPAD4))
		{
			camDiffX -= speed * g_pTimeManager->GetDeltaTime();
		}
		else if (g_pKeyManager->isStayKeyDown(VK_NUMPAD6))
		{
			camDiffX += speed * g_pTimeManager->GetDeltaTime();
		}
		else if (g_pKeyManager->isStayKeyDown(VK_NUMPAD8))
		{
			g_cameraPos.y += speed * g_pTimeManager->GetDeltaTime();
		}
		else if (g_pKeyManager->isStayKeyDown(VK_NUMPAD2))
		{
			g_cameraPos.y -= speed * g_pTimeManager->GetDeltaTime();
		}
		if (abs(camDiffX) >= 1)
		{
			g_cameraPos.x += (int)camDiffX;
			camDiffX = 0;
		}
	}

	m_tileMapManager->Update();

	// 테스트용 원 움직임
	{
		auto circlePos = m_testCircle->GetPosition();
		static float speed = 500;	// 스피드
		if (g_pKeyManager->isStayKeyDown('A'))
		{
			circlePos.x -= speed * g_pTimeManager->GetDeltaTime();
		}
		else if (g_pKeyManager->isStayKeyDown('D'))
		{
			circlePos.x += speed * g_pTimeManager->GetDeltaTime();
		}
		else if (g_pKeyManager->isStayKeyDown('W'))
		{
			circlePos.y += speed * g_pTimeManager->GetDeltaTime();
		}
		else if (g_pKeyManager->isStayKeyDown('S'))
		{
			circlePos.y -= speed * g_pTimeManager->GetDeltaTime();
		}
	
		// 현재 맵의 타일과 원 collision check
		auto originalCirclePos = m_testCircle->GetPosition(); // 원 이전 위치 저장
		
		// 바뀐 위치의 원이 현재 맵과 충돌?
		m_testCircle->SetPosition(circlePos);
		bool collide = m_tileMapManager->CurrMapCircleCollision(m_testCircle);
		if(collide) m_testCircle->SetPosition(originalCirclePos); // 충돌 이전 위치로 되돌림

		m_testCircle->Update();

		// 백그라운드 이미지 업데이트
		{
			m_backgroundImg->Update();
		}
	}
}

void MainGame::Render()
{

	D3DXCOLOR background = D3DXCOLOR(0, 0, 0, 1);
	DeviceContext->ClearRenderTargetView(RTV, (float*)background);
	DeviceContext->VSSetConstantBuffers(0, 1, &m_pViewProjBuffer);

	DeviceContext->OMSetBlendState(m_pAlphaBlendState, NULL, 0xFF);	// 반투명 사용 설정

	//DeviceContext->VSGetSamplers(0, 1, &samplerState);

	

	// Render
	m_backgroundImg->Render();	// 배경 맨 처음 업데이트

	m_tileMapManager->Render();

	m_testCircle->Render();

	DeviceContext->OMSetBlendState(m_pNormalBlendState, NULL, 0xFF); // 반투명 미사용(기본값) 설정

	ImGui::Render();

	SwapChain->Present(0, 0);
}

void MainGame::Release()
{
}