#include "stdafx.h"
#include "Animation.h"


Animation::Animation()
	: m_vPosition(0, 0)
	, m_fAniSpeed(1.0f)
	, m_eCurrState(AniState::Ready)
{
}


Animation::~Animation()
{
	for (auto p : m_mapClip)
		SAFE_DELETE(p.second);
	m_mapClip.clear();
}

void Animation::AddClip(AniState state, Clip * clip)
{
	m_mapClip.insert(make_pair(state, clip));
}

void Animation::Play(AniState state)
{
	//if (m_eCurrState == state) return;

	m_eCurrState = state;

	// m_mapClip.find(m_eCurrState)->second->Play();
	m_mapClip[m_eCurrState]->Play();
}

void Animation::PlayReverse(AniState state)
{
	m_mapClip[state]->PlayReverse();
}

void Animation::SetStartFrame(AniState state, int start)
{
	m_mapClip[state]->SetStartFrame(start);
}

void Animation::SetScale(float x, float y)
{
	for (auto p : m_mapClip)
		p.second->SetScale(x, y);
}

void Animation::SetConstantSizeScale(float x, float y)
{
	for (auto p : m_mapClip)
		p.second->SetConstantScale(x, y);
}

void Animation::ApplyTransform()
{
	for (auto p : m_mapClip)
		p.second->ApplyTransform();
}

void Animation::Update()
{
	if (m_mapClip.find(m_eCurrState) == m_mapClip.end()) return;

	for (auto p : m_mapClip)
	{
		p.second->SetSpeed(m_fAniSpeed);
		p.second->SetPosition(m_vPosition);
		p.second->Update();
	}
}

void Animation::Render()
{
	if (m_mapClip.find(m_eCurrState) == m_mapClip.end()) return;

	m_mapClip[m_eCurrState]->Render();
}
