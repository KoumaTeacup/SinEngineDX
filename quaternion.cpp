#include "quaternion.h"

#define EPSILON 0.000001f

using namespace DirectX;

SEQuaternion::SEQuaternion() :s(0.0f) {
}

SEQuaternion::SEQuaternion(const SEQuaternion & rhs) : s(rhs.s), v(rhs.v) {
}

SEQuaternion::SEQuaternion(float _s, DirectX::XMFLOAT3 _v):s(_s), v(_v){
}

SEQuaternion::SEQuaternion(DirectX::XMFLOAT3 _v):s(0.0f), v(_v){
}

SEQuaternion::SEQuaternion(float x, float y, float z):s(0.0f), v(XMFLOAT3(x,y,z)) {
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

float SEQuaternion::dot(const SEQuaternion & rhs) const {
	XMVECTOR v1 = XMLoadFloat3(&v);
	XMVECTOR v2 = XMLoadFloat3(&rhs.v);
	v1 = XMVector3Dot(v1, v2);
	XMFLOAT3 f3;
	XMStoreFloat3(&f3, v1);
	return s * rhs.s + f3.x;
}

bool SEQuaternion::isUnit() const {
	return fabsf(this->norm() - 1.0f) < EPSILON;
}

SEQuaternion & SEQuaternion::identify() {
	s = 1;
	v = XMFLOAT3(0.0f, 0.0f, 0.0f);
	return *this;
}

SEQuaternion SEQuaternion::rotateBy(const SEQuaternion & rhs) const {
	return SEQuaternion(rhs * (*this) * rhs.inverse());
}

SEQuaternion & SEQuaternion::rotateBy(const SEQuaternion & rhs) {
	return *this = SEQuaternion(rhs * (*this) * rhs.inverse());
}

SEQuaternion SEQuaternion::fromMatrix(float matrix) {/*
													 
	SEQuaternion result;
	XMFLOAT4X4 float4x4;
	XMStoreFloat4x4(&float4x4, matrix);
	float s = sqrtf(float4x4._11 + float4x4._22 + float4x4._33 + 1.0f);
	return SEQuaternion(
		s,
		(float4x4._32 - float4x4._23) / 4.0f / s,
		(float4x4._13 - float4x4._31) / 4.0f / s,
		(float4x4._21 - float4x4._12) / 4.0f / s);*/
	return SEQuaternion();
}

SEQuaternion operator*(float c, SEQuaternion q) {
	return q*c;
}
