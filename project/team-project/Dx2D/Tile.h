#pragma once
class Tile : public iGameNode
{
public:
	Sprite* m_sprite;
	Tile(Sprite* s);
	Tile();
	~Tile();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Update(D3DXVECTOR2 tilePos);
	virtual void Render() override;
	virtual void Release() override;
};

