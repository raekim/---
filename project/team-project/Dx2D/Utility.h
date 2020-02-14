﻿#pragma once
/* Utility */

// 삼각형과 사각형, 원 충돌
// 참조 1 : https://helloneo.pe.kr/393 (선분과 점 거리)
// 참조 2 : https://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle (점이 삼각형 안에 있음 확임)

// 점(point)과 선분(p1, p2가 끝점)의 거리
inline float GetDistanceBetweenPointAndLineSegment(D3DXVECTOR2 p, D3DXVECTOR2 s, D3DXVECTOR2 e)
{
	D3DXVECTOR2 sp = p - s;	// 점 s와 p를 잇는 벡터
	D3DXVECTOR2 se = e - s;	// 점 s와 e를 잇는 벡터
	D3DXVECTOR2 ep = p - e;	// 점 e와 p를 잇는 벡터

	if (D3DXVec2Dot(&sp, &se) * D3DXVec2Dot(&ep, &se) <= 0)
	{
		/* foot exists.  point 로부터 선분으로 내린 수선의 발이 존재 */
		D3DXVECTOR3 crossRes = { 0,0,0 };	// 벡터 외적 결과
		D3DXVECTOR3 sp3 = { sp.x, sp.y, 0 };
		D3DXVECTOR3 se3 = { se.x, se.y, 0 };
		crossRes.z = 0;
		D3DXVec3Cross(&crossRes, &sp3, &se3);
		return D3DXVec3Length(&crossRes) / D3DXVec2Length(&se);
	}
	else
	{
		return min(D3DXVec2Length(&sp), D3DXVec2Length(&ep));
	}
}

// p1:사각형의 중심, s1:사각형의 사이즈
inline bool CircleRectCollision(D3DXVECTOR2 circleCenter, float circleRadius, D3DXVECTOR2 p1, D3DXVECTOR2 s1)
{
	float L1 = p1.x - s1.x * 0.5f;
	float R1 = L1 + s1.x; // R1 = p1.x + s1.x * 0.5f;
	float T1 = p1.y + s1.y * 0.5f;
	float B1 = T1 - s1.y;

	// 충돌 case 1 : 원의 중심이 사각형 안에 있는 경우 
	if (circleCenter.x >= L1 && circleCenter.x <= R1 &&
		circleCenter.y >= B1 && circleCenter.y <= T1)
		return true;

	// 충돌 case 2 : 원이 사각형의 어떤 모서리를 넘었을 경우
	// (원의 중심에서 사각형의 모서리까지의 거리(수선의 발의 거리)) <= (원의 반지름) 일 경우 모서리를 넘은것이다
	{
		float d;	// 점에서 모서리까지의 거리
		D3DXVECTOR2 r1, r2;

		// 왼쪽 모서리
		r1 = {L1, T1}; // 왼쪽 위 점
		r2 = {L1, B1};	// 왼쪽 아래 점
		d = GetDistanceBetweenPointAndLineSegment(circleCenter, r1, r2);
		if (d <= circleRadius) return true;

		// 오른쪽 모서리
		r1 = { R1, T1 };	// 오른쪽 위 점
		r2 = { R1, B1 };	// 오른쪽 아래 점
		d = GetDistanceBetweenPointAndLineSegment(circleCenter, r1, r2);
		if (d <= circleRadius) return true;

		// 위 모서리
		r1 = { L1, T1 };	// 왼쪽 위 점
		r2 = { R1, T1 };	// 오른쪽 위 점
		d = GetDistanceBetweenPointAndLineSegment(circleCenter, r1, r2);
		if (d <= circleRadius) return true;

		// 아래 모서리
		r1 = { L1, B1 };	// 왼쪽 아래 점
		r2 = { R1, B1 };	// 오른쪽 아래 점
		d = GetDistanceBetweenPointAndLineSegment(circleCenter, r1, r2);
		if (d <= circleRadius) return true;
	}

	return false;
}


inline bool sign(D3DXVECTOR2 p1, D3DXVECTOR2 p2, D3DXVECTOR2 p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

inline bool PointInTriangle(D3DXVECTOR2 pt, D3DXVECTOR2 v1, D3DXVECTOR2 v2, D3DXVECTOR2 v3)
{
	float d1, d2, d3;
	bool has_neg, has_pos;

	d1 = sign(pt, v1, v2);
	d2 = sign(pt, v2, v3);
	d3 = sign(pt, v3, v1);

	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(has_neg && has_pos);
}

// p1, p2, p3 : 삼각형의 점들
inline bool CircleTriangleCollision(D3DXVECTOR2 circleCenter, float circleRadius, D3DXVECTOR2 p1, D3DXVECTOR2 p2, D3DXVECTOR2 p3)
{
	// 충돌 case 1 : 원의 중심이 삼각형 안에 있는 경우 
	//if (PointInTriangle(circleCenter, p1, p2, p3))
		//return true;

	// 충돌 case 2 : 원이 삼각형의 어떤 모서리를 넘었을 경우
	{
		float d;	// 점에서 모서리까지의 거리
		D3DXVECTOR2 r1, r2;

		// 모서리 1
		r1 = p1;
		r2 = p2;
		d = GetDistanceBetweenPointAndLineSegment(circleCenter, r1, r2);
		if (d <= circleRadius) return true;

		// 모서리 2
		r1 = p2;
		r2 = p3;
		d = GetDistanceBetweenPointAndLineSegment(circleCenter, r1, r2);
		if (d <= circleRadius) return true;

		// 모서리 3
		r1 = p3;
		r2 = p1;
		d = GetDistanceBetweenPointAndLineSegment(circleCenter, r1, r2);
		if (d <= circleRadius) return true;
	}

	return false;
}

inline bool PointInCircle(D3DXVECTOR2 p1, float r1, D3DXVECTOR2 point)
{
	float distX = p1.x - point.x;
	float distY = p1.y - point.y;

	float dist = sqrtf(distX * distX + distY * distY);

	if (dist > r1)
		return false;

	return true;
}

inline bool PointInRect(D3DXVECTOR2 p1, D3DXVECTOR2 s1, D3DXVECTOR2 point)
{
	float L1 = p1.x - s1.x * 0.5f;
	float R1 = L1 + s1.x; // R1 = p1.x + s1.x * 0.5f;
	float T1 = p1.y + s1.y * 0.5f;
	float B1 = T1 - s1.y;

	if (point.x < L1 || point.x > R1 ||
		point.y < B1 || point.y > T1)
		return false;

	return true;
}

inline bool CircleCollision(D3DXVECTOR2 p1, float r1, D3DXVECTOR2 p2, float r2)
{
	/*
	원 충돌 : 두 점 사이의 거리가 두 원의 반지름을 더한 값보다 작다면 두 원은 충돌 상태
	*/

	float distX = p1.x - p2.x;
	float distY = p1.y - p2.y;

	float distance = sqrtf(distX * distX + distY * distY);

	if (r1 + r2 < distance)
		return false;

	return true;
}

inline bool AABB2D(D3DXVECTOR2 p1, D3DXVECTOR2 s1, D3DXVECTOR2 p2, D3DXVECTOR2 s2)
{
	float L1 = p1.x	- s1.x * 0.5f;
	float R1 = L1	+ s1.x; // R1 = p1.x + s1.x * 0.5f;
	float T1 = p1.y	+ s1.y * 0.5f;
	float B1 = T1	- s1.y;

	float L2 = p2.x	- s2.x * 0.5f;
	float R2 = L2	+ s2.x;
	float T2 = p2.y	+ s2.y * 0.5f;
	float B2 = T2	- s2.y;

	if (R1 < L2 || L1 > R2)
		return false;

	if (B1 > T2 || T1 < B2)
		return false;

	return true;
}

inline bool AABB2D(RECT& rect1, RECT& rect2)
{
	if (rect1.right < rect2.left || rect1.left > rect2.right)
		return false;

	if (rect1.bottom > rect2.top || rect1.top < rect2.bottom)
		return false;

	return true;
}

// 기준축, A도형 중점, A도형 반지름, B도형 중점, B도형 좌표값
// A도형 : 기준축에 평행변을 가지고 있는 도형
inline bool CheckOBB2D(D3DXVECTOR2& PivotDir, D3DXVECTOR2& A_Center, float& A_Half,
	D3DXVECTOR2 B_Center, D3DXVECTOR2 v[4])
{
	// 두 센터에 대한 투영값
	float projectA_C = D3DXVec2Dot(&PivotDir, &A_Center);
	float projectB_C = D3DXVec2Dot(&PivotDir, &B_Center);

	// 두 센터의 투영값의 거리
	float length_C = fabs(projectA_C - projectB_C);

	// B도형의 각 좌표에 대한 투영 값중 가장 작은 값을 계산
	float projectB_Left = D3D11_FLOAT32_MAX;
	for (int i = 0; i < 4; i++)
	{
		float length = D3DXVec2Dot(&PivotDir, &v[i]);

		if (length < projectB_Left)
			projectB_Left = length;
	}

	// B도형의 기준벡터에 대한 반지름값
	float B_Half = fabs(projectB_Left - projectB_C);

	return length_C <= (A_Half + B_Half);
}

inline bool OBB2D(D3DXMATRIX& matA, D3DXMATRIX& matB)
{
	// 처음 설계 기본 좌표 값은 0기준 사이즈 1짜리이다.
	D3DXVECTOR2 vA[4], vB[4];
	vA[0] = D3DXVECTOR2(-0.5f,-0.5f);
	vA[1] = D3DXVECTOR2(-0.5f, 0.5f);
	vA[2] = D3DXVECTOR2( 0.5f, 0.5f);
	vA[3] = D3DXVECTOR2( 0.5f,-0.5f);

	// 월드(SRT)매트릭스를 사용하여 현재 화면에서의 좌표값을 계산
	for (int i = 0; i < 4; i++)
	{
		D3DXVec2TransformCoord(&vB[i], &vA[i], &matB);
		D3DXVec2TransformCoord(&vA[i], &vA[i], &matA);
	}

	// 두 사각형의 중점 계산
	D3DXVECTOR2 vA_Center((vA[0] + vA[2]) * 0.5f);
	D3DXVECTOR2 vB_Center((vB[0] + vB[2]) * 0.5f);

	// 기준축 벡터 계산1
	D3DXVECTOR2 vA_01 = vA[1] - vA[0];
	D3DXVECTOR2 vA_03 = vA[3] - vA[0];
	D3DXVECTOR2 vB_01 = vB[1] - vB[0];
	D3DXVECTOR2 vB_03 = vB[3] - vB[0];

	// 사각형 한변의 절반 크기 (기준축 평행변)
	float fA_01_Half = D3DXVec2Length(&vA_01) * 0.5f;
	float fA_03_Half = D3DXVec2Length(&vA_03) * 0.5f;
	float fB_01_Half = D3DXVec2Length(&vB_01) * 0.5f;
	float fB_03_Half = D3DXVec2Length(&vB_03) * 0.5f;

	// 기준축 벡터 계산2
	D3DXVec2Normalize(&vA_01, &vA_01);
	D3DXVec2Normalize(&vA_03, &vA_03);
	D3DXVec2Normalize(&vB_01, &vB_01);
	D3DXVec2Normalize(&vB_03, &vB_03);

	// 각 기준축 별로 충돌 검사하는 방식이 같기 때문에 함수화하여 사용
	if (!CheckOBB2D(vA_01, vA_Center, fA_01_Half, vB_Center, vB))
		return false;

	if (!CheckOBB2D(vA_03, vA_Center, fA_03_Half, vB_Center, vB))
		return false;

	if (!CheckOBB2D(vB_01, vB_Center, fB_01_Half, vA_Center, vA))
		return false;

	if (!CheckOBB2D(vB_03, vB_Center, fB_03_Half, vA_Center, vA))
		return false;

	return true;
}

inline float GetAngle(D3DXVECTOR2 p1, D3DXVECTOR2 p2) // p1 -> p2
{
	/*
	p1 에서 p2 방향의 각도 값 = theta
	cos(theta) = (x2 - x1) / 대각선 길이
	theta = cos^-1( (x2 - x1) / 대각선 길이 )
	theta = acos( (x2 - x1) / 대각선 길이 )
	*/

	float distX = p2.x - p1.x;
	float distY = p2.y - p1.y;

	float distance = sqrtf(distX * distX + distY * distY);

	float result = acosf(distX / distance);

	if (p2.y < p1.y)
		result = D3DX_PI * 2 - result;

	// 서클 클래스가 사용하는 앵글값은 Degree 값이고
	// cos, sin 함수들이 사용하는 값은 Radian 값이다.
	return D3DXToDegree(result);
}