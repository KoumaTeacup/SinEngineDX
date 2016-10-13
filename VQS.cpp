#include "VQS.h"

using namespace DirectX;

SEVQS::SEVQS(const SEVQS & rhs) : v(rhs.v), q(rhs.q), s(rhs.s) {
}

SEVQS::SEVQS(DirectX::FXMVECTOR _v, SEQuaternion _q, float _s): q(_q), s(_s) {
	XMStoreFloat3(&v, _v);
}

SEVQS::SEVQS(SEQuaternion _q, float _s):q(_q), s(_s) {
}

SEVQS::SEVQS(float _s):s(_s) {
}

SEVQS & SEVQS::operator=(const SEVQS & rhs) {
	v = rhs.v;
	q = rhs.q;
	s = rhs.s;
	return *this;
}

XMVECTOR SEVQS::transform(FXMVECTOR rhs) const {
	return q.rotate(rhs * s) + XMLoadFloat3(&v);
}

SEVQS SEVQS::operator*(const SEVQS & rhs) const {
	XMVECTOR v1 = XMLoadFloat3(&v);
	XMVECTOR v2 = XMLoadFloat3(&rhs.v);

	return SEVQS(transform(v2), q*rhs.q, 1.0f);
	
}

SEVQS & SEVQS::operator*=(const SEVQS & rhs) {
	XMVECTOR v1 = XMLoadFloat3(&v);
	XMVECTOR v2 = XMLoadFloat3(&rhs.v);
	XMStoreFloat3(&v, transform(v2) + v1);
	q *= rhs.q;
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
	q = SEQuaternion::fromEuler(angle);
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
