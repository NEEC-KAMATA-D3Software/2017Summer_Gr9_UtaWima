#include"Transform.h"
#include<D3DX11.h>
#include<xnamath.h>
#include<math.h>

namespace util {


	Transform::Transform()
		:Transform(util::Vec3(0, 0, 0), 0, util::Vec3(1, 1, 1))
	{
	}

	Transform::Transform(util::Vec3 position, float angle, Vec3 scale)
	{
		m_Position = position;
		//Y軸
		m_Rotation = util::Vec3(0,angle,0);
		m_Scale = scale;
	}

	Transform::Transform(util::Vec3 position, util::Vec3 angle, Vec3 scale)
	{
		m_Position = position;

		m_Rotation = angle;

		m_Scale = scale;
	}

	Transform::~Transform()
	{
	}

	Transform::Transform(Transform & other)
	{
		m_Position = other.m_Position;
		m_Rotation = other.m_Rotation;
		m_Scale = other.m_Scale;
	}

	XMVECTOR Transform::rotate(const util::Vec3 & axis, Degree angle)
	{
		return axisRotate(axis, angle);
	}

	XMVECTOR Transform::rotate(const util::Vec3 & angle)
	{
		auto&& xAxisRotate = axisRotate(util::Vec3(1, 0, 0), angle.x);
		auto&& yAxisRotate = axisRotate(util::Vec3(0, 1, 0), angle.y);
		auto&& zAxisRotate = axisRotate(util::Vec3(0, 0, 1), angle.z);

		auto&& xyAxisRotate = XMQuaternionMultiply(xAxisRotate, yAxisRotate);
		auto&& xyzAxisRotate = XMQuaternionMultiply(xyAxisRotate,zAxisRotate);

		//XMVECTORからXMFLOAT4に変換
		return xyzAxisRotate;
	}

	//void Transform::rotate(Quaternion& angle)
	//{
	//	m_Rotation = XMQuaternionMultiply(m_Rotation.toXMVector(), angle.toXMVector());
	//}

	float Transform::getAngle()
	{
		return m_Rotation.x;
	}

	void Transform::scaling(float scale)
	{
		m_Scale = util::Vec3(scale, scale, scale);
	}


	util::Mat4 Transform::toMatrix()
	{
		util::Mat4 world = XMMatrixIdentity();
		//座標埋め込み
		world._41 = m_Position.x;
		world._42 = m_Position.y;
		world._43 = m_Position.z;
		//スケール埋め込み
		world._11 = m_Scale.x;
		world._22 = m_Scale.y;
		world._33 = m_Scale.z;

		auto rotateMat = XMMatrixRotationQuaternion(rotate(m_Rotation));

		return XMMatrixTranspose(rotateMat * world.toXMMatrix());
	}

	util::Mat4 Transform::toRotateMatrix()
	{
		return XMMatrixTranspose(XMMatrixRotationQuaternion(rotate(m_Rotation)));
	}

	void Transform::lookAt(util::Vec3& target, util::Vec3& up)
	{
		util::Vec3 axisZ = target - m_Position;
		axisZ = axisZ.normalize();
		auto upVec = -up.normalize();

		if (axisZ.dot(axisZ) == 1) {
			m_Rotation.x = -90;
		}

		if (axisZ.dot(axisZ) == -1) {
			m_Rotation.x = 90; 
		}

		auto axisX = axisZ.cross(upVec).normalize();
		auto axisY = axisZ.cross(axisX).normalize();

		m_Rotation.x = XMConvertToDegrees(asin(axisZ.y));
		m_Rotation.y = XMConvertToDegrees(atan2(-axisZ.x, -axisZ.z));
		m_Rotation.z = XMConvertToDegrees(atan2(-axisX.y, axisY.y));
	}

	void Transform::lookAt(util::Vec3 & vec)
	{
		if (vec.x == 0 && vec.y == 0 && vec.z == 0) return;

		util::Vec3 axisZ = vec.normalize();
		axisZ = axisZ.normalize();
		auto upVec = util::Vec3(0,-1,0).normalize();

		if (axisZ.dot(axisZ) == 1) {
			m_Rotation.x = -90;
		}

		if (axisZ.dot(axisZ) == -1) {
			m_Rotation.x = 90;
		}

		auto axisX = axisZ.cross(upVec).normalize();
		auto axisY = axisZ.cross(axisX).normalize();

		m_Rotation.x = XMConvertToDegrees(asin(axisZ.y));
		m_Rotation.y = XMConvertToDegrees(atan2(-axisZ.x, -axisZ.z));
		m_Rotation.z = XMConvertToDegrees(atan2(-axisX.y, axisY.y));
	}

	util::Vec3 Transform::front()
	{
		auto&& mat = XMMatrixTranspose(toMatrix().toXMMatrix());
		return util::Vec3(mat._31, mat._32, mat._33).normalize();
	}

	util::Vec3 Transform::up()
	{
		auto&& mat = XMMatrixTranspose(toMatrix().toXMMatrix());
		return util::Vec3(mat._21, mat._22, mat._23).normalize();
	}

	util::Vec3 Transform::left()
	{
		auto&& mat = XMMatrixTranspose(toMatrix().toXMMatrix());
		return util::Vec3(mat._11, mat._12, mat._13).normalize();
	}




}