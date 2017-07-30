#pragma once
#include<D3DX11.h>
#include"ITarget.h"
#include<atlbase.h>
#include<Source\Util\Type.h>

namespace framework {

	class CubeRenderTrget : public ITarget
	{
	public:
		CubeRenderTrget(int width,int height,DXGI_FORMAT format);
		~CubeRenderTrget();

		/**
		* @brief	  �r���[�̐ݒ�p�ϐ���Ԃ�
		* @return	�@ID3D11RenderTargetView*
		*/
		ID3D11RenderTargetView * const* getView();


		void clear(const util::Vec4& clearColor);

		const util::Vec2& getSize();
	private:
		virtual void createBuffer(int width, int height, DXGI_FORMAT format, int sampleCount = 1) override;

		virtual void createShaderResourceView(DXGI_FORMAT format) override;

		void createView(DXGI_FORMAT format);
	private:
		//!�`��^�[�Q�b�g�p
		CComPtr<ID3D11RenderTargetView> m_pView;
		util::Vec2 m_Size;
	};

}