#include "stdafx.h"
#include "TileMapManager.h"

TileMapManager::TileMapManager()
{
	// id(int) : 타일(sprite) 매칭시키는 map 초기화
	// spritesheet_ground.png 파일에서 공백 칸 주의!! (맨 오른쪽 column은 아예 다 공백임)
	g_pTextureManager->AddTexture(L"test-ground", L"spritesheet_ground.png");
	Tile* tile;
	for (int i = 0; i < 8 * 16; ++i)
	{
		if ((i + 1) % 8 == 0) continue; // 이미지상 맨 오른쪽 column은 공백이므로 건너뜀
		tile = new Tile(new Sprite(L"test-ground", 8, 16, i));
		m_tileTemplates.insert(make_pair(i, tile));
	}

	// 타일맵들(int로 표현)을 만들어서 보유한 타일맵 목록에 넣기
	TileMap* tileMap;
	tileMap = new TileMap(WINSIZEX, WINSIZEY);
	// 16x9
	string mapInfoStr = string
	("1100000000000000\
	0000000000000000\
	0000000000000000\
	0000000000000000\
	0000000000000000\
	0000000000000000\
	0000000000000000\
	0000000000000000\
	0000000000000000\
	");
	for (int i = 0; i < tileMap->GetMapInfoHeight(); ++i)
	{
		for (int j = 0; j < tileMap->GetMapInfoWidth(); ++j)
		{
			tileMap->SetTileMapInfo(i, j, mapInfoStr[i*tileMap->GetMapInfoWidth() + j] - '0');
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
