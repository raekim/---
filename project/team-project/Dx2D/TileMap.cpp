#include "stdafx.h"
#include "TileMap.h"


TileMap::TileMap()
{
	// 타일맵 배열 초기화
	m_tileMapInfo = vector<vector<int>>(m_mapHeight / TILESIZE, vector<int>(m_mapWidth / TILESIZE, 0));
}

TileMap::TileMap(int w, int h)
{
	m_mapWidth = w;
	m_mapHeight = h;
	TileMap();
}


TileMap::~TileMap()
{
}

void TileMap::Init()
{
}

void TileMap::Update()
{
	// int로 표현된 맵 정보로부터 실제 tile로 구성된 맵을 로드한다
	for (int i = 0; i < m_mapHeight; ++i)
	{
		for (int j = 0; j < m_mapWidth; ++j)
		{
			// 각 타일들의 적절한 위치 업데이트
			m_tileMap[i][j].Update({ j*TILESIZE + TILESIZE * 0.5f, (m_mapHeight - i - 1)*TILESIZE + TILESIZE * 0.5f });
		}
	}
}

void TileMap::Render()
{
	// int로 표현된 맵 정보로부터 실제 tile로 구성된 맵을 로드한다
	for (int i = 0; i < m_mapHeight; ++i)
	{
		for (int j = 0; j < m_mapWidth; ++j)
		{
			m_tileMap[i][j].Render();
		}
	}
}

void TileMap::Release()
{
}

void TileMap::Load(map<int, Tile*>& templates)
{
	// int로 표현된 맵 정보로부터 실제 tile로 구성된 맵을 로드한다
	for (int i = 0; i < m_mapHeight; ++i)
	{
		for (int j = 0; j < m_mapWidth; ++j)
		{
			// int(key)로부터 실제 tile(value)정보를 꺼내와 타일맵으로 저장한다
			m_tileMap[i][j] = *(templates[m_tileMapInfo[i][j]]);
		}
	}
}

void TileMap::Unload()
{
	m_tileMap.clear();
}
