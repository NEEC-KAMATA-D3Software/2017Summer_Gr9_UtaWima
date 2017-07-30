#pragma once
#include<Effekseer.h>
#include<EffekseerRendererDX11.h>
#include<Source\Util\Type.h>
#include<unordered_map>
#include<string>
#include<memory>
#include<Source\Resource\Effect\Effect.h>

namespace framework {

	class EffectManager
	{
	public:
		~EffectManager();

		void create(int spriteNum = 1000);

		void registPath(const std::string &key,const std::string& filePath);

		void registPath(const std::string& filePath);

		std::weak_ptr<framework::Effect> generate(const std::string &key, const util::Vec3& pos);
		std::weak_ptr<framework::Effect> generate(const std::string &key, util::Transform& trans);

		void setViewMat(const util::Mat4& mat);

		void setProjMat(const util::Mat4& mat);

		void draw();

		static EffectManager* getInstance();

		void clear();

		void effectStop(Effekseer::Handle& handle);

		void effectPlay(Effekseer::Effect* instance);

		bool isEffectExits(std::weak_ptr<framework::Effect> pEffect);

	private:
		//エフェクシアがエフェクトを生成するとき毎回パスが必要見たいのでここで管理
		class EffectFactory {

		public:
			void regist(const std::string& key, const std::string& filePath);


			std::shared_ptr<framework::Effect> crete(const std::string& key, const  util::Vec3& pos, Effekseer::Manager*);
			std::shared_ptr<framework::Effect> crete(const std::string& key, util::Transform& trans, Effekseer::Manager*);

		private:
			//key エフェクトの名前 value　エフェクトのパス
			std::unordered_map<std::string, std::wstring> m_PathContainer;
		};

		EffectManager();
		void createEffekseerMat(Effekseer::Matrix44* result, const util::Mat4& inMat);
		void checkDelete();
		void effectInstanceUpdate();

	private:
		static std::unique_ptr<EffectManager> m_pInstance;

		EffekseerRenderer::Renderer* m_pRenderer;
		Effekseer::Manager* m_pManager;
		//!エフェクト生成クラス
		EffectFactory m_Factory;

		std::list<std::shared_ptr<framework::Effect>> m_EffectContainer;
		Effekseer::Effect* effect;
		Effekseer::Handle hundle;
	};




}