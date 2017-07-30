#include"Effect.h"

namespace framework {

	Effect::Effect(Effekseer::Effect* instance,
		Effekseer::Handle handle)
		:Effect(instance, handle, util::Vec3(0, 0, 0))
	{
	}

	Effect::Effect(Effekseer::Effect * instance, Effekseer::Handle handle, util::Vec3 position)
		: Effect(instance, handle, util::Transform(position, util::Vec3(0, 0, 0), util::Vec3(1, 1, 1)))
	{
	}

	Effect::Effect(Effekseer::Effect * instance, Effekseer::Handle handle, util::Transform & trans)
		: m_pInstance(instance),
		m_Handle(handle),
		m_Trans(trans)
	{
	}

	Effect::~Effect()
	{
		ES_SAFE_RELEASE(m_pInstance);
	}

	void Effect::update(Effekseer::Manager * pManager)
	{
		pManager->SetLocation(m_Handle, convVec(m_Trans.m_Position));
		pManager->SetScale(m_Handle, m_Trans.m_Scale.x, m_Trans.m_Scale.y, m_Trans.m_Scale.z);
		pManager->SetRotation(m_Handle, m_Trans.m_Rotation.x, m_Trans.m_Rotation.y, m_Trans.m_Rotation.z);
	}

	bool Effect::isExists(Effekseer::Manager * pManager)
	{
		return pManager->Exists(m_Handle);
	}

	const Effekseer::Handle Effect::getHandle()
	{
		return m_Handle;
	}

	Effekseer::Effect * Effect::getInst()
	{
		return m_pInstance;
	}

	Effekseer::Vector3D Effect::convVec(const util::Vec3 & vec)
	{
		return Effekseer::Vector3D(vec.x, vec.y, vec.z);
	}

}