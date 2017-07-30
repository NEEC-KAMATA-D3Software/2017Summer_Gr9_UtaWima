#include"CameraComponent.h"
#include<Source\Entity\Entity.h>
#include<assert.h>
#include<Source\Resource\Texture\MSAAMultiRenderTarget.h>
#include<Source\Application\Screen\Screen.h>
#include<D3DX11.h>
#include<Source\Resource\Texture\DepthTarget.h>
#include<Source\Util\Render\DXFunc.h>
#include<Source\Component\Effect\BloomEffect.h>
#include<Source\Resource\Texture\Texture2D.h>
#include<Source\Resource\Texture\MSAADepthTarget.h>
#include<Source\Device\Render\Shader\DeferredLighting.h>
#include<Source\Resource\Texture\MultiRenderTarget.h>
#include<Source\Device\Render\Shader\PostEffectPass.h>

using namespace framework;

namespace component {

	WeakCamera CameraComponent::m_MainCamera;

	CameraComponent::CameraComponent()
	{
	}

	CameraComponent::~CameraComponent()
	{
	}

	void CameraComponent::init()
	{
		if (m_LookEntity.expired())
			m_Camera.setLookAt(util::Vec3(0, 0, 1));

		m_pLighting = std::make_unique<framework::DeferredLighting>();
		m_pLighting->setTexture(m_pGBuffer);

		//���g�ɂ��Ă���|�X�g�G�t�F�N�g�R���|�[�l���g�擾
		findPostEffect();

		m_pPass = std::make_unique<framework::PostEffectPass>(m_PostEfect, m_pGBuffer, m_pLBuffer);
	}

	void CameraComponent::active()
	{
		//���ׂẴJ�����R���|�[�l���g���A�N�e�B�u�ɂ��Ď������A�N�e�B�u�ɂ���
		cameraLoop([](WeakCamera camera) {
			camera.lock()->deActive();
		});

		UpdateComponent::active();

		m_MainCamera = m_Entity.lock()->getComponent<CameraComponent>(this);
		update();//�r���[�s��ɕϊ�����ۂɃf�[�^�����ׂ�0���ƃG���[���o��̂ň�x�X�V���Ă���
	}

	void CameraComponent::deActive()
	{
		UpdateComponent::deActive();
		//���C���J����������
		m_MainCamera.reset();
	}

	void CameraComponent::update()
	{
		//�|�C���^���s���ɂȂ��Ă����珈�����Ȃ�
		if (m_Entity.expired())return;
		auto trans = m_Entity.lock()->getTransform();
		m_Camera.setEyePosition(trans->m_Position);
		if (!m_LookEntity.expired()) {
			//�����_���Z�b�g����Ă���ꍇ�͂��������
			m_Camera.setLookAt(m_LookEntity.lock()->getTransform()->m_Position);
		}
		else {
			//�����_�������ꍇ�̓g�����X�t�H�[����Z����������
			m_Camera.setLookAt(trans->m_Position + trans->front() * 1.0f);
		}

		//90�x�ɂȂ��Ă��܂��ƕ`�悪�΂���Ă��܂��̂ŃN�����v
		trans->m_Rotation.x = util::clamp<float>(trans->m_Rotation.x, -89, 89);
	}

	util::Mat4 CameraComponent::toViewMatrix()
	{
		util::Mat4 viewMat = m_Camera.toViewMatrix().toXMMatrix();
		return viewMat;
	}

	void CameraComponent::setParam(const std::vector<std::string>& param)
	{
		if (param.empty())return;

		lookSet(param);

		renderTargetNumSet(param);
	}

	void CameraComponent::doLighting()
	{
		m_pLBuffer->clear(util::Vec4(0, 0, 0, 1));
		util::getContext()->OMSetRenderTargets(m_pLBuffer->getBufferNum(), m_pLBuffer->getView(), NULL);
		m_pLighting->draw();
		util::getContext()->OMSetRenderTargets(0, NULL, NULL);
	}

	void CameraComponent::doEffect()
	{
		//�����e�N�X�`���̍X�V
		m_pPass->rendering();
	}

	void CameraComponent::beginToBuffer(std::shared_ptr<framework::DepthTarget>& inDepth)
	{
		util::getContext()->OMSetRenderTargets(m_pGBuffer->getBufferNum(), m_pGBuffer->getView(), inDepth->getView());
		m_pGBuffer->clear(util::Vec4(0,0,0,1));
		inDepth->clear();
	}

	void CameraComponent::endToBuffer()
	{
		util::getContext()->OMSetRenderTargets(0, NULL, NULL);
		//m_pGBuffer->resolveTexutre();
	}

	void CameraComponent::gBufferDraw(int drawLocation)
	{
		m_pGBuffer->draw(drawLocation);
	}

	void CameraComponent::lightingDraw(int drawLocation)
	{
		m_pLBuffer->draw(drawLocation);
	}

	void CameraComponent::finalBufferDraw()
	{
		m_pPass->draw();
	}

	CComPtr<ID3D11ShaderResourceView> CameraComponent::getShaderVeiw()
	{
		return m_pLBuffer->getShaderView(0);
	}

	std::vector<WeakCamera> CameraComponent::allCameras()
	{
		std::vector<WeakCamera> cameraContainer;
		cameraLoop([&](WeakCamera camera) {
			cameraContainer.emplace_back(camera);
		});
		return cameraContainer;
	}

	WeakCamera CameraComponent::findCamera(const std::string& cameraEntityName)
	{
		WeakCamera findCamera;
		haveCameraEntityLoop([&](WeakEntity entity) {
			if (entity.lock()->getName() != cameraEntityName)return;
			findCamera = entity.lock()->getComponent<CameraComponent>();
		});
		return findCamera;
	}

	std::vector<WeakCamera> CameraComponent::findCameras(const std::string & cameraEntityName)
	{
		std::vector<WeakCamera> findCamera;
		haveCameraEntityLoop([&](WeakEntity entity) {
			if (entity.lock()->getName() != cameraEntityName)return;
			auto camerasList = entity.lock()->getComponents<CameraComponent>();
			for (auto camera : camerasList) {
				findCamera.emplace_back(camera);
			}
		});
		return findCamera;
	}

	WeakCamera CameraComponent::getMainCamera()
	{
		return m_MainCamera;
	}

	void CameraComponent::lookSet(const std::vector<std::string>& param)
	{
		if (param[0] == "")return;
		if (param[0] == "none")return;

		auto lookEntity = framework::Entity::findGameObj(param[0]);
		assert(!lookEntity.expired() && "�����_�Ɏw�肵��Entity�����݂��Ȃ����s���ł�");

		m_LookEntity = lookEntity;
	}

	void CameraComponent::renderTargetNumSet(const std::vector<std::string>& param)
	{
		m_pGBuffer = std::make_shared<MultiRenderTarget>(8, Screen::PIXEL_WIDTH, Screen::PIXEL_HEIGHT, DXGI_FORMAT_R32G32B32A32_FLOAT);
		m_pLBuffer = std::make_shared<framework::MultiRenderTarget>(2, Screen::PIXEL_WIDTH, Screen::PIXEL_HEIGHT, DXGI_FORMAT_R32G32B32A32_FLOAT);
		m_pPostRender = std::make_shared<framework::RenderTarget>(Screen::PIXEL_WIDTH, Screen::PIXEL_HEIGHT, DXGI_FORMAT_B8G8R8X8_UNORM);
	}

	void CameraComponent::findPostEffect()
	{
		m_PostEfect = m_Entity.lock()->getComponents<PostEffectComponent>();
	}

	void CameraComponent::cameraLoop(std::function<void(WeakCamera)> cameraAction)
	{
		haveCameraEntityLoop([&](WeakEntity entity) {
			auto components = entity.lock()->getComponents<CameraComponent>();
			for (auto camera : components) {
				cameraAction(camera);
			}
		});

	}
	void CameraComponent::haveCameraEntityLoop(std::function<void(WeakEntity)> entityAction)
	{
		//�J�����R���|�[�l���g�������Ă���G���e�B�e�B�[�����ׂĒT��
		haveComponentEntityLoop<CameraComponent>(entityAction);
	}
}