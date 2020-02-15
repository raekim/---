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
	g_pTextureManager->AddTexture(L"Player", L"platformer-pack-redux-360-assets/Spritesheets/spritesheet_players_Cut.png");		// 기존에 없는 이미지 입니다.
	g_pTextureManager->AddTexture(L"FireBall", L"platformer-pack-redux-360-assets/PNG/Particles/fireball_4.png");				// 기존에 없는 이미지 입니다.

	// 캐릭터 기본 생성
	m_pAnimation = new Animation<STATE>;
	m_pBoundBox = new Rect;
	m_pBoundCircle = new Circle;
	m_pOnGroundCheckCircle = new Circle;

	// 발사체 에니메이션 생성
	m_pFire = new Animation<STATE>;

	Clip* clip = new Clip;
	//-----------------------------------Character------------------
	// 플레이어 스테이트 이미지 셋팅			---- 추후 여러 이미지 사용 위해 포문 사용
	{
		// 기본 상태
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
			Sprite* sprite = new Sprite(L"Player", 6, 2, 3);
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
	//-----------------------------------Character------------------

	// Fire 에니메이션
	{
		clip = new Clip;
		for (int i = 0; i < 4; i++)
		{
			Sprite* sprite = new Sprite(L"FireBall", 4, 1, i);
			clip->AddFrame(sprite, 1 / 4.0f);
		}
		m_pFire->AddClip(FIRE, clip);
	}

	this->Init();
}


Character::~Character()
{
	SAFE_DELETE(m_pFire);
	SAFE_DELETE(m_pOnGroundCheckCircle);
	SAFE_DELETE(m_pBoundCircle);
	SAFE_DELETE(m_pBoundBox);
	SAFE_DELETE(m_pAnimation);
}

void Character::Init()
{
	// 캐릭터 기본 상태

	m_eState = IDLE;
	m_pAnimation->SetScale(0.5f, 0.5f);		// 사이즈가 너무 커서 줄임
	m_pAnimation->Play(m_eState);			// 기본상태 플레이
	m_vPosition = D3DXVECTOR2(WINSIZEX * 0.5f, GROUND + m_pAnimation->GetHeight());	// 테스트 위해 임의의 위치 초기화
	m_fSpeed = SPEED;						// 헤더파일에 define으로 관리
	m_fJumpPower = JUMPPOWER;				// 헤더파일에 define으로 관리
	m_fElapsedGravity = 0.0f;				// 누적중력 초기화
	m_isJumping = false;					// 점프 여부 초기화
	m_isOnGorund = true;					// 착지 여부 초기화
	m_isFalling = false;					// 떨어지는중 여부 초기화
	m_isColliderBorder = true;				// 테스트용 BorderLine 여부 초기화

	m_eDir = RIGHT;							// Fire발사 방향 오른쪽으로 초기화

	// Hit Box Init
	{
		m_pBoundBox->SetPosition(m_vPosition.x, m_vPosition.y);	// 캐릭터 중점으로 초기화
		m_pBoundBox->SetSize(m_pAnimation->GetWidth() * 0.5f, m_pAnimation->GetHeight() * 0.5f);	// 캐릭 사이즈를 줄여서 0.5f 곱함
		m_pBoundBox->SetBorder(true);	// 보더라인 킨 상태로 시작

		m_pBoundCircle->SetPosition(m_vPosition.x, m_vPosition.y - m_pAnimation->GetHeight() * 0.5f + 10);	// 캐릭터의 발쪽으로 
		m_pBoundCircle->SetRadius(12.5f);
		m_pBoundCircle->SetBorder(true);
		m_pBoundCircle->SetColor(D3DXCOLOR(1, 1, 1, 0));

		m_pOnGroundCheckCircle->SetPosition(m_vPosition.x, m_vPosition.y - m_pAnimation->GetHeight() * 0.25f);	// m_pBoundCircle 보다 아래쪽으로
		m_pOnGroundCheckCircle->SetRadius(12.5f);
		m_pOnGroundCheckCircle->SetBorder(true);
		m_pOnGroundCheckCircle->SetColor(D3DXCOLOR(1, 1, 1, 0));

	}

	// Fire 초기화----
	m_fFireSpeed = 700.0f;
	m_isShot = false;
	m_pFire->SetPosition(m_vPosition.x, m_vPosition.y + 100);
	m_pFire->Play(FIRE);
	m_vFirePos = { 0, 0 };
	//----------------
}

void Character::Update()
{
	this->Collision();
	this->Conctrol();
	this->Gravity();

	m_pAnimation->Update();
	m_pBoundCircle->Update();
	m_pBoundBox->SetPosition(m_vPosition.x, m_vPosition.y);
	m_pBoundBox->Update();

	if (m_pOnGroundCheckCircle)
	{
		m_pOnGroundCheckCircle->SetPosition(m_vPosition.x, m_vPosition.y - m_pAnimation->GetHeight() * 0.25f);
		m_pOnGroundCheckCircle->Update();
	}
	if (m_isShot)
	{
		this->Shoot();
		m_pFire->SetPosition(m_vFirePos);
		m_pFire->Play(FIRE);
		m_pFire->Update();
	}

	if (m_vPosition.y >= TOPEND)
	{
		m_vPosition.y = TOPEND;
		g_cameraPos.y += m_fSpeed * g_pTimeManager->GetDeltaTime();
	}

	else if (m_vPosition.y <= BOTTOMEND)
	{
		m_vPosition.y = BOTTOMEND;
		g_cameraPos.y -= m_fSpeed * g_pTimeManager->GetDeltaTime();
	}

	ImGui::Checkbox("Character Collider", &m_isColliderBorder);

	m_pBoundBox->SetBorder(m_isColliderBorder);
	m_pBoundCircle->SetBorder(m_isColliderBorder);
	m_pOnGroundCheckCircle->SetBorder(m_isColliderBorder);

}

void Character::Render()
{
	m_pAnimation->Render();
	if (m_isShot)
		m_pFire->Render();
	if (m_pBoundBox)
		m_pBoundBox->Render();
	if (m_pBoundCircle)
		m_pBoundCircle->Render();
	if (m_pOnGroundCheckCircle)
		m_pOnGroundCheckCircle->Render();
}

void Character::Conctrol()
{
	
	// 왼쪽 방향키
	if (g_pKeyManager->isStayKeyDown(VK_LEFT))
	{
		if (m_vPosition.x > LEFTEND)
		{
			m_pAnimation->SetRotation(0, D3DX_PI, 0);
			m_vPosition.x -= m_fSpeed * g_pTimeManager->GetDeltaTime();
			m_eState = RUN;

			// Fire 발사 시 방향 여부 ( 없으면 발사한 후 좌우로 Fire 조작가능해짐)
			if (!m_isShot)
				m_eDir = DIR::LEFT;
		}
		// 왼쪽끝 도달 시 카메라 좌표 움직임.
		else if (m_vPosition.x <= LEFTEND)
		{
			m_vPosition.x = LEFTEND;
			m_pAnimation->SetRotation(0, D3DX_PI, 0);
			g_cameraPos.x -= m_fSpeed * g_pTimeManager->GetDeltaTime();
			m_eState = RUN;
		}
	}

	// 오른쪽 방향키
	else if (g_pKeyManager->isStayKeyDown(VK_RIGHT))
	{
		if (m_vPosition.x < RIGHTEND)
		{
			m_pAnimation->SetRotation(0, 0, 0);
			m_vPosition.x += m_fSpeed * g_pTimeManager->GetDeltaTime();
			m_eState = RUN;

			// Fire 발사 시 방향 여부
			if (!m_isShot)
				m_eDir = DIR::RIGHT;
		}
		// 오른쪽 끝 도달 시 카메라 좌표 움직임.
		else if (m_vPosition.x >= RIGHTEND)
		{
			m_vPosition.x = RIGHTEND;
			m_pAnimation->SetRotation(0, 0, 0);
			m_eState = RUN;
			g_cameraPos.x += m_fSpeed * g_pTimeManager->GetDeltaTime();
		}
	}
	// 아무것도 안누르면 IDLE 상태
	else
		m_eState = IDLE;

	// 점프키
	if (g_pKeyManager->isOnceKeyDown(VK_SPACE) && m_isJumping == false)
	{
		m_isJumping = true;
		m_isOnGorund = false;
		m_isFalling = true;
		m_fElapsedGravity = 0.0f;
	}

	// Fire Shot
	if (g_pKeyManager->isOnceKeyDown('F') && m_isShot == false)
	{
		m_isShot = true;
		m_vFirePos = m_vPosition;
		m_pFire->SetPosition(m_vFirePos);
	}
}

void Character::Shoot()
{
	// m_eDir을 곱하여 한쪽방향으로만 날아가도록
	m_vFirePos.x += m_fFireSpeed * g_pTimeManager->GetDeltaTime() * m_eDir;

	if (m_vFirePos.x > WINSIZEX + m_pFire->GetWidth() || m_vFirePos.x < -m_pFire->GetWidth())
	{
		m_vFirePos.x = -100;
		m_vFirePos.y = -100;
		m_isShot = false;
	}
}

void Character::Collision()
{
	// 이전위치로 되돌리기 위한 써클 검사 ( m_pBoundCircle )===================================
	{
		auto Prepos = m_pBoundCircle->GetPosition();

		if (g_pKeyManager->isStayKeyDown(VK_LEFT))
		{
			Prepos.x -= m_fSpeed * g_pTimeManager->GetDeltaTime();
		}
		else if (g_pKeyManager->isStayKeyDown(VK_RIGHT))
		{
			Prepos.x += m_fSpeed * g_pTimeManager->GetDeltaTime();
		}
		else if (g_pKeyManager->isStayKeyDown(VK_UP))
		{
			Prepos.y += m_fSpeed * g_pTimeManager->GetDeltaTime();
		}
		else if (g_pKeyManager->isStayKeyDown(VK_DOWN))
		{
			Prepos.y -= m_fSpeed * g_pTimeManager->GetDeltaTime();
		}

		// 현재 맵의 타일과 원 collision check
		auto originalCirclePos = m_pBoundCircle->GetPosition(); // 원 이전 위치 저장

		// 바뀐 위치의 원이 현재 맵과 충돌?
		m_pBoundCircle->SetPosition(Prepos);

		bool collide = m_pTileMapManager->CurrMapCircleCollision(m_pBoundCircle);
		
		if (collide)
		{
			m_pBoundCircle->SetPosition(originalCirclePos); // 충돌 이전 위치로 되돌림
			
			// 캐릭터가 사용하는 포지션값 재위치
			m_vPosition.x = m_pBoundCircle->GetPosition().x;
			m_vPosition.y = m_pBoundCircle->GetPosition().y + m_pAnimation->GetHeight() * 0.25f;
		}
	}

	// 바닥 충돌 여부 서클 검사 ( m_pOnGroundCheckCircle )================================
	{
		auto Prepos = m_pOnGroundCheckCircle->GetPosition();

		if (g_pKeyManager->isStayKeyDown(VK_LEFT))
		{
			Prepos.x -= m_fSpeed * g_pTimeManager->GetDeltaTime();
		}
		else if (g_pKeyManager->isStayKeyDown(VK_RIGHT))
		{
			Prepos.x += m_fSpeed * g_pTimeManager->GetDeltaTime();
		}
		else if (g_pKeyManager->isStayKeyDown(VK_UP))
		{
			Prepos.y += m_fSpeed * g_pTimeManager->GetDeltaTime();
		}
		else if (g_pKeyManager->isStayKeyDown(VK_DOWN))
		{
			Prepos.y -= m_fSpeed * g_pTimeManager->GetDeltaTime();
		}

		// 현재 맵의 타일과 원 collision check
		auto originalCirclePos = m_pOnGroundCheckCircle->GetPosition(); // 원 이전 위치 저장

		// 바뀐 위치의 원이 현재 맵과 충돌?
		m_pOnGroundCheckCircle->SetPosition(Prepos);

		bool collide = m_pTileMapManager->CurrMapCircleCollision(m_pOnGroundCheckCircle);
		
		if (collide)
		{
			m_isFalling = false;
			m_isOnGorund = true;
			m_isJumping = false;
			m_fElapsedGravity = 0.0f;
		}
		else
		{
			m_isOnGorund = false;
			m_isJumping = true;

			if (!m_isFalling)
			{
				m_fElapsedGravity = m_fJumpPower;
				m_isFalling = true;
			}
		}
	}
	m_pAnimation->SetPosition(m_vPosition);	// m_vPosition 애니메이션에 셋팅
	m_pAnimation->Play(m_eState);			// 현재 상태 플레이
	m_pBoundCircle->SetPosition(m_vPosition.x, m_vPosition.y - m_pAnimation->GetHeight() * 0.25f + 10);		// 되돌리기용 써클 재위치
	m_pOnGroundCheckCircle->SetPosition(m_vPosition.x, m_vPosition.y - m_pAnimation->GetHeight() * 0.25f);	// 바닥 충돌 여부 써클 재위치
}

void Character::Gravity()
{
	// When a Character Jumping
	if (!m_isOnGorund)
	{
		// JumpState Classification
		if (m_fJumpPower > m_fElapsedGravity)
			m_eState = JUMPUP;
		else
			m_eState = JUMPDOWN;

		// 중력에 따라 올라가고 내려감
		m_vPosition.y += (m_fJumpPower - m_fElapsedGravity) * g_pTimeManager->GetDeltaTime();
		// Increase ElapsedGravity 
		m_fElapsedGravity += GRAVITY * g_pTimeManager->GetDeltaTime();

		// 임시로 바닥을 GROUND로 지정 
		// 타일이 안깔린 맵 바닥으로 추락한다면 점프해버립니다.
		if (m_vPosition.y < GROUND + m_pAnimation->GetHeight() * 0.5f)
		{
			m_vPosition.y = GROUND + m_pAnimation->GetHeight() * 0.5f;
			m_isOnGorund = true;
			m_isJumping = false;
			m_fElapsedGravity = 0.0f;
		}
	}

}
