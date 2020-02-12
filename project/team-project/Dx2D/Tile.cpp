#include "stdafx.h"
#include "Tile.h"


Tile::Tile(Sprite * s)
{
	m_sprite = s;
	m_rectCollider = NULL;
	m_triangleCollider = NULL;
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

	// collider 크기 조정

}

void Tile::Update()
{
	m_sprite->Update();
}

void Tile::Update(D3DXVECTOR2 tilePos)
{
	m_tilePos = tilePos;
	m_sprite->SetPosition(tilePos);
	m_sprite->Update();

	// collider 업데이트
	if (m_triangleCollider)	// 삼각형 collider
	{

	}
	else if (m_rectCollider)	// 사각형 collider
	{
		m_rectCollider->SetPosition(tilePos);
		m_rectCollider->Update();
	}
}

void Tile::Render()
{
	m_sprite->Render();

	// collider 그리기
	if (g_isDrawCollider)
	{
		if (m_triangleCollider)	// 삼각형 collider
		{
			m_triangleCollider->SetBorder(true);
			m_triangleCollider->Render();
		}
		else if (m_rectCollider)	// 사각형 collider
		{
			m_rectCollider->SetBorder(true);
			m_rectCollider->Render();
		}
	}
	else
	{
		if (m_triangleCollider)	// 삼각형 collider
		{
			m_triangleCollider->SetBorder(false);
			m_triangleCollider->Render();
		}
		else if (m_rectCollider)	// 사각형 collider
		{
			m_rectCollider->SetBorder(false);
			m_rectCollider->Render();
		}
	}
}

void Tile::Release()
{
}

void Tile::SetRectCollider(Rect* rect)
{
	if (m_triangleCollider != NULL) return;
	m_rectCollider = rect;
}

void Tile::SetRectCollider()
{
	if (m_triangleCollider != NULL) return;	// 이미 삼각형 collider를 가지고 있는 경우 함수 실행 취소됨(collider는 한 가지 종류만...)

	m_rectCollider = new Rect;
	m_rectCollider->Init();
	// collider를 타일과 같은 크기로 설정
	m_rectCollider->SetSize({ m_sprite->GetWidth()*m_sprite->GetSize().x,  m_sprite->GetHeight()*m_sprite->GetSize().y });
}

void Tile::SetTriangleCollider(vector<PCVertex> m_vecVertex)
{
	if (m_rectCollider != NULL) return;	// 이미 Rect collider를 가지고 있는 경우 함수 실행 취소됨(collider는 한 가지 종류만...)

	m_triangleCollider = new MyTriangle(m_vecVertex);
}
