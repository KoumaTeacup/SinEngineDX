#include "spline.h"
#include "framework.h"

using namespace std;
using namespace DirectX;

float truncFunc(float t, int i) {
	if (t < i) return 0;
	float res = t - i;
	return res*res*res;
}

void RowAdd(vector<vector<float>> &matrix, int toRow, int fromRow, float fromRowScale)
{
	for (unsigned i = 0; i < matrix[toRow].size(); i++)
		matrix[toRow][i] += matrix[fromRow][i] * fromRowScale;
}

void RowMultiply(vector<vector<float>> &matrix, int row, float scalar)
{
	for (unsigned i = 0; i < matrix[row].size(); i++)
		matrix[row][i] *= scalar;
}

void RowReduce(vector<vector<float>> &matrix, int len)
{
	int lead = 0;
	int rowCount = len;
	int colCount = len + 1;

	for (int r = 0; r < rowCount; ++r)
	{
		if (colCount <= lead)
			return;

		int i = r;
		while (matrix[i][lead] == 0)
		{
			++i;
			if (i == rowCount)
			{
				i = r;
				++lead;
				if (colCount == lead)
					return;
			}
		}

		// Swap rows
		auto tmp = matrix[i];
		matrix[i] = matrix[r];
		matrix[r] = tmp;

		if (matrix[r][lead] != 0)
			RowMultiply(matrix, r, 1.f / matrix[r][lead]);

		for (int i = 0; i < rowCount; ++i)
		{
			if (i != r)
				RowAdd(matrix, i, r, -matrix[i][lead]);
		}

		++lead;
	}
}

SESpline::SESpline() : smoothness(5), buffer(nullptr)
{
	setType(SE_ASSET_SPLINE);
}

SESpline::~SESpline()
{
	ReleaseCOM(buffer);
}

void SESpline::loadCP(const int num, const CurveControlPoint * _cpList)
{
	for (int i = 0; i < num; i++) {
		CPList.push_back(_cpList[i]);
	}
	initializeLineSeg();
}

void SESpline::loadCP(const int num, const XMFLOAT3 * _cpList)
{
	for (int i = 0; i < num; i++) {
		CPList.push_back(CurveControlPoint( _cpList[i]));
	}
	initializeLineSeg();
}

void SESpline::Tick(float dt) {
	SEAsset::Tick(dt);
}

void SESpline::Bind()
{
	UINT stride = sizeof(XMFLOAT3);
	UINT offset = 0;
	SEContext->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
}

void SESpline::Draw()
{
	SEAsset::Draw();
	SE_Shader->UpdateConstantBuffer();

	Bind();

	D3D11_PRIMITIVE_TOPOLOGY pt;
	SEContext->IAGetPrimitiveTopology(&pt);
	SEContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	SEContext->Draw((CPList.size() - 1) * smoothness + 1, 0);
	SEContext->IASetPrimitiveTopology(pt);
}

XMVECTOR SESpline::computeCurve(float t) {
	t *= CPList.size() - 1;

	XMVECTOR vec4 = XMVectorSet(1, t, t*t, t*t*t);
	XMFLOAT4 tempVec;
	XMStoreFloat4(&tempVec, XMVector4Dot(vec4, splineFactorX));
	float x = tempVec.x;
	XMStoreFloat4(&tempVec, XMVector4Dot(vec4, splineFactorY));
	float y = tempVec.y;
	XMStoreFloat4(&tempVec, XMVector4Dot(vec4, splineFactorZ));
	float z = tempVec.z;

	for(unsigned l = 0; l < coefX.size(); l++) {
		x += coefX[l] * truncFunc(t, l + 1);
		y += coefY[l] * truncFunc(t, l + 1);
		z += coefZ[l] * truncFunc(t, l + 1);
	}

	return XMVectorSet(x, y, z, 0.0f);
}

void SESpline::initializeLineSeg()
{
	int len = CPList.size();
	if (len < 2)return;

	vector<vector<float>> homoEquationX(len + 2, vector<float>(len + 3, 0.0f));
	vector<vector<float>> homoEquationY(len + 2, vector<float>(len + 3, 0.0f));
	vector<vector<float>> homoEquationZ(len + 2, vector<float>(len + 3, 0.0f));
	int i, j;

	for(i = 0; i < len; i++) {
		for(j = 0; j < 4; j++)
			// a0 + a1*t + a2*t^2 + a3*t^3
			homoEquationX[i][j] = homoEquationY[i][j] = homoEquationZ[i][j] = (float)pow(i, j);
		for(j = 4; j < len + 2; j++)
			// b1*[t-1] + b2*[t-2] + ... + bk-1*[t-(k-1)]
			homoEquationX[i][j] = homoEquationY[i][j] = homoEquationZ[i][j] = truncFunc(float(i), j - 3);
		homoEquationX[i][j] = CPList[i].pos.x;
		homoEquationY[i][j] = CPList[i].pos.y;
		homoEquationZ[i][j] = CPList[i].pos.z;
	}

	homoEquationX[i][2] = homoEquationY[i][2] = homoEquationZ[i][2] = 2.f;
	homoEquationX[i + 1][2] = homoEquationY[i + 1][2] = homoEquationZ[i + 1][2] = 2.f;
	homoEquationX[i + 1][3] = homoEquationY[i + 1][3] = homoEquationZ[i + 1][3] = 6.0f*(len-1.0f);

	for (j = 4; j < len + 2; j++) 
		homoEquationX[i + 1][j] = homoEquationY[i + 1][j] = homoEquationZ[i + 1][j] = float(6 * XMMax(0, len + 2 - j));

	RowReduce(homoEquationX, len + 2);
	RowReduce(homoEquationY, len + 2);
	RowReduce(homoEquationZ, len + 2);

	splineFactorX = XMVectorSet(homoEquationX[0][len + 2], homoEquationX[1][len + 2], homoEquationX[2][len + 2], homoEquationX[3][len + 2]);
	splineFactorY = XMVectorSet(homoEquationY[0][len + 2], homoEquationY[1][len + 2], homoEquationY[2][len + 2], homoEquationY[3][len + 2]);
	splineFactorZ = XMVectorSet(homoEquationZ[0][len + 2], homoEquationZ[1][len + 2], homoEquationZ[2][len + 2], homoEquationZ[3][len + 2]);

	if (buffer) ReleaseCOM(buffer);

	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = sizeof(XMFLOAT3) * ((CPList.size()-1) * smoothness + 1);
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.StructureByteStride = 0;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;

	coefX.clear();
	coefY.clear();
	coefZ.clear();
	for(int l = 4; l < len + 2; l++) {
		coefX.push_back(homoEquationX[l][len + 2]);
		coefY.push_back(homoEquationY[l][len + 2]);
		coefZ.push_back(homoEquationZ[l][len + 2]);
	}

	vector<CurveData> curveData;
	float step = 1.0f / smoothness;
	for(float t = 0; t <= len - 1 + 0.00001;) {
		XMVECTOR vec4 = XMVectorSet(1, t, t*t, t*t*t);
		XMFLOAT4 tempVec;
		XMStoreFloat4(&tempVec, XMVector4Dot(vec4, splineFactorX));
		float x = tempVec.x;
		XMStoreFloat4(&tempVec, XMVector4Dot(vec4, splineFactorY));
		float y = tempVec.y;
		XMStoreFloat4(&tempVec, XMVector4Dot(vec4, splineFactorZ));
		float z = tempVec.z;

		for(int l = 0; l < len - 2; l++) {
			x += coefX[l] * truncFunc(t, l + 1);
			y += coefY[l] * truncFunc(t, l + 1);
			z += coefZ[l] * truncFunc(t, l + 1);
		}
		curveData.push_back(CurveData(x, y, z));
		t += step;
	}
	sd.pSysMem = &curveData[0];

	SEDevice->CreateBuffer(&bd, &sd, &buffer);
}