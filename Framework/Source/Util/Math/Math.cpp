#include"Math.h"



namespace util {



	XMVECTOR axisRotate(const util::Vec3 & axis, Degree angle)
	{
		angle = XMConvertToRadians(angle);

		return XMQuaternionRotationAxis(XMLoadFloat3(&axis), angle);
	}

	void createMatrixFromFront(util::Vec3& front, util::Mat4 * result)
	{
		util::Vec3 temp = front + util::Vec3(0, -1, 0);

		util::Vec3 left = XMVector3Cross(front.toXMVector(), temp.toXMVector());

		util::Vec3 up = XMVector3Cross(front.toXMVector(), left.toXMVector());

		*result = util::Mat4(
			left.x, left.y, left.z, 0,
			up.x, up.y, up.z, 0,
			front.x, front.y, front.z, 0,
			0, 0, 0, 1);
	}

	void createMatrixFromLeft(util::Vec3& left, util::Mat4 * result)
	{
		util::Vec3 temp = left + util::Vec3(0, -1, 0);

		util::Vec3 front = XMVector3Cross(temp.toXMVector(), left.toXMVector());

		util::Vec3 up = XMVector3Cross(front.toXMVector(), left.toXMVector());

		*result = util::Mat4(
			left.x, left.y, left.z, 0,
			up.x, up.y, up.z, 0,
			front.x, front.y, front.z, 0,
			0, 0, 0, 1);
	}

	int sign(float x)
	{
		int sign = !signbit(x);
		return sign ? 1 : -1;
	}

	float bezierCurve(float x, float p1, float p2, float p3)
	{
		float q1 = lerp(x, p1, p2);
		float q2 = lerp(x, p2, p3);

		return lerp(x, q1, q2);
	}

	float bezierCurve(float x, float p1, float p2, float p3, float p4)
	{
		float q1 = lerp(x, p1, p2);
		float q2 = lerp(x, p2, p3);
		float q3 = lerp(x, p3, p4);

		float q4 = lerp(x,q1,q2);
		float q5 = lerp(x, q2, q3);

		return lerp(x, q4, q5);
	}

}