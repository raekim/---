#include "stdafx.h"
#include "TileMap.h"


TileMap::TileMap()
{

}

TileMap::TileMap(int w, int h)
{
	m_mapWidth = w;
	m_mapHeight = h;
	m_mapInfoWidth = m_mapWidth / TILESIZE;
	m_mapInfoHeight = m_mapHeight / TILESIZE;

	// 타일맵 자료구조 초기화
	m_tileMapInfo = vector<vector<int>>(m_mapInfoHeight, vector<int>(m_mapInfoWidth, 0));
	m_tileMap = vector<vector<Tile>>(m_mapInfoHeight, vector<Tile>(m_mapInfoWidth));
}


TileMap::~TileMap()
{
}

void TileMap::Init()
{
}

void TileMap::Update()
{
	//// int로 표현된 맵 정보로부터 실제 tile로 구성된 맵을 로드한다
	//for (int i = 0; i < m_mapInfoHeight; ++i)
	//{
	//	for (int j = 0; j < m_mapInfoWidth; ++j)
	//	{
	//		// 각 타일들의 적절한 위치 업데이트
	//		if (m_tileMapInfo[i][j] == 0) continue;
	//		m_tileMap[i][j].Update({ j*TILESIZE + TILESIZE * 0.5f, (m_mapInfoHeight - i - 1)*TILESIZE + TILESIZE * 0.5f });
	//	}
	//}
}

void TileMap::Render()
{
	// int로 표현된 맵 정보로부터 실제 tile로 구성된 맵을 로드한다
	for (int i = 0; i < m_mapInfoHeight; ++i)
	{
		for (int j = 0; j < m_mapInfoWidth; ++j)
		{
			if (m_tileMapInfo[i][j] == 0) continue;

			// 각 타일들의 적절한 위치 업데이트
			if (m_tileMapInfo[i][j] == 0) continue;
			D3DXVECTOR2 tilePos = { j*TILESIZE + TILESIZE * 0.5f, (m_mapInfoHeight - i - 1)*TILESIZE + TILESIZE * 0.5f };
			tilePos -= g_cameraPos;
			m_tileMap[i][j].Update(tilePos);
			// 타일 렌더
			m_tileMap[i][j].Render();
		}
	}
}

void TileMap::Release()
{
}

bool TileMap::CircleCollision(Circle * other)
{
	bool collide = false;
	// int로 표현된 맵 정보로부터 실제 tile로 구성된 맵을 로드한다
	for (int i = 0; i < m_mapInfoHeight; ++i)
	{
		for (int j = 0; j < m_mapInfoWidth; ++j)
		{

			// 각 타일들의 collider 위치 업데이트 (하나를 share하므로 각 타일마다 업데이트 해주어야 함)
			if (m_tileMapInfo[i][j] == 0) continue;
			D3DXVECTOR2 tilePos = { j*TILESIZE + TILESIZE * 0.5f, (m_mapInfoHeight - i - 1)*TILESIZE + TILESIZE * 0.5f };
			tilePos -= g_cameraPos;
			m_tileMap[i][j].Update(tilePos);

			// 충돌 체크
			collide |= m_tileMap[i][j].CollisionWithCircle(other);
		}
	}
	return collide;
}

void TileMap::Load(map<int, Tile*>& templates)
{
	// int로 표현된 맵 정보로부터 실제 tile로 구성된 맵을 로드한다
	for (int i = 0; i < m_mapInfoHeight; ++i)
	{
		for (int j = 0; j < m_mapInfoWidth; ++j)
		{
			if (m_tileMapInfo[i][j] == 0) continue;
			// int(key)로부터 실제 tile(value)정보를 꺼내와 타일맵으로 저장한다
			Tile* pTile = templates[m_tileMapInfo[i][j]];
			
			m_tileMap[i][j] = *pTile;
		}
	}
}

void TileMap::Unload()
{
	m_tileMap.clear();
}
