#pragma once
class Monster
{
	enum STATE
	{
		READY = -1,
		MOVE,
		DIE,
		MAX
	};

private:

	// 첫번째 몬스터구현
	D3DXVECTOR2			m_SlimePos;
	D3DXVECTOR3			m_SlimeRot;
	float				m_SlimeSpeed;

	Rect*				m_SlimeRect;

	Animation<STATE>*	m_Slime;
	STATE				m_eState;
	
	bool				is_SlimeMove;
	// 두번째 몬스터 구현
	D3DXVECTOR2			m_BeePos;
	D3DXVECTOR3			m_BeeRot;
	float				m_BeeSpeed;

	Animation<STATE>*	m_Bee;

	Rect*				m_BeeRect;
	bool				is_BeeMove;

public:
	Monster();
	~Monster();

	void Init();
	void Update();
	void Render();

};

