#include "stdafx.h"
#include "Monster.h"


Monster::Monster()
{
	g_pTextureManager->AddTexture(L"Monster", L"spritesheet_enemie.png");

	m_Slime = new Animation<STATE>;
	m_Bee	= new Animation<STATE>;

	Clip* clip = new Clip;

	for (int i = 0; i < 2; i++)
	{
		Sprite* sprite = new Sprite(L"Monster", 5, 13, i + 5 * 2);
		clip->AddFrame(sprite, 1 / 2.0f);
	}
	m_Slime->AddClip(MOVE, clip);

	clip = new Clip;
	for (int i = 0; i < 2; i++)
	{
		Sprite* sprite = new Sprite(L"Monster", 5, 13, i + 5 * 8);
		clip->AddFrame(sprite, 1 / 2.0f);
	}
	m_Bee->AddClip(MOVE, clip);

	m_SlimeRect = new Rect;
	m_BeeRect = new Rect;
}


Monster::~Monster()
{
	delete m_Slime;
	delete m_Bee;

	delete m_SlimeRect;
	delete m_BeeRect;
}

void Monster::Init()
{
	m_eState = MOVE;
	m_Slime->Play(m_eState);
	m_Bee->Play(m_eState);

	m_SlimePos.x = 700;
	m_SlimePos.y = WINSIZEY - 220;
	m_SlimeRot = D3DXVECTOR3(0, 0, 0);
	m_Slime->SetScale(0.7f, 0.7f);
	m_SlimeSpeed = 30.0f;

	m_BeePos.x = 800;
	m_BeePos.y = WINSIZEY - 500;
	m_BeeRot = D3DXVECTOR3(0, 0, 0);
	m_Bee->SetScale(0.7f, 0.7f);
	m_BeeSpeed = 40.0f;

	m_SlimeRect->SetSize(m_Slime->GetWidth()* 0.5f, m_Slime->GetHeight() * 0.5f);
	m_SlimeRect->SetDraw(true);
	m_SlimeRect->SetBorder(true);

	m_BeeRect->SetSize(m_Bee->GetWidth()* 0.5f, m_Bee->GetHeight() * 0.5f);
	m_BeeRect->SetDraw(true);
	m_BeeRect->SetBorder(true);

	is_SlimeMove = true;

	is_BeeMove = false;
}

void Monster::Update()
{
	// 몬스터 슬라임의 움직임
	if (is_SlimeMove)
	{
		m_SlimePos.x -= m_SlimeSpeed * g_pTimeManager->GetDeltaTime();
		if (m_SlimePos.x <= 600)
			is_SlimeMove = false;
	
		m_SlimeRot.y = 0.0f;
	}
	else if(!is_SlimeMove)
	{
		m_SlimePos.x += m_SlimeSpeed * g_pTimeManager->GetDeltaTime();
		if (m_SlimePos.x >= 850)
			is_SlimeMove = true;
	
		m_SlimeRot.y = D3DX_PI;
	}

	m_Slime->SetPosition(m_SlimePos.x - g_cameraPos.x, m_SlimePos.y - g_cameraPos.y);
	m_Slime->SetRotation(m_SlimeRot.x, m_SlimeRot.y, m_SlimeRot.z);

	m_SlimeRect->SetPosition(m_SlimePos.x - g_cameraPos.x, m_SlimePos.y - g_cameraPos.y);

	if (!is_BeeMove)
	{
		m_BeePos.x += m_BeeSpeed * g_pTimeManager->GetDeltaTime();
		if (m_BeePos.x >= 900)
			is_BeeMove = true;

		m_BeeRot.y = D3DX_PI;
	}
	else if(is_BeeMove)
	{
		m_BeePos.x -= m_BeeSpeed * g_pTimeManager->GetDeltaTime();
		if (m_BeePos.x <= 500)
			is_BeeMove = false;

		m_BeeRot.y = 0.0f;
	}

	m_Bee->SetPosition(m_BeePos.x - g_cameraPos.x, m_BeePos.y - g_cameraPos.y);
	m_Bee->SetRotation(m_BeeRot.x, m_BeeRot.y, m_BeeRot.z);

	m_BeeRect->SetPosition(m_BeePos.x - g_cameraPos.x, m_BeePos.y - g_cameraPos.y);

	m_Slime->Update();
	m_Bee->Update();

	m_SlimeRect->Update();
	m_BeeRect->Update();

}

void Monster::Render()
{
	m_Slime->Render();
	m_Bee->Render();

	m_SlimeRect->Render();
	m_BeeRect->Render();
}
