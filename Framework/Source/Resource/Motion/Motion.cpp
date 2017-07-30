#include"Motion.h"
#include<atlbase.h>
#include<assert.h>
#include<Source\Util\IO\FbxLoader.h>
#include<Source\Resource\Model\Model.h>
#include<Source\Device\Render\Renderer\3D\RenderDefine.h>
#include<Source\Resource\Texture\BoneTexture.h>
#include<Source\Util\Thread\CriticalSection.h>

namespace framework {
	Motion::Motion()
	{
	}
	Motion::Motion(const std::string& filePath)
		:Motion()
	{
		util::CriticalSection c;
		util::FbxLoader loader;
		loader.loadFBX(filePath);
		m_MotionMat = loader.getMotionMatrix();
		endFrame = m_MotionMat.size();
		assert(m_MotionMat.size() <= MAX_MOTION_FRAME && "アニメーションサイズが大きすぎます");
		if (m_MotionMat.size() < MAX_MOTION_FRAME) {
			m_MotionMat.resize(MAX_MOTION_FRAME);
		}
	}

	Motion::~Motion()
	{
	}

	void Motion::clear()
	{
		m_MotionMat.clear();
	}

	std::vector<util::Mat4> Motion::getFrameMotion(int frame)
	{
		std::vector<util::Mat4> result;

		result.reserve(m_MotionMat[frame].size());
		for (auto boneMat : m_MotionMat[frame]) {
			result.emplace_back(boneMat.second);
		}

		return result;
	}

	std::vector<util::Mat4> Motion::getFrameMotionWithName(const std::string & name, int frame)
	{
		std::vector<util::Mat4> result;

		result.reserve(m_MotionMat[frame].size());
		for (auto boneMat : m_MotionMat[frame]) {
			if (boneMat.first == name) {
				result.emplace_back(boneMat.second);
			}
		}
		
		return result;
	}

	int Motion::getMaxFrame()
	{
		return endFrame;
	}
}