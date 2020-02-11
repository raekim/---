#pragma once

class MainGame : public iGameNode
{
private:
	ID3D11Buffer* m_pViewProjBuffer;	// ��, �������� ��Ʈ���� ����
	void SetViewProjectionMatrixes();	// ��, �������� ��Ʈ���� ���� �� ���� ����

	ID3D11BlendState*	m_pNormalBlendState;	// �⺻ ���� �ɼ�
	ID3D11BlendState*	m_pAlphaBlendState;		// ������ ���� �ɼ�
	void SetBlendStates();	// �ɼǺ� ���� ���°� ����

private:
	// Game Classes	
	Character*				m_pCharacter;

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

