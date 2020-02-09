#pragma once

class Player;

class Map
{
private:
	Sprite*			m_arrLayer[3];
	float			m_arryLayerPosX[3];
	float			m_fGroundY;

	Player*			m_pPlayer;
	float			m_fMapPosX;

public:
	Map();
	~Map();

	void Init();
	void Update();
	void Render();

	void SetPlayer(Player* p) { m_pPlayer = p; }

	float GetGroundY() { return m_fGroundY; }
	float GetMapPosX() { return m_fMapPosX; }
};

