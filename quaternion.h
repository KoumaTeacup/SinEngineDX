#pragma once

#include <DirectXMath.h>

class SEQuaternion
{
public:
	// default ctor
	SEQuaternion();
	// copy ctor
	SEQuaternion(const SEQuaternion &rhs);
	// misc ctor
	SEQuaternion(float _s, DirectX::XMFLOAT3 _v);
	SEQuaternion(DirectX::XMFLOAT3 _v);
	SEQuaternion(float x, float y, float z);
	SEQuaternion(float _s, float x, float y, float z);

	// dtor
	~SEQuaternion() {}

	// copy assignment
	SEQuaternion& operator=(const SEQuaternion &rhs);

	// operation overload
	SEQuaternion operator-() const;
	SEQuaternion operator+(const SEQuaternion &rhs) const;
	SEQuaternion& operator+=(const SEQuaternion &rhs);
	SEQuaternion operator-(const SEQuaternion &rhs) const;
	SEQuaternion& operator-=(const SEQuaternion &rhs);
	SEQuaternion operator*(const SEQuaternion &rhs) const;
	SEQuaternion& operator*=(const SEQuaternion &rhs);
	SEQuaternion operator*(float c) const;
	SEQuaternion& operator*=(float c);
	SEQuaternion operator/(float c) const;
	SEQuaternion& operator/=(float c);

	// unary operation
	SEQuaternion conj() const;
	float norm() const;
	SEQuaternion inverse() const;
	float getReal() const;
	float dot(const SEQuaternion &rhs) const;
	bool isUnit() const;
	SEQuaternion &identify();

	// binary operation
	SEQuaternion rotateBy(const SEQuaternion & rhs) const;
	SEQuaternion &rotateBy(const SEQuaternion & rhs);

	// static operation
	static SEQuaternion fromMatrix(float matrix);
private:
	float s;
	DirectX::XMFLOAT3 v;
};

SEQuaternion operator*(float c, SEQuaternion q);