#pragma once

class Map;
class MapObject;

class Player
{
	enum STATE
	{
		READY = -1,
		IDLE,
		RUN,
		JUMPUP,
		JUMPDOWN,
		DIE,
		MAX
	};

private:
	D3DXVECTOR2			m_vPosition;
	D3DXVECTOR3			m_vRotaion;
	bool				m_isLeft;
	Rect*				m_pRect;
	RECT				m_stRect;

	Animation<STATE>*	m_pAnimation;
	STATE				m_eState;

	float				m_fSpeed;

	bool				m_isOnGround;
	bool				m_isJumping;
	float				m_fJumpPower;
	float				m_fElapsedGravity;

	float				m_fLeftEnd;
	float				m_fRightEnd;
	int					m_nMoveDir;

	Map*				m_pMap;
	MapObject*			m_pMapObject;

	int					m_nNumJump;

public:
	Player();
	~Player();

	void Init();
	void Update();
	void Render();

	float GetSpeed() { return m_fSpeed; }
	int GetMoveDir() { return m_nMoveDir; }

	RECT& GetRect() { return m_stRect; }

	void SetMap(Map* p) { m_pMap = p; }
	void SetMapObject(MapObject* p) { m_pMapObject = p; }

	D3DXMATRIX& GetWMatrix() { return m_pRect->GetWMatrix(); }

	D3DXVECTOR2 GetPosition() { return m_vPosition; }
	float GetWidth() { return m_pAnimation[m_eState].GetWidth(); }
	float GetHeight() { return m_pAnimation[m_eState].GetHeight(); }
	D3DXVECTOR3 GetRotation() { return m_vRotaion; }
};

