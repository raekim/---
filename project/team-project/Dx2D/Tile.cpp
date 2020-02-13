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
	m_sprite->SetConstantScale({ TILESIZE,TILESIZE });
}

bool Tile::CollisionWithCircle(Circle* other)
{
	if (m_hasRectCollider)
	{
		return CircleRectCollision(other->GetPosition(), other->GetRadius(), m_rectCollider->GetPosition(), m_rectCollider->GetSize());
	}
	else
	{
		return CircleTriangleCollision(other->GetPosition(), other->GetRadius(), m_triangleCollider->m_triangleVertex[0], m_triangleCollider->m_triangleVertex[1], m_triangleCollider->m_triangleVertex[2]);
	}
	//CircleRectCollision(D3DXVECTOR2 circleCenter, float circleRadius, D3DXVECTOR2 p1, D3DXVECTOR2 s1)
	//CircleTriangleCollision(D3DXVECTOR2 circleCenter, float circleRadius, D3DXVECTOR2 p1, D3DXVECTOR2 p2, D3DXVECTOR2 p3)
}

void Tile::Update()
{
	// collider 업데이트
	if (m_hasRectCollider)
	{
		m_rectCollider->SetPosition(m_tilePos);
		m_rectCollider->Update();
	}
	else
	{
		m_triangleCollider->SetPosition(m_tilePos);
		m_triangleCollider->Update();
	}
}

void Tile::Update(D3DXVECTOR2 tilePos)
{
	// 카메라 offset을 적용한 화면상의 타일 위치 저장
	m_tilePos = tilePos;

	// 타일 이미지 업데이트
	m_sprite->SetPosition(tilePos);
	m_sprite->Update();

	// collider 업데이트
	if (m_hasRectCollider)
	{
		m_rectCollider->SetPosition(tilePos);
		m_rectCollider->Update();
	}
	else
	{
		m_triangleCollider->SetPosition(tilePos);
		m_triangleCollider->Update();
	}
}

void Tile::Render()
{
	// 타일 이미지 그리기
	m_sprite->Render();

	// collider 그리기
	// g_isDrawCollider가 true일 때 collider를 그려준다.
	if (m_hasRectCollider)
	{
		m_rectCollider->SetBorder(g_isDrawCollider);
		if (g_isDrawCollider) m_rectCollider->Render();
	}
	else
	{
		m_triangleCollider->SetBorder(g_isDrawCollider);
		if (g_isDrawCollider) m_triangleCollider->Render();
	}
}

void Tile::Release()
{
}

void Tile::SetRectCollider(Rect* rect)
{
	m_hasRectCollider = true;
	m_rectCollider = rect;
}

void Tile::SetRectCollider()
{
	m_hasRectCollider = true;

	m_rectCollider = new Rect;
	m_rectCollider->Init();
	
	// collider를 타일과 같은 크기로 설정
	m_rectCollider->SetSize({ m_sprite->GetWidth()*m_sprite->GetSize().x,  m_sprite->GetHeight()*m_sprite->GetSize().y });
}

void Tile::SetTriangleCollider(vector<PCVertex> m_vecVertex)
{
	m_hasRectCollider = false;

	m_triangleCollider = new MyTriangle(m_vecVertex);
	m_triangleCollider->Init();

	//collider를 타일과 같은 크기로 설정
	m_triangleCollider->SetSize({ m_sprite->GetWidth()*m_sprite->GetSize().x,  m_sprite->GetHeight()*m_sprite->GetSize().y });
}
