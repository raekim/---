#pragma once
class MyTriangle : public Primitive2DObejct<PCVertex>
{
public:
	vector<D3DXVECTOR2> m_triangleVertexOffset;
	vector<D3DXVECTOR2> m_triangleVertex;
	MyTriangle(vector<PCVertex> vecVertex);
	MyTriangle();
	~MyTriangle();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
};

