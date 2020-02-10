#pragma once

#define GROUND 100.0f

class Character
{
	enum STATE
	{
		READY = -1,
		IDLE,
		RUN,
		JUMPUP,
		JUMPDOWN,
		HIT,
		DIE,
		MAX
	};

private:
	Animation<STATE>*		m_pAnimation;
	STATE					m_eState;
	D3DXVECTOR2				m_vPosition;
	
	float					m_fElapsedGravity;
	float					m_fJumpPower;
	float					m_fSpeed;
	bool					m_isJumping;
	

	void Conctrol();

public:
	Character();
	~Character();

	void Init();
	void Update();
	void Render();

};

