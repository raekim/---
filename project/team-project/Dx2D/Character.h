#pragma once

// 기본값 디파인
#define GROUND 50.0f				// 임시 바닥 지정
#define LEFTEND (WINSIZEX * 0.5f - 200)		// 아케이드식 이동에 필요한 왼쪽 끝 방향
#define RIGHTEND (WINSIZEX * 0.5f + 200)	//	"					  오른쪽 끝 방향
#define TOPEND (WINSIZEY * 0.5f + 200)		//  "
#define BOTTOMEND (WINSIZEY * 0.5f - 200)	//  "
#define SPEED 250.0f						// 캐릭터 기본 스피드값
#define JUMPPOWER 500.0f					// 캐릭터 기본 점프파워값

class Character
{
	enum STATE	// Character Animation State
	{
		READY = -1,
		IDLE,
		RUN,
		JUMPUP,
		JUMPDOWN,
		HIT,
		DIE,
		FIRE,
		MAX
	};

	enum DIR // Fire 발사 시 바라보고 있는 방향 ( DIR 값을 곱해줘서 왼쪽으로 날아갈지 오른쪽으로 날아갈지!)
	{
		LEFT = -1,
		RIGHT = 1
	};

private:
	Animation<STATE>*		m_pAnimation;			// 캐릭터 에니메이션
	STATE					m_eState;				// 캐릭터 현재 상태
	D3DXVECTOR2				m_vPosition;			// 캐릭터 포지션
	Rect*					m_pBoundBox;			// 캐리터 히트박스
	Circle*					m_pBoundCircle;			// 캐릭터 히트서클
	Circle*					m_pOnGroundCheckCircle;	// 바닥에 붙어있는지 여부 체크하기 위한 충돌 서클 (착지 여부 확인 서클)

	float					m_fElapsedGravity;		// 누적 중력
	float					m_fJumpPower;			// 캐릭터 점프 파워
	float					m_fSpeed;				// 캐릭터 스피드
	bool					m_isJumping;			// 캐릭터 점프 여부
	bool					m_isOnGorund;			// 착지 여부
	bool					m_isFalling;
	

	// FIRE ----------
	DIR						m_eDir;					// Fire 발사 방향
	Animation<STATE>*		m_pFire;				// Fire 에니메이션
	bool					m_isShot;				// Fire 발사 여부
	float					m_fFireSpeed;			// Fire Speed
	D3DXVECTOR2				m_vFirePos;				// Fire Position
	
													//----------------

	TileMapManager*			m_pTileMapManager;		// 셋터 통해 받아올 타일맵 매니저
	vector<Rect*>			m_vecTest;				// 오브젝트 테스터
	bool					m_isColliderBorder;		// 캐릭터 히트박스 보더라인 여부

public:
	Character();
	~Character();

	void Init();
	void Update();
	void Render();


	// 구현 함수
	void Conctrol();		// 조작키 ( 좌우 방향키로 이동, SpaceBar - 점프, F - Fire 발사
	void Shoot();			// Fire 발사 함수
	void Collision();		// 충돌
	void Gravity();			// 점프 시 중력 및 떨어지는 함수


	// Getter / Setter
	void SetPosition(D3DXVECTOR2 pos) { m_vPosition = pos; }
	void SetSpeed(float speed) { m_fSpeed = speed; }		
	void SetJumpPower(float JumpPower) { m_fJumpPower = JumpPower; }
	void SetTileMapManager(TileMapManager* tileManager) { m_pTileMapManager = tileManager; }

	float GetSpeed() { return m_fSpeed; }
	float GetJumpPower() { return m_fJumpPower; }
	
	int GetMoveDir() { return (int)m_eState; }

	D3DXVECTOR2 GetCircleCenter() { return m_pBoundCircle->GetPosition(); }
	float GetCircleRadius() { return m_pBoundCircle->GetRadius(); }
};

