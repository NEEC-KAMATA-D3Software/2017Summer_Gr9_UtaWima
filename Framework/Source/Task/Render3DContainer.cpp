#include"Render3DContainer.h"
#include<Source\Util\Event\WindowEvent.h>
#include<Source\Util\Render\DXFunc.h>
#include<Source\Resource\Texture\Texture2D.h>
#include<Source\Util\Math\Transform.h>
#include<Source\Resource\Texture\DepthTarget.h>
#include<Source\Resource\Texture\RenderTarget.h>
#include<Source\Device\Render\Renderer\2D\Single\SpriteRenderer.h>
#include<Source\Resource\ResourceManager.h>
#include<Source\Resource\Texture\MultiRenderTarget.h>
#include<Source\Application\Screen\Screen.h>
#include<Source\Device\Render\Shader\DeferredLighting.h>
#include<Source\Resource\Texture\CubeReanderTarget.h>
#include<Source\Resource\Texture\CubeDepthTarget.h>
#include<Source\Entity\Entity.h>
#include<Source\Resource\Texture\MSAAMultiRenderTarget.h>
#include<Source\Component\CameraComponent.h>
#include<Source\Component\Effect\DirectionalLightComponent.h>
#include<Source\Device\Render\Renderer\3D\Instance\SkinningInstanceRenderer.h>
#include<Source\Util\Win\WinFunc.h>
#include<Source\Util\CSharp\SharpClass.h>
#include<Source\Device\Render\Renderer\Effekseer\EffectManager.h>
#include<Effekseer.h>
#include<EffekseerRendererDX11.h>

namespace framework {

	Render3DContainer::Render3DContainer()
	{
	}

	Render3DContainer::~Render3DContainer()
	{
		util::AntWeak::apiEnd();
	}

	void Render3DContainer::init()
	{
		//�����ȃE�B�[�N�폜
		TaskContainer::checkDelete();
		for (auto task : m_Container) {
			task.lock()->setup();
		}

		m_pDepthTarget = std::make_shared<DepthTarget>(Screen::PIXEL_WIDTH, Screen::PIXEL_HEIGHT, DXGI_FORMAT_R32_TYPELESS);

		m_pShadowMap = std::make_unique<DepthTarget>(Screen::PIXEL_WIDTH, Screen::PIXEL_HEIGHT, DXGI_FORMAT_R32_TYPELESS);
	
		CComPtr<ID3D11Texture2D> backBuffer = util::getBackBuffer();
		m_pTarget = std::make_unique<framework::RenderTarget>(backBuffer, DXGI_FORMAT_R8G8B8A8_UNORM);

		//�X�e���V���X�e�[�g�ƃ��X�^���C�U�[��3D�`��̏������i�K�ŃZ�b�g���Ă���
		//TODO ���X�^���C�U�[�̓����_���[���ƂɎ�������\��
		auto stencilState = util::createDepthStencilState();
		auto rs = util::createRasterizerSate(FALSE);
		m_pBlend = util::createBlendState();
		util::getContext()->RSSetState(rs);
		util::getContext()->OMSetDepthStencilState(stencilState.p, 0);
		util::setBlendState(m_pBlend.p);

		m_pDepthBuffer = std::make_shared<framework::Texture2D>(Screen::WINDOW_WIDTH, Screen::WINDOW_HEIGHT, m_pShadowMap->getShaderView());

		ResourceManager::getInstance()->removeTexture("ShadowMap");
		ResourceManager::getInstance()->importTexture2D("ShadowMap", m_pDepthBuffer);

		util::WindowEvent::getInstance()->addEvent(ID_COLOR, [&]() {
			
			m_DrawCallBack = [&](std::weak_ptr<component::CameraComponent> currentCamera) {
				//�ŏI�I�ȕ`�挋�ʂ�\��
				currentCamera.lock()->gBufferDraw(0);
			};
		});
		util::WindowEvent::getInstance()->addEvent(ID_NORMAL, [&]() {
			m_DrawCallBack = [&](std::weak_ptr<component::CameraComponent> currentCamera) {
				//�ŏI�I�ȕ`�挋�ʂ�\��
				currentCamera.lock()->gBufferDraw(1);
			};
		});

		util::WindowEvent::getInstance()->addEvent(ID_WORLD, [&]() {
			m_DrawCallBack = [&](std::weak_ptr<component::CameraComponent> currentCamera) {
				//�ŏI�I�ȕ`�挋�ʂ�\��
				currentCamera.lock()->gBufferDraw(2);
			};
		});

		util::WindowEvent::getInstance()->addEvent(ID_HEIGHT, [&]() {
			m_DrawCallBack = [&](std::weak_ptr<component::CameraComponent> currentCamera) {
				//�ŏI�I�ȕ`�挋�ʂ�\��
				currentCamera.lock()->gBufferDraw(3);
			};
		});

		util::WindowEvent::getInstance()->addEvent(ID_SPECULAR, [&]() {
			m_DrawCallBack = [&](std::weak_ptr<component::CameraComponent> currentCamera) {
				//�ŏI�I�ȕ`�挋�ʂ�\��
				currentCamera.lock()->gBufferDraw(4);
			};
		});

		util::WindowEvent::getInstance()->addEvent(ID_ENVIRONEMT, [&]() {
			m_DrawCallBack = [&](std::weak_ptr<component::CameraComponent> currentCamera) {
				//�ŏI�I�ȕ`�挋�ʂ�\��
				currentCamera.lock()->gBufferDraw(5);
			};
		});

		util::WindowEvent::getInstance()->addEvent(ID_METALNESS, [&]() {
			m_DrawCallBack = [&](std::weak_ptr<component::CameraComponent> currentCamera) {
				//�ŏI�I�ȕ`�挋�ʂ�\��
				currentCamera.lock()->gBufferDraw(6);
			};
		});

		util::WindowEvent::getInstance()->addEvent(ID_SHADOW_MAP, [&]() {
			m_DrawCallBack = [&](std::weak_ptr<component::CameraComponent> currentCamera) {
				
			};
		});

		util::WindowEvent::getInstance()->addEvent(ID_LIGHTING, [&]() {
			m_DrawCallBack = [&](std::weak_ptr<component::CameraComponent> currentCamera) {
				currentCamera.lock()->finalBufferDraw();
			};
		});

		util::WindowEvent::getInstance()->addEvent(ID_VELOCITY, [&]() {
			m_DrawCallBack = [&](std::weak_ptr<component::CameraComponent> currentCamera) {
				currentCamera.lock()->gBufferDraw(7);
			};
		});

		m_DrawCallBack = [&](std::weak_ptr<component::CameraComponent> currentCamera) {
			//�ŏI�I�ȕ`�挋�ʂ�\��
			currentCamera.lock()->gBufferDraw(0);
		};

		//�f�t�H���g�Ń��C�e�B���O
		m_DrawCallBack = [&](std::weak_ptr<component::CameraComponent> currentCamera) {
			//�ŏI�I�ȕ`�挋�ʂ�\��
			currentCamera.lock()->finalBufferDraw();
		};


		//Data data;

		//util::SharpClass sClass("Script.Class1");
		//
		//auto inst = sClass.createInstance();
	
		//inst.addMethod("Test2", 1);
		//inst.setParam<Data>("Test2",&data,0);

		//inst.callMethod<int>("Test2");

		//inst.addMethod("Sum",2);
		//inst.addMethod("Test", 0);
		////std::vector<int> param = {20,30};
		////inst.setParam("Sum",param);
		//inst.setParam("Sum",20,0);
		//inst.setParam("Sum", 20, 1);

		//auto sdf = inst.callMethod<int>("Sum");
		//auto sdf2 = inst.callMethod<int>("Test");

		//util::SharpClass sClass2("Script.Test");

		//auto inst2 = sClass2.createInstance();

		//inst2.addMethod("A",0);
		//inst2.addMethod("B", 0);
		//auto gs = inst2.callMethod<int>("A");
		//auto gs2 = inst2.callMethod<std::string>("B");
	}

	void Render3DContainer::draw()
	{

		//�ŏ����Ȃǂ��ĕ`��̈悪�Ȃ��ꍇ�͕`�悵�Ȃ�
		HRESULT hr = framework::DirectXInstance::getInstance()->getSwapChain()->Present(0, DXGI_PRESENT_TEST);
		if (hr == DXGI_STATUS_OCCLUDED) return;
		util::setBlendState(m_pBlend.p);


		//�^�X�N�폜
		checkDelete();

		//���݂̃��C���J�������ꎞ�ޔ�������
		auto currentCamera = component::CameraComponent::getMainCamera();

		shadowMapDoraw();

		if (!currentCamera.expired()) {
			//�J���������Ƃɖ߂�
			currentCamera.lock()->active();
		}

		//GBuffer�̕`��
		currentCamera.lock()->beginToBuffer(m_pDepthTarget);

		gBufferDraw();

		EffectManager::getInstance()->setViewMat(currentCamera.lock()->toViewMatrix());
		EffectManager::getInstance()->draw();

		currentCamera.lock()->endToBuffer();

		//���C�e�B���O����
		currentCamera.lock()->doLighting();

		currentCamera.lock()->doEffect();

		//m_g = std::make_unique<GradationFilter>(std::make_shared<Texture2D>(Screen::WINDOW_WIDTH/4, Screen::WINDOW_HEIGHT / 4, currentCamera.lock()->getShaderVeiw()));

		//m_g->write();

		util::setSingleViewPort(Screen::PIXEL_WIDTH, Screen::PIXEL_HEIGHT);
		util::getContext()->OMSetRenderTargets(1, m_pTarget->getView(), NULL);
		m_pTarget->clear(util::Vec4(0.024,0.285,0.35,1));

		//TODO �����Ƀ|�X�g������������
		m_DrawCallBack(currentCamera);


		//m_g->draw();

		//m_Bar->draw();
	}

	void Render3DContainer::removeTask(std::weak_ptr<Render3DComponent> task)
	{
		TaskContainer::removeTask(task);
	}

	void Render3DContainer::gBufferDraw()
	{
		//�r���[�|�[�g���X�N���[���T�C�Y�ɐݒ�
		util::setSingleViewPort(Screen::PIXEL_WIDTH, Screen::PIXEL_HEIGHT);

		for (auto task : m_Container) {
			task.lock()->draw();
		}
	}

	void Render3DContainer::shadowMapDoraw()
	{
		auto shadowMapCamera = component::DirectionalLightComponent::getCamera();
		if (shadowMapCamera.expired())return;
		shadowMapCamera.lock()->active();

		util::setSingleViewPort(Screen::PIXEL_WIDTH, Screen::PIXEL_HEIGHT);
		util::getContext()->OMSetRenderTargets(0, NULL, m_pShadowMap->getView());
		m_pShadowMap->clear();

		for (auto task : m_Container) {
			task.lock()->draw();
		}
	}

	void Render3DContainer::cubeMapCreate()
	{
		//TODO ��������`�施��
		for (auto task : m_Container) {
			task.lock()->cubeMapCreate(m_Container);
		}
	}

	void Render3DContainer::checkDelete()
	{
		//�����ȃE�B�[�N�폜
	//TaskContainer::checkDelete();

		m_Container.remove_if([&](std::weak_ptr<Render3DComponent> _task) {
			return _task.expired();
		});

		//�폜�Ώۂ��f���[�g
		for (auto deleteTask : m_DeleteContainer) {
			m_Container.remove_if([&](std::weak_ptr<Render3DComponent> _task) {
				////�����|�C���^���폜
				//return deleteTask.lock().get() == _task.lock().get();
				if (deleteTask.lock().get() == _task.lock().get()) {
					return true;
				}
				return false;
			});
		}
		m_DeleteContainer.clear();

		for (auto& deleteTask : m_Container) {
			if (deleteTask.lock()->drawNum() < 1)
				deleteTask.lock()->deActive();
		}
	}

}