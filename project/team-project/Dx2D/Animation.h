#pragma once

enum class AniState
{
	Ready = -1,
	Idle = 0,
	Run,
	Jump,
	Die,
	Hit
};

class Animation
{
private:
	D3DXVECTOR2				m_vPosition;
	float					m_fAniSpeed;
	AniState				m_eCurrState;

	// 맵<키값, 데이터값>
	map<AniState, Clip*>	m_mapClip;

public:
	Animation();
	~Animation();

	void AddClip(AniState state, Clip* clip);
	void Play(AniState state);
	void PlayReverse(AniState state);
	void SetStartFrame(AniState state, int start);

	void SetPosition(float x, float y) { m_vPosition.x = x; m_vPosition.y = y; }
	void SetPosition(D3DXVECTOR2 p) { m_vPosition.x = p.x; m_vPosition.y = p.y; }

	void SetScale(float x, float y);
	void SetConstantSizeScale(float x, float y);

	void SetAniSpeed(float s) { m_fAniSpeed = s; }
	void ApplyTransform();

	void Update();
	void Render();
};

