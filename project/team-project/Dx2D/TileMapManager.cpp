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
			tile->Init();
			// 타일 collider 설정
			// 디폴트는 sprite 사이즈에 맞는 rect collider
			// 각 타일에 알맞은 collider는 밑의 수동 설정에서 코드로 적용시킨다
			{
				tile->SetRectCollider();
			}
			m_tileTemplates.insert(make_pair((i + 1) * 10 + (j + 1), tile));
		}
	}

	// 수동 collider 설정
	{
		//m_vecVertex.push_back(PCVertex(D3DXVECTOR3(-0.5f, -0.5f, 0), D3DXCOLOR(1, 1, 1, 1))); // 0
		//m_vecVertex.push_back(PCVertex(D3DXVECTOR3(-0.5f, 0.5f, 0), D3DXCOLOR(1, 1, 1, 1))); // 1
		//m_vecVertex.push_back(PCVertex(D3DXVECTOR3(0.5f, 0.5f, 0), D3DXCOLOR(1, 1, 1, 1))); // 2
		//m_vecVertex.push_back(PCVertex(D3DXVECTOR3(0.5f, -0.5f, 0), D3DXCOLOR(1, 1, 1, 1))); // 3
		vector<PCVertex> rightTop;	// 직각인 각이 오른쪽 위에 있는 삼각형
		rightTop.emplace_back(D3DXVECTOR3(-0.5f, 0.5f, 0), D3DXCOLOR(1, 1, 1, 1));
		rightTop.emplace_back(D3DXVECTOR3(0.5f, 0.5f, 0), D3DXCOLOR(1, 1, 1, 1));
		rightTop.emplace_back(D3DXVECTOR3(0.5f, -0.5f, 0), D3DXCOLOR(1, 1, 1, 1));
		
		vector<PCVertex> leftTop;	// 직각인 각이 왼쪽 위에 있는 삼각형
		leftTop.emplace_back(D3DXVECTOR3(-0.5f, -0.5f, 0), D3DXCOLOR(1, 1, 1, 1));
		leftTop.emplace_back(D3DXVECTOR3(-0.5f, 0.5f, 0), D3DXCOLOR(1, 1, 1, 1));
		leftTop.emplace_back(D3DXVECTOR3(0.5f, 0.5f, 0), D3DXCOLOR(1, 1, 1, 1));
		
		vector<PCVertex> leftBottom;	// 직각인 각이 왼쪽 아래에 있는 삼각형
		leftBottom.emplace_back(D3DXVECTOR3(0.5f, -0.5f, 0), D3DXCOLOR(1, 1, 1, 1));
		leftBottom.emplace_back(D3DXVECTOR3(-0.5f, -0.5f, 0), D3DXCOLOR(1, 1, 1, 1));
		leftBottom.emplace_back(D3DXVECTOR3(-0.5f, 0.5f, 0), D3DXCOLOR(1, 1, 1, 1));

		vector<PCVertex> rightBottom;	// 직각인 각이 오른쪽 아래에 있는 삼각형
		rightBottom.emplace_back(D3DXVECTOR3(0.5f, 0.5f, 0), D3DXCOLOR(1, 1, 1, 1));
		rightBottom.emplace_back(D3DXVECTOR3(0.5f, -0.5f, 0), D3DXCOLOR(1, 1, 1, 1));
		rightBottom.emplace_back(D3DXVECTOR3(-0.5f, -0.5f, 0), D3DXCOLOR(1, 1, 1, 1));
		
		m_tileTemplates[14]->SetTriangleCollider(rightTop);
		m_tileTemplates[163]->SetTriangleCollider(leftTop);
		m_tileTemplates[135]->SetTriangleCollider(leftBottom);
		m_tileTemplates[125]->SetTriangleCollider(rightBottom);
	}


	// 타일맵들(int로 표현)을 만들어서 보유한 타일맵 목록에 넣기
	TileMap* tileMap;
	FILE * pFile;

	tileMap = new TileMap(WINSIZEX*2, WINSIZEY);
	// 16x32 맵 정보를 파일로부터 읽어온다
	pFile = fopen("../../_MapInfo/map1.txt", "rt");
	int tileInfo[9][32];
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 32; ++j)
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
	//pFile = fopen("../../_MapInfo/map1.txt", "rt");
	//int tileInfo[9][16];
	//for (int i = 0; i < 9; ++i)
	//{
	//	for (int j = 0; j < 16; ++j)
	//	{
	//		fscanf(pFile, "%d ", &tileInfo[i][j]);
	//	}
	//}
	//for (int i = 0; i < tileMap->GetMapInfoHeight(); ++i)
	//{
	//	for (int j = 0; j < tileMap->GetMapInfoWidth(); ++j)
	//	{
	//		tileMap->SetTileMapInfo(i, j, tileInfo[i][j]);
	//	}
	//}
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

bool TileMapManager::CurrMapCircleCollision(Circle * other)
{
	return m_tileMaps[m_currentLoadedMapIdx]->CircleCollision(other);
}
