#include "Vector.h"


namespace util {
	Vector2::Vector2()
		:Vector2(0,0)
	{
	}
	Vector2::Vector2(FLOAT x_, FLOAT y_)
		:XMFLOAT2(x_,y_)
	{
	}

	Vector2::Vector2(Vector2& vec)
	{
		this->x = vec.x;
		this->y = vec.y;
	}

	Vector2::Vector2(XMFLOAT2 vec)
		: XMFLOAT2(vec)
	{
	}

	Vector2::Vector2(const XMVECTOR & vec)
	{
		XMStoreFloat2(this,vec);
	}

	Vector2 Vector2::normalize()
	{
		//0œŽZ‰ñ”ð
		if (x == 0.0 && y == 0, 0)return Vector2();

		auto&& temp = XMVector2Normalize(XMLoadFloat2(this));
		return  Vector2(temp);
	}

	XMVECTOR Vector2::toXMVector()
	{
		return XMLoadFloat2(this);
	}

	const Vector2 Vector2::operator+()
	{
		return *this;
	}

	const Vector2 Vector2::operator-()
	{
		return util::Vector2(-x, -y);
	}

	const Vector2 Vector2::operator+(const Vector2& other)
	{
		return Vector2(x + other.x, y + other.y);
	}

	const Vector2 Vector2::operator+(const float value)
	{
		return Vector2(x + value, y + value);
	}

	const Vector2 Vector2::operator-(const Vector2& other)
	{
		return Vector2(x - other.x, y - other.y);
	}

	const Vector2 Vector2::operator-(const float value)
	{
		return Vector2(x - value, y - value);
	}

	const Vector2 Vector2::operator*(const Vector2& other)
	{
		return Vector2(x * other.x, y * other.y);
	}

	const Vector2 Vector2::operator*(const float value)
	{
		return Vector2(x * value, y * value);
	}

	const Vector2 Vector2::operator/(const Vector2& other)
	{
		return Vector2(x / other.x, y / other.y);
	}

	const Vector2 Vector2::operator/(const float value)
	{
		return Vector2(x / value, y / value);
	}

	const Vector2 Vector2::operator=(const Vector2& other)
	{
		this->x = other.x;
		this->y = other.y;
		return *this;
	}

	const Vector2 Vector2::operator+=(const Vector2& other)
	{
		this->x += other.x;
		this->y += other.y;
		return *this;
	}

	const Vector2 Vector2::operator-=(const Vector2& other)
	{
		this->x -= other.x;
		this->y -= other.y;
		return *this;
	}

	const Vector2 Vector2::operator*=(const Vector2& other)
	{
		this->x *= other.x;
		this->y *= other.y;
		return *this;
	}

	const Vector2 Vector2::operator/=(const Vector2& other)
	{
		this->x /= other.x;
		this->y /= other.y;
		return *this;
	}

	///////////////// Vector3 //////////////////////////////////////////////////////////////

	Vector3::Vector3()
		:Vector3(0,0,0)
	{
	}

	Vector3::Vector3(FLOAT x_, FLOAT y_, FLOAT z_)
		:XMFLOAT3(x_,y_,z_)
	{
	}

	Vector3::Vector3(Vector3 & other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
	}

	Vector3::Vector3(XMFLOAT3 vec)
		: XMFLOAT3(vec)
	{
	}

	Vector3::Vector3(const XMVECTOR& vec)
	{
		XMStoreFloat3(this, vec);
	}

	Vector3 Vector3::normalize()
	{
		auto&& temp = XMVector3Normalize(XMLoadFloat3(this));
		return  Vector3(temp);
	}

	float Vector3::length()
	{
		auto len = XMVector3Length(this->toXMVector());

		return XMVectorGetX(len);
	}

	float Vector3::distance(Vector3 & other)
	{
		float temp = pow(other.x - this->x,2) + pow(other.y - this->y,2) + pow(other.z - this->z,2);
		temp = sqrt(temp);
		return temp;
	}

	XMVECTOR Vector3::toXMVector()
	{
		return XMLoadFloat3(this);
	}

	float Vector3::dot(Vector3 & other)
	{
		auto dot = XMVector3Dot((*this).toXMVector(),other.toXMVector());
		return XMVectorGetX(dot);
	}

	Vector3 Vector3::cross(Vector3 & other)
	{
		return  XMVector3Cross((*this).toXMVector(),other.toXMVector());
	}

	const Vector3 Vector3::operator+()
	{
		return *this;
	}

	const Vector3 Vector3::operator-()
	{
		return util::Vector3(-x,-y,-z);
	}

	const Vector3 Vector3::operator+(const Vector3& other)
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	const Vector3 Vector3::operator+(const float value)
	{
		return Vector3(x + value, y + value, z + value);
	}

	const Vector3 Vector3::operator-(const Vector3& other)
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	const Vector3 Vector3::operator-(const float value)
	{
		return Vector3(x - value, y - value, z - value);
	}

	const Vector3 Vector3::operator*(const Vector3& other)
	{
		return Vector3(x * other.x, y * other.y, z * other.z);
	}

	const Vector3 Vector3::operator*(const float value)
	{
		return Vector3(x * value, y * value, z * value);
	}

	const Vector3 Vector3::operator/(const Vector3& other)
	{
		return Vector3(x / other.x, y / other.y, z / other.z);
	}

	const Vector3 Vector3::operator/(const float value)
	{
		return Vector3(x / value, y / value, z / value);
	}

	const Vector3 Vector3::operator=(const Vector3 & other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		return *this;
	}

	const Vector3 Vector3::operator=(const XMVECTOR&  other)
	{
		Vector3 temp;
		XMStoreFloat3(&temp,other);

		this->x = temp.x;
		this->y = temp.y;
		this->z = temp.z;
		return *this;
	}

	const Vector3 Vector3::operator+=(const Vector3& other)
	{
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		return *this;
	}

	const Vector3 Vector3::operator-=(const Vector3& other)
	{
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
		return *this;
	}

	const Vector3 Vector3::operator*=(const Vector3& other)
	{
		this->x *= other.x;
		this->y *= other.y;
		this->z *= other.z;
		return *this;
	}

	const Vector3 Vector3::operator/=(const Vector3& other)
	{
		this->x /= other.x;
		this->y /= other.y;
		this->z /= other.z;
		return *this;
	}

	///////////////// Vector4 //////////////////////////////////////////////////////////////

	Vector4::Vector4()
		:Vector4(0,0,0,0)
	{
	}

	Vector4::Vector4(FLOAT x_, FLOAT y_, FLOAT z_, FLOAT w_)
		:XMFLOAT4(x_,y_,z_,w_)
	{
	}

	Vector4::Vector4(Vector4 & other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		this->w = other.w;
	}

	Vector4::Vector4(XMFLOAT4 other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		this->w = other.w;
	}

	Vector4::Vector4(const XMVECTOR& vec)
	{
		Vector4 temp;
		XMStoreFloat4(&temp,vec);

		this->x = temp.x;
		this->y = temp.y;
		this->z = temp.z;
		this->w = temp.w;
	}

	XMVECTOR Vector4::toXMVector()
	{
		return XMLoadFloat4(this);
	}

	const Vector4 Vector4::operator+()
	{
		return *this;
	}

	const Vector4 Vector4::operator-()
	{
		return util::Vector4(-x, -y, -z, -w);
	}

	const Vector4 Vector4::operator+(const Vector4 & other)
	{
		return Vector4(x + other.x, y + other.y, z + other.z,w + other.w);
	}

	const Vector4 Vector4::operator+(const float value)
	{
		return Vector4(x + value, y + value, z + value,w + value);
	}

	const Vector4 Vector4::operator-(const Vector4 & other)
	{
		return Vector4(x - other.x, y - other.y, z - other.z,w - other.w);
	}

	const Vector4 Vector4::operator-(const float value)
	{
		return Vector4(x - value, y - value, z - value, w - value);
	}

	const Vector4 Vector4::operator*(const Vector4 & other)
	{
		return Vector4(x * other.x, y * other.y, z * other.z, w * other.w);
	}

	const Vector4 Vector4::operator*(const float value)
	{
		return Vector4(x * value, y * value, z * value, w * value);
	}

	const Vector4 Vector4::operator/(const Vector4 & other)
	{
		return Vector4(x / other.x, y / other.y, z / other.z,w / other.w);
	}

	const Vector4 Vector4::operator/(const float value)
	{
		return Vector4(x / value, y / value, z / value, w / value);
	}

	const Vector4 Vector4::operator=(const Vector4 & other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		this->w = other.w;
		return *this;
	}

	const Vector4 Vector4::operator=(const XMVECTOR & other)
	{
		Vector4 temp;
		XMStoreFloat4(&temp, other);

		this->x = temp.x;
		this->y = temp.y;
		this->z = temp.z;
		this->w = temp.w;
		return *this;
	}

	const Vector4 Vector4::operator*=(const Vector4 & other)
	{
		this->x *= other.x;
		this->y *= other.y;
		this->z *= other.z;
		this->w *= other.w;
		return *this;
	}

	const Vector4 Vector4::operator-=(const Vector4 & other)
	{
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
		this->w -= other.w;
		return *this;
	}

	const Vector4 Vector4::operator+=(const Vector4 & other)
	{
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		this->w += other.w;
		return *this;
	}

	const Vector4 Vector4::operator/=(const Vector4 & other)
	{
		this->x /= other.x;
		this->y /= other.y;
		this->z /= other.z;
		this->w /= other.w;
		return *this;
	}

	Matrix::Matrix()
		:XMFLOAT4X4()
	{
	}

	Matrix::Matrix(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33)
		:XMFLOAT4X4(m00, m01, m02, m03,
			m10, m11, m12, m13,
			m20, m21, m22, m23,
			m30, m31, m32, m33)
	{
	}

	Matrix::Matrix(XMMATRIX& mat){
		XMStoreFloat4x4(this, mat);
	}

	Matrix Matrix::transpose()const
	{
		return XMMatrixTranspose(this->toXMMatrix());
	}

	Matrix::~Matrix()
	{
	}

	XMMATRIX Matrix::toXMMatrix()const
	{
		return XMLoadFloat4x4(this);
	}


}


