#pragma once
#include "TileMap.h"
#include "Tile.h"
class TileMapManager : public iGameNode
{
	vector<TileMap>		m_tileMaps;
	map<int, Tile*>		m_tileTemplates;	// id(int) : 타일 매핑해주는 맵
	int					m_currentLoadedMapIdx;	// 현재 로딩된 맵 인덱스
public:
	TileMapManager();
	~TileMapManager();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;

	void LoadMap(int idx = 0);	// 타일맵 정보를 가지고 있는 2차원 int 배열로부터 실제 Tile로 이루어진 맵을 그리기 위한 자원을 로드한다
	void UnloadMap(int idx = 0);
};

