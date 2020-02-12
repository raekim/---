#pragma once
class MyTriangle : public Primitive2DObejct<PCVertex>
{
public:
	MyTriangle(vector<PCVertex> m_vecVertex);
	MyTriangle();
	~MyTriangle();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
};

