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
	SEQuaternion(float _s, DirectX::FXMVECTOR);
	SEQuaternion(DirectX::FXMVECTOR);
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

	// Getter
	float getReal() const;
	DirectX::XMVECTOR getImagine() const;
	DirectX::XMVECTOR get() const;

	// unary operation
	SEQuaternion conj() const;
	float norm() const;
	SEQuaternion inverse() const;
	bool isUnit() const;
	SEQuaternion &identify();

	// binary operation
	float dot(const SEQuaternion &rhs) const;

	DirectX::XMVECTOR rotate(DirectX::FXMVECTOR element) const;

	// conversion
	DirectX::XMMATRIX toMatrix() const;
	DirectX::XMVECTOR toEuler() const;
	static SEQuaternion fromMatrix(DirectX::CXMMATRIX matrix);
	static SEQuaternion fromEuler(DirectX::FXMVECTOR euler);
	
private:
	float s;
	DirectX::XMFLOAT3 v;
};

SEQuaternion operator*(float c, SEQuaternion q);