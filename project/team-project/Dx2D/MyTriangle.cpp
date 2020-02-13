#include "stdafx.h"
#include "MyTriangle.h"

MyTriangle::MyTriangle()
{
}

MyTriangle::MyTriangle(vector<PCVertex> vecVertex)
{
	Primitive2DObejct::Init();
	Init();

	// 월드 매트릭스 셋팅 및 버퍼 생성
	{
		D3D11_BUFFER_DESC desc = { 0, };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(D3DXMATRIX);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		// 버퍼에 담을 데이터
		D3D11_SUBRESOURCE_DATA data = { 0, };
		data.pSysMem = &m_matWorld;

		HRESULT hr = Device->CreateBuffer(&desc, &data, &m_pWorldBuffer);
		assert(SUCCEEDED(hr));
	}

	// 버텍스 값 셋팅 및 버퍼 생성
	{
		m_vecVertex.push_back(vecVertex[0]); // 0
		m_vecVertex.push_back(vecVertex[1]); // 1
		m_vecVertex.push_back(vecVertex[2]); // 2

		// 버퍼 설정값
		D3D11_BUFFER_DESC desc = { 0, };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(PCVertex) * m_vecVertex.size();
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		// 버퍼에 담을 데이터
		D3D11_SUBRESOURCE_DATA data = { 0, };
		data.pSysMem = &m_vecVertex[0];

		// 버퍼 생성(IN 버퍼 설정값, IN 데이터, OUT 만들어진 버퍼에 대한 포인터)
		HRESULT hr = Device->CreateBuffer(&desc, &data, &m_pVertexBuffer);
		assert(SUCCEEDED(hr));
	}

	// 인덱스 값 셋팅 및 버퍼 생성
	{
		m_vecIndex.push_back(0);
		m_vecIndex.push_back(1);
		m_vecIndex.push_back(2);

		// 버퍼 설정값
		D3D11_BUFFER_DESC desc = { 0, };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(UINT) * m_vecIndex.size();
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		// 버퍼에 담을 데이터
		D3D11_SUBRESOURCE_DATA data = { 0, };
		data.pSysMem = &m_vecIndex[0];

		// 버퍼 생성(IN 버퍼 설정값, IN 데이터, OUT 만들어진 버퍼에 대한 포인터)
		HRESULT hr = Device->CreateBuffer(&desc, &data, &m_pIndexBuffer);
		assert(SUCCEEDED(hr));
	}

	// 색상 값 설정 및 버퍼 생성
	{
		m_stColor = D3DXCOLOR(0, 0, 0, 0);

		// 버퍼 설정값
		D3D11_BUFFER_DESC desc = { 0, };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(D3DXCOLOR);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		// 버퍼에 담을 데이터
		D3D11_SUBRESOURCE_DATA data = { 0, };
		data.pSysMem = &m_stColor;

		// 버퍼 생성(IN 버퍼 설정값, IN 데이터, OUT 만들어진 버퍼에 대한 포인터)
		HRESULT hr = Device->CreateBuffer(&desc, &data, &m_pColorBuffer);
		assert(SUCCEEDED(hr));
	}

	// 셰이더 클래스 생성
	{
		m_pShader = new Shader(L"Color");
		m_pShader->CreateinputLayout(PCElementDesc, PCElementCount);
	}

	// 보더라인 버퍼 생성
	{
		this->CreateBorderVertexBuffer(0);
	}
}


MyTriangle::~MyTriangle()
{
	Primitive2DObejct::~Primitive2DObejct();
}

void MyTriangle::Init()
{
}

void MyTriangle::Update()
{
	Primitive2DObejct::Update();
}

void MyTriangle::Render()
{
	Primitive2DObejct::Render();
}
