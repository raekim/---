#include "stdafx.h"
#include "Player.h"
#include "Map.h"
#include "MapObject.h"


Player::Player()
{
	g_pTextureManager->AddTexture(L"Player", L"Player.png");

	m_pAnimation = new Animation<STATE>;

	// 대기 동작 추가
	Clip* clip = new Clip;
	for (int i = 0; i < 12; i++)
	{
		Sprite* sprite = new Sprite(L"Player", 13, 10, i);
		clip->AddFrame(sprite, 1 / 12.0f);
	}
	m_pAnimation->AddClip(IDLE, clip);

	// 달리기 동작 추가
	clip = new Clip;
	for (int i = 0; i < 8; i++)
	{
		Sprite* sprite = new Sprite(L"Player", 13, 10, 13 * 6 + i);
		clip->AddFrame(sprite, 1 / 16.0f);
	}
	m_pAnimation->AddClip(RUN, clip);

	// 점프 업 동작 추가
	clip = new Clip;
	for (int i = 0; i < 4; i++)
	{
		Sprite* sprite = new Sprite(L"Player", 13, 10, 13 * 2 + i);
		clip->AddFrame(sprite, 1 / 4.0f);
	}
	m_pAnimation->AddClip(JUMPUP, clip);

	// 점프 다운 동작 추가
	clip = new Clip;
	for (int i = 0; i < 4; i++)
	{
		Sprite* sprite = new Sprite(L"Player", 13, 10, 13 * 4 + i);
		clip->AddFrame(sprite, 1 / 4.0f);
	}
	m_pAnimation->AddClip(JUMPDOWN, clip);

	// 죽는 동작 추가
	clip = new Clip;
	for (int i = 0; i < 13; i++)
	{
		Sprite* sprite = new Sprite(L"Player", 13, 10, 13 * 8 + i);
		clip->AddFrame(sprite, 1 / 13.0f);
	}
	m_pAnimation->AddClip(DIE, clip);

	m_pRect = new Rect;


}


Player::~Player()
{
	delete m_pAnimation;
	delete m_pRect;
}

void Player::Init()
{
	m_eState = IDLE;
	m_pAnimation->Play(m_eState);

	m_vPosition = D3DXVECTOR2(DEFAULT_X, m_pMap->GetGroundY());
	m_vRotaion = D3DXVECTOR3(0, 0, 0);
	m_pAnimation->SetScale(0.5f, 0.5f);

	m_pRect->SetSize(m_pAnimation->GetWidth() * 0.5f, m_pAnimation->GetHeight());
	m_pRect->SetDraw(false);
	m_pRect->SetBorder(true);

	m_fSpeed = 300.0f;

	m_isOnGround = true;
	m_isJumping = false;
	m_fJumpPower = 450.0f;

	m_fLeftEnd = 200;
	m_fRightEnd = WINSIZEX * 0.5f - 100;

	m_isLeft = false;

	m_nNumJump = 0;
}

void Player::Update()
{
	static int currState = 0;
	static const char* szName[MAX];
	
	for (int i = 0; i < MAX; i++)
		szName[i] = new char[32];

	szName[0] = "IDLE";
	szName[1] = "RUN";
	szName[2] = "JUMP UP";
	szName[3] = "JUMP DOWN";
	szName[4] = "DIE";

	ImGui::ListBox("Animation", &currState, szName, MAX);

	ImGui::InputFloat("PlayerSpeed", &m_fSpeed);

	if (g_pKeyManager->isStayKeyDown('D'))
	{
		if (m_vPosition.x < m_fRightEnd)
		{
			m_vPosition.x += m_fSpeed * g_pTimeManager->GetDeltaTime();
			m_nMoveDir = 0;
		}
		else
		{
			m_vPosition.x = m_fRightEnd;
			m_nMoveDir = 1;
		}

		m_vRotaion.y = 0.0f;
		m_isLeft = false;
		currState = 1;
	}
	else if (g_pKeyManager->isStayKeyDown('A'))
	{
		if (m_vPosition.x > m_fLeftEnd)
		{
			m_vPosition.x -= m_fSpeed * g_pTimeManager->GetDeltaTime();
			m_nMoveDir = 0;
		}
		else
		{
			m_vPosition.x = m_fLeftEnd;
			m_nMoveDir = -1;
		}

		m_vRotaion.y = D3DX_PI;
		m_isLeft = true;
		currState = 1;
	}
	else
	{
		m_nMoveDir = 0;
		currState = 0;
	}

	if (g_pKeyManager->isOnceKeyDown(VK_SPACE) && m_isJumping == false)
	{
		m_isOnGround = false;
		m_isJumping = true;
		m_fElapsedGravity = 0.0f;
		m_nNumJump++;
	}

	ImGui::InputFloat("PlayerPosY", &m_vPosition.y);
	static float y;
	ImGui::InputFloat("ObjectPosY", &y);

	m_stRect.left = m_pRect->GetPosition().x - m_pRect->GetSize().x * 0.5f;
	m_stRect.right = m_stRect.left + m_pRect->GetSize().x;
	m_stRect.top = m_pRect->GetPosition().y + m_pRect->GetSize().y * 0.5f;
	m_stRect.bottom = m_stRect.top - m_pRect->GetSize().y;

	if (m_isOnGround == false)
	{
		for (int i = 0; i < m_pMapObject->GetObjects().size(); i++)
		{
			tagObject object = m_pMapObject->GetObjects()[i];

			y = m_pMapObject->GetObjects()[0].pSprite->GetPosition().y;

			if (m_fElapsedGravity >= m_fJumpPower
				&& m_vPosition.y > object.pSprite->GetPosition().y
				&& AABB2D(m_stRect, object.stRect))
			{
				m_vPosition.y = object.vPos.y + object.pSprite->GetHeight() * 0.5f;
				m_isJumping = false;
				m_fElapsedGravity = m_fJumpPower;
				break;
			}
			
			if (i == m_pMapObject->GetObjects().size() - 1 && m_isJumping == false)
			{
				m_isJumping = true;
			}
		}
	}

	if (m_isJumping)
	{
		if (m_fJumpPower > m_fElapsedGravity)
			currState = 2;
		else
			currState = 3;

		m_vPosition.y += (m_fJumpPower - m_fElapsedGravity) * g_pTimeManager->GetDeltaTime();
		m_fElapsedGravity += GRAVITY * g_pTimeManager->GetDeltaTime();

		if (m_vPosition.y <= m_pMap->GetGroundY())
		{
			m_vPosition.y = m_pMap->GetGroundY();
			m_isJumping = false;
			m_isOnGround = true;
			m_fElapsedGravity = 0.0f;
		}
	}

	float posX = m_vPosition.x + m_pAnimation->GetWidth() * 0.25f - m_pAnimation->GetWidth() * 0.5f * m_isLeft;

	m_pAnimation->SetPosition(posX, m_vPosition.y + m_pAnimation->GetHeight() * 0.5f);

	ImGui::DragFloat("Rotation X", &m_vRotaion.x);
	ImGui::DragFloat("Rotation Y", &m_vRotaion.y);
	ImGui::DragFloat("Rotation Z", &m_vRotaion.z);

	m_pAnimation->SetRotation(m_vRotaion.x, m_vRotaion.y, m_vRotaion.z);

	m_eState = (STATE)currState;
	m_pAnimation->Play(m_eState);

	m_pAnimation->Update();

	m_pRect->SetPosition(m_vPosition.x, m_vPosition.y + m_pAnimation->GetHeight() * 0.5f);
	m_pRect->Update();
}

void Player::Render()
{
	m_pAnimation->Render();
	m_pRect->Render();

	g_pTextManager->IntRender(m_nNumJump, 10, WINSIZEY - 120, 50, NumberType::Blue);
}
