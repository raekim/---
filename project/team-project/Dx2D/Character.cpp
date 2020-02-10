#include "stdafx.h"
#include "Character.h"

Character::Character()
{
	/*READY = -1,
		IDLE,
		RUN,
		JUMPUP,
		JUMPDOWN,
		HIT,
		DIE,*/
	g_pTextureManager->AddTexture(L"Player", L"platformer-pack-redux-360-assets/Spritesheets/spritesheet_players_Cut.png");

	m_pAnimation = new Animation<STATE>;

	// 플레이어 스테이트 이미지 셋팅			---- 추후 여러 이미지 사용 위해 포문 사용
	{
		// 기본 상태
		Clip* clip = new Clip;
		for (int i = 0; i < 1; i++)
		{
			Sprite* sprite = new Sprite(L"Player", 6, 2, i);
			clip->AddFrame(sprite, 1 / 1.0f);
		}
		m_pAnimation->AddClip(IDLE, clip);

		// 걷기
		clip = new Clip;
		for (int i = 0; i < 2; i++)
		{
			Sprite* sprite = new Sprite(L"Player", 6, 2, i + 1);
			clip->AddFrame(sprite, 1 / 6.0f);
		}
		m_pAnimation->AddClip(RUN, clip);

		// 점프 업 (업 다운 구분만 해놓음. 리소스는 같은 이미지)
		clip = new Clip;
		for (int i = 0; i < 1; i++)
		{
			Sprite* sprite = new Sprite(L"Player", 6, 2, i + 3);
			clip->AddFrame(sprite, 1 / 1.0f);
		}
		m_pAnimation->AddClip(JUMPUP, clip);

		// 점프 다운
		clip = new Clip;
		for (int i = 0; i < 1; i++)
		{
			Sprite* sprite = new Sprite(L"Player", 6, 2, i + 3);
			clip->AddFrame(sprite, 1 / 1.0f);
		}
		m_pAnimation->AddClip(JUMPDOWN, clip);

		// 히트
		clip = new Clip;
		for (int i = 0; i < 1; i++)
		{
			Sprite* sprite = new Sprite(L"Player", 6, 2, i + 4);
			clip->AddFrame(sprite, 1 / 1.0f);
		}
		m_pAnimation->AddClip(HIT, clip);

		// 죽음
		clip = new Clip;
		for (int i = 0; i < 1; i++)
		{
			Sprite* sprite = new Sprite(L"Player", 6, 2, i + 5);
			clip->AddFrame(sprite, 1 / 1.0f);
		}
		m_pAnimation->AddClip(DIE, clip);
	}


	this->Init();
}


Character::~Character()
{
	SAFE_DELETE(m_pAnimation);
}

void Character::Init()
{
	m_eState = IDLE;
	m_pAnimation->Play(m_eState);

	m_vPosition = D3DXVECTOR2(WINSIZEX * 0.5f, GROUND + m_pAnimation->GetHeight() * 0.5f);	// 테스트 위해 임의의 위치 초기화
	m_fSpeed = 100.0f;
	m_fJumpPower = 500.0f;
	m_fElapsedGravity = 0.0f;
	m_isJumping = false;
}

void Character::Update()
{
	this->Conctrol();
	m_pAnimation->SetPosition(m_vPosition);
	m_pAnimation->Play(m_eState);
	m_pAnimation->Update();
}

void Character::Render()
{
	m_pAnimation->Render();
}

void Character::Conctrol()
{
	if (g_pKeyManager->isStayKeyDown(VK_LEFT) && m_vPosition.x + m_pAnimation->GetWidth() * 0.5f > 0)
	{
		m_pAnimation->SetRotation(0, D3DX_PI, 0);
		m_vPosition.x -= m_fSpeed * g_pTimeManager->GetDeltaTime();
		m_eState = RUN;
	}
	else if (g_pKeyManager->isStayKeyDown(VK_RIGHT) && m_vPosition.x < WINSIZEX - m_pAnimation->GetWidth() * 0.5f)
	{
		m_pAnimation->SetRotation(0, 0, 0);
		m_vPosition.x += m_fSpeed * g_pTimeManager->GetDeltaTime();
		m_eState = RUN;
	}
	else
		m_eState = IDLE;

	// 점프키
	if (g_pKeyManager->isOnceKeyDown(VK_SPACE))
	{
		m_isJumping = true;
	}
	if (m_isJumping)
	{
		// 점프 업, 다운 상태 구분
		if (m_fJumpPower > m_fElapsedGravity)
			m_eState = JUMPUP;
		else
			m_eState = JUMPDOWN;
	
		// 중력에 따라 올라가고 내려감
		m_vPosition.y += (m_fJumpPower - m_fElapsedGravity) * g_pTimeManager->GetDeltaTime();
	
		m_fElapsedGravity += GRAVITY * g_pTimeManager->GetDeltaTime();

		// 땅바닥 도달
		if (m_vPosition.y < GROUND + m_pAnimation->GetHeight() * 0.5f)
		{
			m_isJumping = false;
			m_fElapsedGravity = 0.0;
			m_vPosition.y = GROUND + m_pAnimation->GetHeight() * 0.5f;
		}
	}
}