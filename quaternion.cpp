#include "quaternion.h"

#define EPSILON 0.000001f

using namespace DirectX;

SEQuaternion::SEQuaternion() :s(1.0f) {
}

SEQuaternion::SEQuaternion(const SEQuaternion & rhs) : s(rhs.s), v(rhs.v) {
}

SEQuaternion::SEQuaternion(float _s, FXMVECTOR _v):s(_s){
	XMStoreFloat3(&v, _v);
}

SEQuaternion::SEQuaternion(FXMVECTOR _v):s(1.0f){
	XMStoreFloat3(&v, _v);
}

SEQuaternion::SEQuaternion(float x, float y, float z):s(1.0f), v(XMFLOAT3(x,y,z)) {
}

SEQuaternion::SEQuaternion(float _s, float x, float y, float z) : s(_s), v(XMFLOAT3(x, y, z)) {
}

SEQuaternion & SEQuaternion::operator=(const SEQuaternion & rhs) {
	s = rhs.s;
	v = rhs.v;
	return *this;
}

SEQuaternion  SEQuaternion::operator-() const{
	return *this * (-1);
}

SEQuaternion SEQuaternion::operator+(const SEQuaternion &rhs)  const {
	return SEQuaternion(*this) += rhs;
}

SEQuaternion & SEQuaternion::operator+=(const SEQuaternion & rhs) {
	XMVECTOR v1 = XMLoadFloat3(&v);
	XMVECTOR v2 = XMLoadFloat3(&rhs.v);

	s += s;
	v1 += v2;
	XMStoreFloat3(&v, v1);

	return *this;
}

SEQuaternion SEQuaternion::operator-(const SEQuaternion & rhs)  const {
	return SEQuaternion(*this) -= rhs;
}

SEQuaternion & SEQuaternion::operator-=(const SEQuaternion & rhs) {
	return *this += (-rhs);
}

SEQuaternion SEQuaternion::operator*(const SEQuaternion & rhs)  const {
	return SEQuaternion(*this) *= rhs;
}

SEQuaternion & SEQuaternion::operator*=(const SEQuaternion & rhs) {
	XMVECTOR v1 = XMLoadFloat3(&v);
	XMVECTOR v2 = XMLoadFloat3(&rhs.v);
	float s1 = s;
	float s2 = rhs.s;

	XMFLOAT3 f3;
	XMStoreFloat3(&f3, XMVector3Dot(v1, v2));
	s = s1*s2 - f3.x;
	XMStoreFloat3(&v, s1 * v2 + s2 * v1 + XMVector3Cross(v1,v2));
	
	return *this;
}

SEQuaternion SEQuaternion::operator*(float c)  const {
	return SEQuaternion(*this) *= c;
}

SEQuaternion & SEQuaternion::operator*=(float c) {
	XMVECTOR v1 = XMLoadFloat3(&v);
	s *= c;
	v1 *= c;
	XMStoreFloat3(&v, v1);
	return *this;
}

SEQuaternion SEQuaternion::operator/(float c) const {
	return SEQuaternion(*this) /= c;
}

SEQuaternion & SEQuaternion::operator/=(float c) {
	return *this *= 1.0f / c;
}

SEQuaternion SEQuaternion::conj() const {
	XMVECTOR v1 = XMLoadFloat3(&v);
	SEQuaternion result;

	result.s = s;
	XMStoreFloat3(&result.v, -v1);

	return result;
}

float SEQuaternion::norm() const {
	XMVECTOR v1 = XMLoadFloat3(&v);
	v1 = XMVector3LengthSq(v1);
	XMFLOAT3 f3;
	XMStoreFloat3(&f3, v1);
	return s*s + f3.x;
}

SEQuaternion SEQuaternion::inverse() const {
	return this->conj() / this->norm();
}

float SEQuaternion::getReal() const {
	return s;
}

XMVECTOR SEQuaternion::getImagine() const {
	return XMLoadFloat3(&v);
}

float SEQuaternion::dot(const SEQuaternion & rhs) const {
	XMVECTOR v1 = XMLoadFloat3(&v);
	XMVECTOR v2 = XMLoadFloat3(&rhs.v);
	v1 = XMVector3Dot(v1, v2);
	XMFLOAT3 f3;
	XMStoreFloat3(&f3, v1);
	return s * rhs.s + f3.x;
}

XMVECTOR SEQuaternion::rotate(FXMVECTOR element) const {
	SEQuaternion mid(0.0f, element);
	return ((*this) * mid * this->inverse()).getImagine();
}

bool SEQuaternion::isUnit() const {
	return fabsf(this->norm() - 1.0f) < EPSILON;
}

SEQuaternion & SEQuaternion::identify() {
	s = 1.0f;
	v = XMFLOAT3(0.0f, 0.0f, 0.0f);
	return *this;
}

XMMATRIX SEQuaternion::toMatrix() const {
	float x2 = v.x*v.x;
	float y2 = v.y*v.y;
	float z2 = v.z*v.z;
	float xy = v.x*v.y;
	float yz = v.y*v.z;
	float xz = v.x*v.z;
	float sx = s*v.x;
	float sy = s*v.y;
	float sz = s*v.z;
	XMFLOAT4X4 matrix(
		1.0f-2.0f*(y2+z2),	2.0f*(xy-sz),		2.0f*(xz+sy),		0.0f,
		2.0f*(xy+sz),		1.0f-2.0f*(x2+z2),	2.0f*(yz-sx),		0.0f,
		2.0f*(xz-sy),		2.0f*(yz+sx),		1.0f-2.0f*(x2+y2),	0.0f,
		0.0f,				0.0f,				0.0f,				1.0f
	);
	return XMLoadFloat4x4(&matrix);

}

DirectX::XMVECTOR SEQuaternion::get() const {
	return XMLoadFloat4(&XMFLOAT4(v.x, v.y, v.z, s));
}

DirectX::XMVECTOR SEQuaternion::toEuler() const {

	float y2 = v.y * v.y;
	float t0 = -2.0f * (y2 + v.z * v.z) + 1.0f;
	float t1 = +2.0f * (v.x * v.y + s * v.z);
	float t2 = -2.0f * (v.x * v.z - s * v.y);
	float t3 = +2.0f * (v.y * v.z + s * v.x);
	float t4 = -2.0f * (v.x * v.x + y2) + 1.0f;

	t2 = t2 > 1.0f ? 1.0f : t2;
	t2 = t2 < -1.0f ? -1.0f : t2;

	float x = atan2f(t3, t4);
	float y = asinf(t2);
	float z = atan2f(t1, t0);

	return XMLoadFloat3(&XMFLOAT3(x, y, z));
}

SEQuaternion SEQuaternion::fromMatrix(CXMMATRIX matrix) {
													 
	SEQuaternion result;
	XMFLOAT4X4 float4x4;
	XMStoreFloat4x4(&float4x4, matrix);
	float s = sqrtf(float4x4._11 + float4x4._22 + float4x4._33 + 1.0f);
	return SEQuaternion(
		s,
		(float4x4._32 - float4x4._23) / 4.0f / s,
		(float4x4._13 - float4x4._31) / 4.0f / s,
		(float4x4._21 - float4x4._12) / 4.0f / s);
}

SEQuaternion SEQuaternion::fromEuler(DirectX::FXMVECTOR euler) {
	XMFLOAT3 f3;
	XMStoreFloat3(&f3, euler * 0.5f);
	double t0 = cosf(f3.z);
	double t1 = sinf(f3.z);
	double t2 = cosf(f3.x);
	double t3 = sinf(f3.x);
	double t4 = cosf(f3.y);
	double t5 = sinf(f3.y);

	return SEQuaternion(
		t0 * t2 * t4 + t1 * t3 * t5,
		t0 * t3 * t4 - t1 * t2 * t5,
		t0 * t2 * t5 + t1 * t3 * t4,
		t1 * t2 * t4 - t0 * t3 * t5);
}

SEQuaternion operator*(float c, SEQuaternion q) {
	return q*c;
}
