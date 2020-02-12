#include "stdafx.h"
#include "TileMapManager.h"

TileMapManager::TileMapManager()
{
	// id(int) : 타일(sprite) 매칭시키는 map 초기화
	// spritesheet_ground.png 파일에서 공백 칸 주의!! (맨 오른쪽 column은 아예 다 공백임)
	g_pTextureManager->AddTexture(L"test-ground", L"spritesheet_ground.png");
	Tile* tile;
	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			tile = new Tile(new Sprite(L"test-ground", 8, 16, i * 8 + j));
			// 타일 collider 설정
			// 디폴트는 sprite 사이즈에 맞는 rect collider입니다.
			{
				tile->SetRectCollider();
			}
			m_tileTemplates.insert(make_pair((i + 1) * 10 + (j + 1), tile));
		}
	}

	// 수동 collider 설정
	{

	}

	// 타일맵들(int로 표현)을 만들어서 보유한 타일맵 목록에 넣기
	TileMap* tileMap;
	FILE * pFile;

	tileMap = new TileMap(WINSIZEX, WINSIZEY);


	// 16x9 맵 정보를 파일로부터 읽어온다
	pFile = fopen("../../_MapInfo/map1.txt", "rt");
	int tileInfo[9][16];
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 16; ++j)
		{
			fscanf(pFile, "%d ", &tileInfo[i][j]);
		}
	}
	for (int i = 0; i < tileMap->GetMapInfoHeight(); ++i)
	{
		for (int j = 0; j < tileMap->GetMapInfoWidth(); ++j)
		{
			tileMap->SetTileMapInfo(i, j, tileInfo[i][j]);
		}
	}
	m_tileMaps.push_back(tileMap);
}


TileMapManager::~TileMapManager()
{
}

void TileMapManager::Init()
{
	m_currentLoadedMapIdx = 0;
}

void TileMapManager::Update()
{
	m_tileMaps[m_currentLoadedMapIdx]->Update();
}

void TileMapManager::Render()
{
	m_tileMaps[m_currentLoadedMapIdx]->Render();
}

void TileMapManager::Release()
{
}

void TileMapManager::LoadMap(int idx)
{
	m_tileMaps[idx]->Load(m_tileTemplates);
}

void TileMapManager::UnloadMap(int idx)
{
	m_tileMaps[idx]->Unload();
}