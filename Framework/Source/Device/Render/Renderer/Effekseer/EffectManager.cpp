#include"EffectManager.h"
#include<D3DX11.h>
#include<xnamath.h>
#include<Source\Application\Screen\Screen.h>

namespace framework {

	std::unique_ptr<EffectManager> EffectManager::m_pInstance = std::unique_ptr<EffectManager>(new EffectManager());

	EffectManager::EffectManager()
	{

	}

	EffectManager::~EffectManager()
	{
		m_EffectContainer.clear();
		m_pManager->Destroy();
		m_pRenderer->Destory();
	}

	void EffectManager::create(int spriteNum)
	{
		if (m_pRenderer != nullptr) {
			m_pRenderer->Destory();
		}
		if (m_pManager != nullptr) {
			m_pManager->Destroy();
		}

		m_pRenderer = ::EffekseerRendererDX11::Renderer::Create(util::getDevice(), util::getContext(), spriteNum);
		m_pManager = ::Effekseer::Manager::Create(spriteNum);


		util::Mat4 projMat = XMMatrixPerspectiveFovLH(
			XMConvertToRadians(Screen::PERSPECTIVE),
			Screen::WINDOW_WIDTH / (float)Screen::WINDOW_HEIGHT,//floatし忘れ注意
			1.0f,
			Screen::FAR_);

		setProjMat(projMat);

		// 描画方法の指定、独自に拡張しない限り定形文です。
		m_pManager->SetSpriteRenderer(m_pRenderer->CreateSpriteRenderer());
		m_pManager->SetRibbonRenderer(m_pRenderer->CreateRibbonRenderer());
		m_pManager->SetRingRenderer(m_pRenderer->CreateRingRenderer());
		m_pManager->SetModelRenderer(m_pRenderer->CreateModelRenderer());
		m_pManager->SetModelLoader(m_pRenderer->CreateModelLoader());
		// テクスチャ画像の読込方法の指定(パッケージ等から読み込む場合拡張する必要があります。)
		m_pManager->SetTextureLoader(m_pRenderer->CreateTextureLoader());
		// 座標系の指定(RHで右手系、LHで左手系)
		m_pManager->SetCoordinateSystem(::Effekseer::CoordinateSystem::LH);
	}

	void EffectManager::registPath(const std::string &key, const std::string & filePath)
	{
		m_Factory.regist(key,filePath);
	}

	void EffectManager::registPath(const std::string & filePath)
	{
		util::CSVLoader loader(filePath);

		auto param = loader.load();

		param.erase(param.begin());

		for (auto p :  param) {
			registPath(p[0], p[1]);
		}
	}

	std::weak_ptr<framework::Effect> EffectManager::generate(const std::string & key, const util::Vec3& pos)
	{
		auto effect = m_Factory.crete(key, pos,m_pManager);
		m_EffectContainer.emplace_back(effect);
		return effect;
	}

	std::weak_ptr<framework::Effect> EffectManager::generate(const std::string & key, util::Transform & trans)
	{
		auto effect = m_Factory.crete(key, trans, m_pManager);
		m_EffectContainer.emplace_back(effect);
		return effect;
	}

	void EffectManager::setViewMat(const util::Mat4 & mat)
	{
		Effekseer::Matrix44 viewMat;

		createEffekseerMat(&viewMat,mat);
		m_pRenderer->SetCameraMatrix(viewMat.Transpose());
	}

	void EffectManager::setProjMat(const util::Mat4 & mat)
	{
		Effekseer::Matrix44 projMat;

		createEffekseerMat(&projMat, mat);
		m_pRenderer->SetProjectionMatrix(projMat);
	}

	void EffectManager::draw()
	{
		checkDelete();
		effectInstanceUpdate();
		m_pManager->Update();
		m_pRenderer->BeginRendering();
		m_pManager->Draw();
		m_pRenderer->EndRendering();
	}

	EffectManager * EffectManager::getInstance()
	{
		return m_pInstance.get();
	}

	void EffectManager::clear()
	{
		m_EffectContainer.clear();
	}

	void EffectManager::effectStop(Effekseer::Handle & handle)
	{
		m_pManager->StopEffect(handle);
	}

	void EffectManager::effectPlay(Effekseer::Effect* instance)
	{
		
	}

	bool EffectManager::isEffectExits(std::weak_ptr<framework::Effect> pEffect)
	{
		auto handle = pEffect.lock()->getHandle();
		return  !m_pManager->Exists(handle);
	}


	void EffectManager::createEffekseerMat(Effekseer::Matrix44 * result, const util::Mat4 & inMat)
	{
		util::foreach(4, [&](int i) {
			util::foreach(4, [&](int j) {
				result->Values[j][i] = inMat.m[j][i];
			});
		});
	}

	void EffectManager::checkDelete()
	{
		//インスタンスが切れているオブジェクト削除
		m_EffectContainer.remove_if([&](std::shared_ptr<framework::Effect>& effect) {
			return !effect->isExists(m_pManager);
		});
	}

	void EffectManager::effectInstanceUpdate()
	{
		for (auto effect : m_EffectContainer) {
			effect->update(m_pManager);
		}
	}

	void EffectManager::EffectFactory::regist(const std::string & key, const std::string & filePath)
	{
		assert(m_PathContainer.find(key) == m_PathContainer.end() && "キーがすでに登録されています");
		
		auto path = util::StringToWString(filePath);

		m_PathContainer[key] = path;
	}

	std::shared_ptr<framework::Effect> EffectManager::EffectFactory::crete(const std::string & key, const  util::Vec3& pos, Effekseer::Manager* manager)
	{
		assert(m_PathContainer.find(key) != m_PathContainer.end() && "キーが登録されていません");

		
		auto effect = Effekseer::Effect::Create(manager, (EFK_CHAR*)(m_PathContainer[key].c_str()));
		auto handle = manager->Play(effect, pos.x, pos.y, pos.z);

		return std::make_shared<framework::Effect>(effect, handle);
	}

	std::shared_ptr<framework::Effect> EffectManager::EffectFactory::crete(const std::string & key, util::Transform & trans, Effekseer::Manager *manager)
	{
		assert(m_PathContainer.find(key) != m_PathContainer.end() && "キーがすでに登録されていません");


		auto effect = Effekseer::Effect::Create(manager, (EFK_CHAR*)(m_PathContainer[key].c_str()));
		auto handle = manager->Play(effect,0,0,0);

		return std::make_shared<framework::Effect>(effect, handle, trans);
	}

}