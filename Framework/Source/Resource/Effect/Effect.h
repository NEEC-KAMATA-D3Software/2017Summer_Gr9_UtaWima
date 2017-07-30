#pragma once
#include<Effekseer.h>
#include<EffekseerRendererDX11.h>
#include<Source\Util\Type.h>
#include<Source\Util\Math\Transform.h>

namespace framework {

	class Effect
	{
	public:
		Effect(Effekseer::Effect* instance, Effekseer::Handle handle);
		Effect(Effekseer::Effect* instance, Effekseer::Handle handle, util::Vec3 position);
		Effect(Effekseer::Effect* instance, Effekseer::Handle handle, util::Transform& trans);
		~Effect();

		void update(Effekseer::Manager* pManager);

		bool isExists(Effekseer::Manager* pManager);

		const Effekseer::Handle getHandle();

		Effekseer::Effect* getInst();

	public:
		util::Transform m_Trans;

	private:
		Effekseer::Vector3D convVec(const util::Vec3& vec);

	private:
		Effekseer::Effect* m_pInstance;
		Effekseer::Handle m_Handle;
	};
}