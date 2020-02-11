#pragma once
#include "Tile.h"
#define TILESIZE 80

// 타일맵 하나는 맵 하나를 표현
class TileMap : public iGameNode
{
private:
	int						m_mapWidth;				// 맵 너비
	int						m_mapHeight;			// 맵 높이
	int						m_mapInfoWidth;			// 맵 정보 너비(몇 칸의 타일로 이루어져 있는가?)
	int						m_mapInfoHeight;		// 맵 정보 높이(몇 칸의 타일로 이루어져 있는가?)
	vector<vector<int>>		m_tileMapInfo;			// 타일맵의 int 표현
	vector<vector<Tile>>	m_tileMap;				// 실제 Tile로 구성된 타일맵(매니저에 의해 로딩되어야 함)

public:
	TileMap();
	TileMap(int w, int h);
	~TileMap();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;

	// getter & setter
	int GetMapInfoWidth(){ return m_mapInfoWidth; }
	int GetMapInfoHeight() { return m_mapInfoHeight; }
	void SetTileMapInfo(int i, int j, int info) { m_tileMapInfo[i][j] = info; }

	// 맵 로드/언로드
	void Load(map<int, Tile*>& templates);
	void Unload();
};

