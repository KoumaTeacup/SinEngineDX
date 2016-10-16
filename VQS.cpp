#include "VQS.h"

using namespace DirectX;

SEVQS::SEVQS(): v(0.0f, 0.0f, 0.0f), q(), s(1.0f) {
}

SEVQS::SEVQS(const SEVQS & rhs) : v(rhs.v), q(rhs.q), s(rhs.s) {
}

SEVQS::SEVQS(DirectX::FXMVECTOR _v, SEQuaternion _q, float _s): q(_q), s(_s) {
	XMStoreFloat3(&v, _v);
}

SEVQS::SEVQS(SEQuaternion _q, float _s):v(0.0f, 0.0f, 0.0f), q(_q), s(_s) {
}

SEVQS::SEVQS(float _s):v(0.0f, 0.0f,0.0f), q(), s(_s) {
}

SEVQS & SEVQS::operator=(const SEVQS & rhs) {
	XMStoreFloat3(&v, XMLoadFloat3(&rhs.v));
	//v = rhs.v;
	q = rhs.q;
	s = rhs.s;
	return *this;
}

SEVQS SEVQS::operator+(const SEVQS & rhs) const {
	SEVQS vqs;
	XMStoreFloat3(&vqs.v, (XMLoadFloat3(&rhs.v) + XMLoadFloat3(&v)));
	vqs.q = (q + rhs.q).normalize();
	vqs.s = s + rhs.s;
	return vqs;
}

SEVQS & SEVQS::operator+=(const SEVQS & rhs) {
	XMStoreFloat3(&v, (XMLoadFloat3(&rhs.v) + XMLoadFloat3(&v)));
	(q += rhs.q).normalize();
	s += rhs.s;
	return *this;
}

SEVQS SEVQS::operator*(float t) const {
	SEVQS vqs;
	XMStoreFloat3(&vqs.v, (XMLoadFloat3(&v) * t));
	vqs.q = q * t;
	vqs.s = s * t;
	return vqs;
}

SEVQS & SEVQS::operator*=(float t) {
	XMStoreFloat3(&v, (XMLoadFloat3(&v) * t));
	q *= t;
	s *= t;
	return *this;
}

XMVECTOR SEVQS::transform(FXMVECTOR rhs) const {
	return q.rotate(rhs * s) + XMLoadFloat3(&v);
}

SEVQS SEVQS::operator*(const SEVQS & rhs) const {
	XMVECTOR v1 = XMLoadFloat3(&v);

	return SEVQS(rhs.transform(v1), rhs.q * q, 1.0f);
	
}

SEVQS & SEVQS::operator*=(const SEVQS & rhs) {
	XMVECTOR v1 = XMLoadFloat3(&v);

	XMStoreFloat3(&v, rhs.transform(v1));
	q = rhs.q * q;
	return *this;
}

SEVQS SEVQS::concatWithScale(const SEVQS & rhs) const {
	XMVECTOR v2 = XMLoadFloat3(&rhs.v);

	return SEVQS(this->transform(v2), q*rhs.q, s*rhs.s);
}

SEVQS & SEVQS::concatWithScale(const SEVQS & rhs) {
	XMVECTOR v2 = XMLoadFloat3(&rhs.v);
	XMStoreFloat3(&v, this->transform(v2));
	q *= rhs.q;
	s *= rhs.s;
	return *this;
}

SEVQS & SEVQS::translateBy(FXMVECTOR vector) {
	XMStoreFloat3(&v, XMLoadFloat3(&v) + vector);
	return *this;
}

SEVQS & SEVQS::rotateBy(FXMVECTOR angle) {
	q = SEQuaternion::fromEuler(angle).normalize();
	return *this;
}

SEVQS & SEVQS::scaleBy(float magnitute) {
	s *= magnitute;
	return *this;
}

XMMATRIX SEVQS::toMatrix() const {
	return XMMatrixAffineTransformation(
		XMLoadFloat3(&XMFLOAT3(s, s, s)),
		XMVectorZero(),
		q.get(),
		XMLoadFloat3(&v));
}

DirectX::XMMATRIX SEVQS::toMatrixWithScale(DirectX::FXMVECTOR scale) const {
	return XMMatrixAffineTransformation(
		scale,
		XMVectorZero(),
		q.get(),
		XMLoadFloat3(&v));
}
