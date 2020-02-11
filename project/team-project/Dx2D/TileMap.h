#pragma once
#include "Tile.h"
#define TILESIZE 20

// 타일맵 하나는 맵 하나를 표현
class TileMap : public iGameNode
{
private:
	int						m_mapWidth;				// 맵 너비
	int						m_mapHeight;			// 맵 높이
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

	void SetTileMapInfo(vector<vector<int>>	info) { m_tileMapInfo = info; }
	// 맵 로드/언로드
	void Load(map<int, Tile*>& templates);
	void Unload();
};

