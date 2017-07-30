#pragma once
#include<Source\Util\Type.h>
#include<minmax.h>
#include<math.h>
#include<vector>

#define PI 3.14159265359

namespace util {

	using Radian = float;
	using Degree = float;
	using Quaternion = util::Vec4;

	XMVECTOR axisRotate(const util::Vec3& axis, Degree angle);

	template<typename T>
	T clamp(T num, T min, T max) {
		return min(max(num, min), max);
	}

	/**
	* @brief		1ŸŒ³üŒ`•âŠ®(0 ~ 1)
	*/
	template<typename T = float>
	T lerp(T num, T min, T max) {
		return (1.0f -num ) * min + max *  num;
	}

	void createMatrixFromFront(util::Vec3& front,util::Mat4* result);

	void createMatrixFromLeft(util::Vec3& left, util::Mat4 * result);

	/**
	* @brief		’l‚Ì•„†‚ğ•Ô‚·
	* @return		+‚È‚ç‚P@-‚È‚ç-1
	*/
	int sign(float x);

	float bezierCurve(float x,float p1,float p2,float p3);

	float bezierCurve(float x, float p1, float p2, float p3, float p4);

//	float multidimensionalBezier(std::vector<float> param);
}