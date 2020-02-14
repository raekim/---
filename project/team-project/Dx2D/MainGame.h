#pragma once
#include "TileMapManager.h"
class MainGame : public iGameNode
{
private:
	ID3D11Buffer* m_pViewProjBuffer;	// 뷰, 프로젝션 매트릭스 버퍼
	void SetViewProjectionMatrixes();	// 뷰, 프로젝션 매트릭스 셋팅 및 버퍼 생성

	ID3D11BlendState*	m_pNormalBlendState;	// 기본 블렌드 옵션
	ID3D11BlendState*	m_pAlphaBlendState;		// 반투명 블렌드 옵션
	void SetBlendStates();	// 옵션별 블렌드 상태값 생성

	// my code
	ID3D11SamplerState* samplerState; // 샘플링 스테이트

private:
	// Game Classes
	TileMapManager* m_tileMapManager;
	Circle* m_testCircle;	// 원 충돌 테스트용
	Sprite* m_backgroundImg;	// 테스트 백그라운드 이미지
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

