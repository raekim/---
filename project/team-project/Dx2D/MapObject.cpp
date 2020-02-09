#include "stdafx.h"
#include "MapObject.h"
#include "Map.h"


MapObject::MapObject()
{
	g_pTextureManager->AddTexture(L"Bush", L"BushMario.png");

	tagObject stObject;

	stObject.pSprite = new Sprite(L"Bush");
	stObject.pSprite->SetConstantScale(200, 50);
	stObject.vPos = D3DXVECTOR2(500, 250);
	m_vecObject.push_back(stObject);

	stObject.pSprite = new Sprite(L"Bush");
	stObject.pSprite->SetConstantScale(200, 50);
	stObject.vPos = D3DXVECTOR2(700, 250);
	m_vecObject.push_back(stObject);

	stObject.pSprite = new Sprite(L"Bush");
	stObject.pSprite->SetConstantScale(200, 50);
	stObject.vPos = D3DXVECTOR2(1800, 200);
	m_vecObject.push_back(stObject);
}


MapObject::~MapObject()
{
	for (int i = 0; i < m_vecObject.size(); i++)
		SAFE_DELETE(m_vecObject[i].pSprite);
	m_vecObject.clear();
}

void MapObject::Init()
{
}

void MapObject::Update()
{
	for (int i = 0; i < m_vecObject.size(); i++)
	{
		m_vecObject[i].pSprite->SetPosition(m_vecObject[i].vPos.x - m_pMap->GetMapPosX(), m_vecObject[i].vPos.y);
		m_vecObject[i].pSprite->Update();
		
		m_vecObject[i].stRect.left = m_vecObject[i].pSprite->GetPosition().x - m_vecObject[i].pSprite->GetWidth() * 0.5f;
		m_vecObject[i].stRect.right = m_vecObject[i].stRect.left + m_vecObject[i].pSprite->GetWidth();
		m_vecObject[i].stRect.top = m_vecObject[i].pSprite->GetPosition().y + m_vecObject[i].pSprite->GetHeight() * 0.5f;
		m_vecObject[i].stRect.bottom = m_vecObject[i].stRect.top - m_vecObject[i].pSprite->GetHeight();
	}
}

void MapObject::Render()
{
	for (int i = 0; i < m_vecObject.size(); i++)
	{
		m_vecObject[i].pSprite->Render();
	}
}
