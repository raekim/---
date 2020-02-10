#pragma once
template <typename T>
class Primitive2DObejct
{
protected:
	D3DXMATRIX				m_matWorld;			// SRT 매트릭스 : 변환 행렬
	D3DXVECTOR2				m_vPosition;
	D3DXVECTOR2				m_vSize;

	vector<T>				m_vecVertex;
	vector<UINT>			m_vecIndex;

	D3DXCOLOR				m_stColor;			// 모든 버텍스가 공용으로 사용하는 색상값

	/* 셰이더 버퍼들 ======================*/
	ID3D11Buffer*			m_pWorldBuffer;		// 월드 매트릭스 버퍼
	ID3D11InputLayout*		m_pVertexLayout;	// 입력 레이아웃 설정값
	ID3D11Buffer*			m_pVertexBuffer;	// 버텍스 버퍼
	ID3D11Buffer*			m_pIndexBuffer;		// 인덱스 버퍼
	ID3D11Buffer*			m_pColorBuffer;		// 색상 버퍼
	/*====================================*/

	Shader*					m_pShader;			// 셰이더 클래스

	Shader*					m_pBorderShader;	// 보더라인 셰이더 클래스
	ID3D11Buffer*			m_pBorderBuffer;	// 보더라인 버텍스 버퍼
	UINT					m_nBVCount;			// 보더라인 버텍스 갯수
	bool					m_isDrawBorder;

protected:
	void CreateBorderVertexBuffer(int start);

public:
	Primitive2DObejct();
	virtual ~Primitive2DObejct();

	virtual void Init();
	virtual void Update();
	virtual void Render();

	void DrawBorder();

	void SetPosition(D3DXVECTOR2 p) { m_vPosition = p; }
	void SetPosition(float x, float y) { m_vPosition = D3DXVECTOR2(x, y); }
	D3DXVECTOR2 GetPosition() { return m_vPosition; }

	void SetSize(D3DXVECTOR2 s) { m_vSize = s; }
	void SetSize(float x, float y) { m_vSize = D3DXVECTOR2(x, y); }
	D3DXVECTOR2 GetSize() { return m_vSize; }

	void SetColor(D3DXCOLOR c) { m_stColor = c; }
	void SetBorder(bool b) { m_isDrawBorder = b; }
};

template<typename T>
inline Primitive2DObejct<T>::Primitive2DObejct()
{
}

template<typename T>
inline Primitive2DObejct<T>::~Primitive2DObejct()
{
	SAFE_RELEASE(m_pBorderBuffer);
	SAFE_DELETE(m_pBorderShader);
	SAFE_DELETE(m_pShader);

	SAFE_RELEASE(m_pColorBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pWorldBuffer);
}

template<typename T>
inline void Primitive2DObejct<T>::CreateBorderVertexBuffer(int start)
{
	m_pBorderShader = new Shader(L"Color");
	m_pBorderShader->CreateinputLayout(PCElementDesc, PCElementCount);

	vector<PCVertex> vecVertex;

	for (int i = start; i < m_vecVertex.size(); i++)
		vecVertex.push_back(PCVertex(m_vecVertex[i].Position, D3DXCOLOR(1, 0, 0, 1)));
	
	vecVertex.push_back(PCVertex(m_vecVertex[start].Position, D3DXCOLOR(1, 0, 0, 1)));

	m_nBVCount = vecVertex.size();

	// 버퍼 설정값
	D3D11_BUFFER_DESC desc = { 0, };
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(PCVertex) * vecVertex.size();
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	// 버퍼에 담을 데이터
	D3D11_SUBRESOURCE_DATA data = { 0, };
	data.pSysMem = &vecVertex[0];

	// 버퍼 생성(IN 버퍼 설정값, IN 데이터, OUT 만들어진 버퍼에 대한 포인터)
	HRESULT hr = Device->CreateBuffer(&desc, &data, &m_pBorderBuffer);
	assert(SUCCEEDED(hr));
}

template<typename T>
inline void Primitive2DObejct<T>::Init()
{
	D3DXMatrixIdentity(&m_matWorld);
	m_vPosition = { 50, 50 };
	m_vSize = { 100, 100 };

	m_isDrawBorder = false;
}

template<typename T>
inline void Primitive2DObejct<T>::Update()
{
	D3DXMATRIX Scale, Translate;
	
	D3DXMatrixScaling(&Scale, m_vSize.x, m_vSize.y, 1);
	D3DXMatrixTranslation(&Translate, m_vPosition.x, m_vPosition.y, 0);
	
	m_matWorld = Scale * Translate;
	D3DXMatrixTranspose(&m_matWorld, &m_matWorld);
}

template<typename T>
inline void Primitive2DObejct<T>::Render()
{
	m_pShader->SetShader();

	DeviceContext->UpdateSubresource(m_pWorldBuffer, 0, 0, &m_matWorld, 0, 0);
	DeviceContext->VSSetConstantBuffers(1, 1, &m_pWorldBuffer);
	DeviceContext->UpdateSubresource(m_pColorBuffer, 0, 0, &m_stColor, 0, 0);
	DeviceContext->PSSetConstantBuffers(0, 1, &m_pColorBuffer);

	// mycode
	DeviceContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_vecVertex[0], 0, 0);

	UINT stride = sizeof(T);
	UINT offset = 0;

	DeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	DeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DeviceContext->DrawIndexed(m_vecIndex.size(), 0, 0);

	//if (::g_isDrawBorder)
	//{
	//	DrawBorder(stride, offset);
	//}
}

template<typename T>
void Primitive2DObejct<T>::DrawBorder()
{
	m_pShader->SetShader();

	DeviceContext->UpdateSubresource(m_pWorldBuffer, 0, 0, &m_matWorld, 0, 0);
	DeviceContext->VSSetConstantBuffers(1, 1, &m_pWorldBuffer);
	DeviceContext->UpdateSubresource(m_pColorBuffer, 0, 0, &m_stColor, 0, 0);
	DeviceContext->PSSetConstantBuffers(0, 1, &m_pColorBuffer);

	// mycode
	DeviceContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_vecVertex[0], 0, 0);

	UINT stride = sizeof(T);
	UINT offset = 0;

	DeviceContext->UpdateSubresource(m_pColorBuffer, 0, 0, &D3DXCOLOR(1, 0, 0, 1), 0, 0);
	DeviceContext->IASetVertexBuffers(0, 1, &m_pBorderBuffer, &stride, &offset);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	DeviceContext->Draw(m_nBVCount, 0);
}
