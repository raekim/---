#pragma once

class Map;

struct tagObject
{
	Sprite* pSprite;
	D3DXVECTOR2 vPos;
	RECT stRect;
};

class MapObject
{
private:
	Map*				m_pMap;
	vector<tagObject>	m_vecObject;

public:
	MapObject();
	~MapObject();

	void Init();
	void Update();
	void Render();

	void SetMap(Map* p) { m_pMap = p; }

	const vector<tagObject>& GetObjects() { return m_vecObject; }
};

