#include"Render2DContainer.h"
#include<Source\Util\Math\Transform.h>
#include<Source\Device\Render\Renderer\2D\Single\SpriteRenderer.h>
#include<Source\Application\Screen\Screen.h>
#include<Source\Util\Render\RenderTargetStack.h>
#include<Source\Resource\Texture\RenderTarget.h>

namespace framework {
	Render2DContainer::Render2DContainer()
	{
	}

	Render2DContainer::~Render2DContainer()
	{
	}

	void Render2DContainer::init()
	{
		//�����ȃE�B�[�N�폜
		TaskContainer::checkDelete();
		for (auto task : m_Container) {
			task.lock()->setup();
		}

		m_pTarget = std::make_shared<framework::RenderTarget>(Screen::UI_WIDTH, Screen::UI_HEIGHT, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);

		D3D11_BLEND_DESC blendDesc;

		ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = TRUE;
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		m_pBlend = util::createBlendState(false,&blendDesc);
	}

	void Render2DContainer::draw()
	{
		util::setBlendState(m_pBlend.p);

		checkDelete();

		callOrderSort();

		draw2D();

		//�o�b�N�o�b�t�@�ɖ߂�
		backBufferDraw();
	}

	void Render2DContainer::draw2D()
	{
		//���݂̃^�[�Q�b�g���ꎞ�ޔ�
		util::RenderTargetStack stack(1);

		//2D�p�^�[�Q�b�g�Ɠ����T�C�Y�̃r���[�|�[�g
		util::setSingleViewPort(Screen::WINDOW_WIDTH, Screen::WINDOW_HEIGHT);
		//2D�p�^�[�Q�b�g�ɐ؂�ւ�
		util::getContext()->OMSetRenderTargets(1, m_pTarget->getView(), NULL);
		m_pTarget->clear(util::Vec4(0, 0, 0, 0));

		for (auto task : m_Container) {
			task.lock()->draw();
		}
	}

	void Render2DContainer::backBufferDraw()
	{
		//�ŏI�o�͂Ȃ̂Ńo�b�N�o�b�t�@�Ɠ����T�C�Y�̃r���[�|�[�g
		util::setSingleViewPort(Screen::PIXEL_WIDTH, Screen::PIXEL_HEIGHT);

		//�^�[�Q�b�g�̕`��ʒu�̓E�B���h�E�̒��S�ʒu
		util::Transform trans(util::Vec3(Screen::PIXEL_WIDTH_HALF, Screen::PIXEL_HEIGHT_HALF, 0), util::Vec3(0, 0, 0), util::Vec3(1, 1, 1));

		m_pTarget->draw(&trans);

		framework::DirectXInstance::getInstance()->getSwapChain()->Present(0, 0);
	}

}
