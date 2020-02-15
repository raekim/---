#pragma once

class MainGame : public iGameNode
{
private:
	ID3D11Buffer* m_pViewProjBuffer;	// 뷰, 프로젝션 매트릭스 버퍼
	void SetViewProjectionMatrixes();	// 뷰, 프로젝션 매트릭스 셋팅 및 버퍼 생성

	ID3D11BlendState*	m_pNormalBlendState;	// 기본 블렌드 옵션
	ID3D11BlendState*	m_pAlphaBlendState;		// 반투명 블렌드 옵션
	void SetBlendStates();	// 옵션별 블렌드 상태값 생성

private:
	// Game Classes
	Character*				m_pCharacter;
	TileMapManager*			m_tileMapManager;
	Rect*					m_pTest;
	Rect*					m_pTest2;
	Rect*					m_pTest3;
	MyTriangle*				m_pTriangle;
	Circle*					m_testCircle;		// 원 충돌 테스트용
public:
	MainGame();
	~MainGame();

	virtual void Init() override;
	void CreateGameClasses();
	void DeleteGameClasses();
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
};

