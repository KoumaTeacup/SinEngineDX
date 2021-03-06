#pragma once

#include "quaternion.h"

class SEVQS
{
public:
	// copy ctor
	SEVQS();
	// copy ctor
	SEVQS(const SEVQS &rhs);
	// misc ctor
	SEVQS(DirectX::FXMVECTOR _v, SEQuaternion _q = SEQuaternion(), float _s = 1.0f);
	SEVQS(SEQuaternion _q, float _s = 1.0f);
	SEVQS(float _s);

	// dtor
	~SEVQS() {}

	// copy assignment
	SEVQS &operator=(const SEVQS &rhs);

	// operation overload
	SEVQS operator+(const SEVQS &rhs) const;
	SEVQS &operator+=(const SEVQS &rhs);
	SEVQS operator*(float t) const;
	SEVQS &operator*=(float t);

	// transformation
	DirectX::XMVECTOR transform(DirectX::FXMVECTOR rhs) const;

	// concatenation
	SEVQS operator*(const SEVQS &rhs) const;
	SEVQS &operator*=(const SEVQS &rhs);
	SEVQS concatWithScale(const SEVQS &rhs) const;
	SEVQS &concatWithScale(const SEVQS &rhs);

	// modify loaclly, assuming 0-ed out first
	SEVQS &translateBy(DirectX::FXMVECTOR vector);
	SEVQS &rotateBy(DirectX::FXMVECTOR angle);
	SEVQS &scaleBy(float magnitute);

	// conversion
	DirectX::XMMATRIX toMatrix() const;
	DirectX::XMMATRIX toMatrixWithScale(DirectX::FXMVECTOR scale) const;

	// Interpolation
	SEVQS &increment(DirectX::FXMVECTOR v, SEQuaternion q, float s);

	// Getter
	DirectX::XMVECTOR getV() const {	return XMLoadFloat3(&v); }
	void setV(const DirectX::XMVECTOR &_v) { XMStoreFloat3(&v, _v); }
	SEQuaternion getQ() const { return q; }
	void setQ(const SEQuaternion &_q) { q = _q; }
	float getS() const { return s; }
private:
	DirectX::XMFLOAT3 v;
	SEQuaternion q;
	float s;
};