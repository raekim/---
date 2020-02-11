#include "stdafx.h"
#include "Tile.h"


Tile::Tile(Sprite * s)
{
	m_sprite = s;
}

Tile::Tile()
{
}


Tile::~Tile()
{
}

void Tile::Init()
{
	m_sprite->Init();
}

void Tile::Update()
{
	m_sprite->Update();
}

void Tile::Update(D3DXVECTOR2 tilePos)
{
	m_sprite->SetPosition(tilePos);
	m_sprite->Update();
}

void Tile::Render()
{
	m_sprite->Render();
}

void Tile::Release()
{
}
