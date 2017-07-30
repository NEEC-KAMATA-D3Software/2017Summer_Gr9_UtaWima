#pragma once
#include<D3DX11.h>
#include<xnamath.h>


namespace util {

	struct Vector2 : public XMFLOAT2
	{
		Vector2();
		Vector2(FLOAT x_, FLOAT y_);
		Vector2(Vector2& vec);
		Vector2(XMFLOAT2 vec);
		Vector2(const XMVECTOR& vec);

		Vector2 normalize();

		XMVECTOR toXMVector();

		const Vector2 operator+ ();

		const Vector2 operator- ();

		const Vector2 operator+ (const Vector2& other);

		const Vector2 operator+ (const float value);

		const Vector2 operator- (const Vector2& other);

		const Vector2 operator- (const float value);

		const Vector2 operator* (const Vector2& other);

		const Vector2 operator* (const float value);

		const Vector2 operator/ (const Vector2& other);

		const Vector2 operator/ (const float value);

		const Vector2 operator= (const Vector2& other);

		const Vector2 operator+= (const Vector2& other);

		const Vector2 operator-= (const Vector2& other);

		const Vector2 operator*= (const Vector2& other);

		const Vector2 operator/= (const Vector2& other);

	};

	struct Vector3 : public XMFLOAT3
	{
		Vector3();
		Vector3(FLOAT x_, FLOAT y_, FLOAT z_);
		Vector3(Vector3& other);
		Vector3(XMFLOAT3 vec);
		Vector3(const XMVECTOR& vec);

		Vector3 normalize();

		float  length();

		float distance(Vector3& other);

		XMVECTOR toXMVector();

		float dot(Vector3& other);

		Vector3 cross(Vector3& other);

		const Vector3 operator+ ();

		const Vector3 operator- ();

		const Vector3 operator+ (const Vector3& other);

		const Vector3 operator+ (const float value);

		const Vector3 operator- (const Vector3& other);

		const Vector3 operator- (const float value);

		const Vector3 operator* (const Vector3& other);

		const Vector3 operator* (const float value);

		const Vector3 operator/ (const Vector3& other);

		const Vector3 operator/ (const float value);

		const Vector3 operator=	(const Vector3& other);

		const Vector3 operator=	(const XMVECTOR& other);

		const Vector3 operator+= (const Vector3& other);

		const Vector3 operator-= (const Vector3& other);

		const Vector3 operator*= (const Vector3& other);

		const Vector3 operator/= (const Vector3& other);
	};

	struct Vector4 : public XMFLOAT4
	{
		Vector4();
		Vector4(FLOAT x_, FLOAT y_, FLOAT z_, FLOAT w_);
		Vector4(Vector4& other);
		Vector4(XMFLOAT4 other);
		Vector4(const XMVECTOR& vec);

		XMVECTOR toXMVector();

		const Vector4 operator+ ();

		const Vector4 operator- ();

		const Vector4 operator+ (const Vector4& other);

		const Vector4 operator+ (const float value);

		const Vector4 operator- (const Vector4& other);

		const Vector4 operator- (const float value);

		const Vector4 operator* (const Vector4& other);

		const Vector4 operator* (const float value);

		const Vector4 operator/ (const Vector4& other);

		const Vector4 operator/ (const float value);

		const Vector4 operator=	(const Vector4& other);

		const Vector4 operator=	(const XMVECTOR& other);

		const Vector4 operator+= (const Vector4& other);

		const Vector4 operator-= (const Vector4& other);

		const Vector4 operator*= (const Vector4& other);

		const Vector4 operator/= (const Vector4& other);
	};

	class Matrix : public XMFLOAT4X4
	{
	public:
		Matrix();

		Matrix(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);

		Matrix(XMMATRIX& mat);


		Matrix transpose()const;

		~Matrix();

		XMMATRIX toXMMatrix()const;

	};

}