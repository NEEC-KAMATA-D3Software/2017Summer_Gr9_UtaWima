#include"CubeInstanceRenderer.h"
#include<Source\Device\Render\Shader\StandardMaterial.h>
#include<Source\Resource\ResourceManager.h>
#include<Source\Util\Math\Transform.h>
#include<Source\Component\CameraComponent.h>
#include<Source\Util\WrapFunc.h>

namespace framework {


	CubeInstanceRenderer::CubeInstanceRenderer()
	{
		m_pAlwaysBuffer = util::createConstantBuffer(sizeof(AlwaysBuffer));
		m_pAlwaysBuffer2 = util::createConstantBuffer(sizeof(AlwaysBuffer2));
		m_pSometimeBuffer = util::createConstantBuffer(sizeof(SometimeBuffer));
	}

	CubeInstanceRenderer::~CubeInstanceRenderer()
	{
	}

	void CubeInstanceRenderer::init()
	{
		m_pMaterial = ResourceManager::getInstance()->getMaterial("BoxInstance");
		//入力レイアウト作成/////////////
		//TODO　レンダラインスタンスごとにIAオブジェクトが重複するので対策が必要
		D3D11_INPUT_ELEMENT_DESC layout[] = {
			{ "SV_Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "NORMAL",		0,DXGI_FORMAT_R32G32B32_FLOAT,0,sizeof(util::Vec3),D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		m_pInputLayout = util::createIA(layout, _countof(layout), m_pMaterial->getVertexByte()->GetBufferPointer(), m_pMaterial->getVertexByte()->GetBufferSize());

		BoxLayout vertexData[8] = {
			{ util::Vec3(-1.0f,1.0f,-1.0f),util::Vec3(-1.0f,1.0f,-1.0f) },{ util::Vec3(1.0f,1.0f,-1.0f),  util::Vec3(1.0f,1.0f,-1.0f)},
			{ util::Vec3(1.0f,-1.0f,-1.0f),util::Vec3(1.0f,-1.0f,-1.0f) },{ util::Vec3(-1.0f,-1.0f,-1.0f),util::Vec3(-1.0f,-1.0f,-1.0f) },
			{ util::Vec3(-1.0f,1.0f,1.0f),util::Vec3(-1.0f,1.0f,1.0f) },{ util::Vec3(1.0f,1.0f,1.0f),   util::Vec3(1.0f,1.0f,1.0f) },
			{ util::Vec3(1.0f,-1.0f,1.0f),util::Vec3(1.0f,-1.0f,1.0f) },{ util::Vec3(-1.0f,-1.0f,1.0f), util::Vec3(-1.0f,-1.0f,1.0f) }
		};

		m_Model.mesh.vertexBuffer = util::createVertexBuffer(sizeof(BoxLayout) * 8, vertexData);

		UINT idxVertexID[] = {
			0,1,3,  1,2,3,  1,5,2,  5,6,2,  5,4,6,  4,7,6,
			4,5,0,  5,1,0,  4,0,7,  0,3,7,  3,2,7,  2,6,7
		};
		m_Model.mesh.indexBuffer = util::createIndexBuffer(sizeof(int) * 36, &idxVertexID);
		//インデックス描画のときの立方体は36頂点
		m_Model.drawNum = 36;

		m_Projection = XMMatrixPerspectiveFovLH(
			XMConvertToRadians(Screen::PERSPECTIVE),
			Screen::WINDOW_WIDTH / (float)Screen::WINDOW_HEIGHT,//floatし忘れ注意
			1.0f,
			Screen::FAR_);

	}

	void CubeInstanceRenderer::onWire()
	{
		isWire = true;
	}

	void CubeInstanceRenderer::onPolygon()
	{
		isWire = false;
	}

	void CubeInstanceRenderer::setColor(std::vector<util::Vec3> colors)
	{
		m_Colors = colors;
	}

	void CubeInstanceRenderer::instanceAction(int instanceNum, util::Transform * trans)
	{
		m_AlwaysBuffer.world[instanceNum] = trans->toMatrix().toXMMatrix();
		m_AlwaysBuffer.rotate[instanceNum] = trans->toRotateMatrix().toXMMatrix();
		if (!m_Colors.empty() || m_Colors.size() > instanceNum) {
			util::Vec3 color = m_Colors[instanceNum];
			m_AlwaysBuffer2.color[instanceNum] = util::Vec4(color.x, color.y, color.z,1);
		}
		else {
			m_AlwaysBuffer2.color[instanceNum] = util::Vec4(0, 0, 1, 1);
		}
	}

	void CubeInstanceRenderer::setPipeline(int drawNum)
	{
		//パイプラインにシェーダーセット
		m_pMaterial->active();

		//バッファー２を更新
		util::updateConstantBuffer<AlwaysBuffer2>(m_pAlwaysBuffer2.p, m_AlwaysBuffer2);

		m_AlwaysBuffer.view = component::CameraComponent::getMainCamera().lock()->toViewMatrix().toXMMatrix();

		util::updateConstantBuffer<AlwaysBuffer>(m_pAlwaysBuffer.p, m_AlwaysBuffer);

		SometimeBuffer sb;

		sb.proj = XMMatrixTranspose(m_Projection.toXMMatrix());

		util::updateConstantBuffer<SometimeBuffer>(m_pSometimeBuffer.p, sb);

		//頂点バッファセット
		UINT strides[1] = { sizeof(BoxLayout) };
		UINT offsets[1] = { 0 };
		util::getContext()->IASetVertexBuffers(0, 1, &m_Model.mesh.vertexBuffer.p, strides, offsets);
		util::getContext()->IASetIndexBuffer(m_Model.mesh.indexBuffer.p, DXGI_FORMAT_R32_UINT, 0);
		if (isWire)
			util::getContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
		else
			util::getContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//定数バッファにセットするための変数
		ID3D11Buffer* mediationBuffer[3];

		//順番厳守
		mediationBuffer[0] = m_pSometimeBuffer.p;
		mediationBuffer[1] = m_pAlwaysBuffer.p;
		mediationBuffer[2] = m_pAlwaysBuffer2.p;

		m_pMaterial->setConstantBuffer(mediationBuffer, 3);

		util::getContext()->IASetInputLayout(m_pInputLayout.p);

		util::getContext()->DrawIndexedInstanced(m_Model.drawNum, drawNum, 0, 0, 0);

		m_pMaterial->deActive();
	}




}