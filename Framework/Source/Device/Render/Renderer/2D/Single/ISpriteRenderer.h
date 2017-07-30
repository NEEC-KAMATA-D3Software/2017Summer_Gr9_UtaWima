#pragma once
#include<Source\Device\Render\Shader\StandardMaterial.h>
#include<Source\Resource\Texture\Texture2D.h>
#include<Source\Resource\ResourceManager.h>
#include<Source\Util\Math\Transform.h>
#include<Source\Util\WrapFunc.h>
#include<Source\Application\Screen\Screen.h>
#include<Source\Resource\Texture\MSAAMultiRenderTarget.h>

namespace framework {

	class IMaterial;
	class Texture2D;
	class MultiRenderTarget;

	template<typename AlwaysBuffer,typename SometimeBuffer>
	class ISpriteRenderer
	{
	public:

		struct SpriteVertex
		{
			util::Vec3 position;
			util::Vec2 uv;
		};

		ISpriteRenderer(const std::string & materialName)
		{
			//�Œ�P�͊m�ۂ��Ă���
			m_pTexture.reserve(1);
			//�}�e���A���̂݌p���ɂ���ĕύX����\��������
			m_pMaterial = ResourceManager::getInstance()->getMaterial(materialName);

			m_pAlwaysBuffer = util::createConstantBuffer(sizeof(AlwaysBuffer));
			m_pSometimeBuffer = util::createConstantBuffer(sizeof(SometimeBuffer));


			//SpriteVertex vertexData[4] = {
			//	{ util::Vec3(0,  1, 0.0f)  ,util::Vec2(0,1) },
			//	{ util::Vec3(1,  1, 0.0f)  ,util::Vec2(1,1) },
			//	{ util::Vec3(1, 0, 0.0f)  ,util::Vec2(1,0) },
			//	{ util::Vec3(0, 0, 0.0f)	 ,util::Vec2(0,0) }
			//};

			SpriteVertex vertexData[4] = {
				{ util::Vec3(-1,  1, 0.0f)  ,util::Vec2(0,1) },
				{ util::Vec3(1,  1, 0.0f)  ,util::Vec2(1,1) },
				{ util::Vec3(1, -1, 0.0f)  ,util::Vec2(1,0) },
				{ util::Vec3(-1, -1, 0.0f)	 ,util::Vec2(0,0) }
			};

			//float v = 500;
			//SpriteVertex vertexData[4] = {
			//	{ util::Vec3(-v,  v, 0.0f)  ,util::Vec2(0,1) },
			//	{ util::Vec3(v,  v, 0.0f)  ,util::Vec2(1,1) },
			//	{ util::Vec3(v, -v, 0.0f)  ,util::Vec2(1,0) },
			//	{ util::Vec3(-v, -v, 0.0f)	 ,util::Vec2(0,0) }
			//};
			m_pVertexBuffer = util::createVertexBuffer(sizeof(vertexData), vertexData);

			UINT index[6] = {
				3,1,0, 3,2,1
			};
			m_pIndexBuffer = util::createIndexBuffer(sizeof(UINT) * 6, index);

			//���̓��C�A�E�g�쐬/////////////
			D3D11_INPUT_ELEMENT_DESC layout[] = {
				{ "SV_Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
				{ "TEXTURE"	   ,0,DXGI_FORMAT_R32G32_FLOAT   ,0,sizeof(util::Vec3),D3D11_INPUT_PER_VERTEX_DATA,0 },
			};
			m_pInputLayout = util::createIA(layout, _countof(layout), m_pMaterial->getVertexByte()->GetBufferPointer(), m_pMaterial->getVertexByte()->GetBufferSize());

			m_Projection = util::Mat4(
				2.0f / Screen::WINDOW_WIDTH, 0, 0, -1,
				0, -2.0f / Screen::WINDOW_HEIGHT, 0, 1,
				0, 0, 1, 0,
				0, 0, 0, 1
			);
		}
		
		~ISpriteRenderer() {
		
		}
		
		void setSize(const util::Vec2& size) {
			m_Size = util::Mat4(
				size.x/2, 0, 0, 0,
				0, size.y/2, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			);
		}

		void draw(util::Transform * trans) {

			assert(m_pMaterial.get() != nullptr && "�}�e���A�����쐬����Ă��܂���");

			m_pMaterial->active();
			begin();

			m_WoldMat = trans->toMatrix().toXMMatrix() * m_Size.toXMMatrix();

			setPipeline();

			UINT strides[1] = { sizeof(SpriteVertex) };
			UINT offsets[1] = { 0 };
			util::getContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer.p, strides, offsets);
			util::getContext()->IASetIndexBuffer(m_pIndexBuffer.p, DXGI_FORMAT_R32_UINT, 0);
			util::getContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


			ID3D11Buffer* buffer[2];
			buffer[0] = m_pSometimeBuffer.p;
			buffer[1] = m_pAlwaysBuffer.p;

			m_pMaterial->setConstantBuffer(buffer, 2);

			//m_pTexture�̃T�C�Y��m_pTextureArray�̗v�f���͓���
			m_pMaterial->setTexture(0, m_pTexture.size(), m_pTextureArray.get());

			util::getContext()->IASetInputLayout(m_pInputLayout.p);

			util::getContext()->DrawIndexed(6, 0, 0);


			end();
			m_pMaterial->deActive();
		}


		/**
		* @brief			ResourceManager����e�N�X�`�����擾���ăZ�b�g����
		* @param texName    ResourceManager�ɓo�^����Key
		* @dital			���I�ɔz����m�ۂ��Ȃ������߂��炩���ߐݒ肵�Ă����Ƃ悢
		*/
		void setTexture(const std::string& texName)
		{
			//�P�����Z�b�g����K�v�Ȃ��Ƃ��̓N���A�ɂ��Ēǉ�����
			m_pTexture.clear();
			m_pTexture.emplace_back(ResourceManager::getInstance()->getTexture2D(texName));
			m_pTextureArray = std::make_unique<ID3D11ShaderResourceView*[]>(m_pTexture.size());
			util::foreach(m_pTexture.size(), [&](int i) {
				m_pTextureArray[i] = *(m_pTexture[i]->getReource());
			});

			ta
		}


		/**
		* @brief			�P�����e�N�X�`�����Z�b�g����
		* @param texture    Texture2D�|�C���^
		* @dital			���I�ɔz����m�ۂ��Ȃ������߂��炩���ߐݒ肵�Ă����Ƃ悢
		*/
		void setTexture(std::shared_ptr<Texture2D> texture)
		{
			//�P�����Z�b�g����K�v�Ȃ��Ƃ��̓N���A�ɂ��Ēǉ�����
			m_pTexture.clear();
			m_pTexture.emplace_back(texture);
			m_pTextureArray = std::make_unique<ID3D11ShaderResourceView*[]>(m_pTexture.size());
			util::foreach(m_pTexture.size(), [&](int i) {
				m_pTextureArray[i] = *(m_pTexture[i]->getReource());
			});

		}

		/**
		* @brief			vector�ŕ����̃e�N�X�`�����Z�b�g����
		* @param texture    Texture2D�̃|�C���^vector
		* @dital			���I�ɔz����m�ۂ��Ȃ������߂��炩���ߐݒ肵�Ă����Ƃ悢
		*/
		void setTexture(std::vector<std::shared_ptr<Texture2D>> texture)
		{
			m_pTexture.clear();
			m_pTexture = texture;
			m_pTextureArray = std::make_unique<ID3D11ShaderResourceView*[]>(m_pTexture.size());
			util::foreach(m_pTexture.size(), [&](int i) {
				m_pTextureArray[i] = *(m_pTexture[i]->getReource());
			});
		}


		/**
		* @brief			�}���`�����_�[�^�[�Q�b�g�ňꊇ�ŃZ�b�g����
		* @param texture    MultiRendetTarget��ShaderdPtr
		* @dital			���I�ɔz����m�ۂ��Ȃ������߂��炩���ߐݒ肵�Ă����Ƃ悢
		*/
		void setTexture(const std::shared_ptr<MultiRenderTarget>& texture) {
			m_pTexture.clear();
			m_pTexture.resize(texture->getBufferNum());
			auto&& size = texture->getSize();
			auto damyTexture = std::make_shared<Texture2D>(size.x, size.y, texture->getShaderView(0));
			m_pTexture[0] = damyTexture;
			m_pTextureArray = std::make_unique<ID3D11ShaderResourceView*[]>(m_pTexture.size());

			util::foreach(m_pTexture.size(), [&](int i) {
				m_pTextureArray[i] = texture->getShaderView(i).p;
			});
		}

		/**
		* @brief			�}���`�����_�[�^�[�Q�b�g�ňꊇ�ŃZ�b�g����
		* @param texture    MultiRendetTarget��ShaderdPtr
		* @dital			���I�ɔz����m�ۂ��Ȃ������߂��炩���ߐݒ肵�Ă����Ƃ悢
		*/
		void setTexture(const std::shared_ptr<MSAAMultiRenderTarget>& texture) {
			m_pTexture.clear();
			m_pTexture.resize(texture->getBufferNum());
			auto&& size = texture->getSize();
			auto damyTexture = std::make_shared<Texture2D>(size.x, size.y, texture->getShaderView(0));
			m_pTexture[0] = damyTexture;
			m_pTextureArray = std::make_unique<ID3D11ShaderResourceView*[]>(m_pTexture.size());

			util::foreach(m_pTexture.size(), [&](int i) {
				m_pTextureArray[i] = texture->getShaderView(i).p;
			});
		}

	private:
		virtual void begin() {};
		virtual void end() {};
		virtual void setPipeline() = 0;

	protected:
		util::Mat4 m_WoldMat;
		CComPtr<ID3D11Buffer> m_pVertexBuffer;
		CComPtr<ID3D11Buffer> m_pIndexBuffer;

		std::vector<std::shared_ptr<Texture2D>> m_pTexture;
		std::unique_ptr<ID3D11ShaderResourceView*[]> m_pTextureArray;

		CComPtr<ID3D11InputLayout> m_pInputLayout;
		//!�X�V�p�x�������o�b�t�@
		CComPtr<ID3D11Buffer> m_pAlwaysBuffer;
		//!�X�V�p�x�̒Ⴂ�o�b�t�@
		CComPtr<ID3D11Buffer> m_pSometimeBuffer;
		std::shared_ptr<IMaterial> m_pMaterial;
		util::Mat4 m_Projection;
		//!�`��T�C�Y
		util::Mat4 m_Size;
	};
}