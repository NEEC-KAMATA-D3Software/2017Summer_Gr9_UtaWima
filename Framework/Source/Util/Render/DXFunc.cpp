#include"DXFunc.h"
#include"../Win/WinFunc.h"
#include<Source\Util\WrapFunc.h>
#include<Source\Resource\Texture\RenderTarget.h>
#include<Source\Resource\Texture\Texture2D.h>
#include<Source\Device\Render\Renderer\2D\Single\SpriteRenderer.h>

namespace util {

	ID3D11Device * getDevice()
	{
		return  framework::DirectXInstance::getInstance()->getDevice();
	}

	ID3D11DeviceContext * getContext()
	{
		return  framework::DirectXInstance::getInstance()->getContext();
	}

	IDXGISwapChain * getSwapChain()
	{
		return  framework::DirectXInstance::getInstance()->getSwapChain();
	}

	CComPtr<ID3D11Texture2D> getBackBuffer()
	{
		ID3D11Texture2D * pResult;
		HRESULT hr = framework::DirectXInstance::getInstance()->getSwapChain()->GetBuffer(
			0,
			__uuidof(ID3D11Texture2D),
			(LPVOID*)&pResult);

		if (FAILED(hr)) {
			WinFunc::OutLog("バックバッファ取得失敗");
		}

		CComPtr<ID3D11Texture2D> _pResult;
		_pResult.Attach(pResult);

		return pResult;
	}

	void setSingleViewPort(int width, int height)
	{
		D3D11_VIEWPORT port[1];
		port[0].TopLeftX = 0.0f;
		port[0].TopLeftY = 0.0f;
		port[0].Width = width;
		port[0].Height = height;
		port[0].MinDepth = 0.0f;
		port[0].MaxDepth = 1.0f;

		framework::DirectXInstance::getInstance()->setViewPort(port, 1);
	}

	CComPtr<ID3D11Buffer> createVertexBuffer(UINT bufferSize, void* data)
	{
		// 頂点バッファ定義
		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;      // デフォルト使用法
		vertexBufferDesc.ByteWidth = bufferSize;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // 頂点バッファ
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// 頂点バッファ\サブリソースの定義
		D3D11_SUBRESOURCE_DATA subData;
		ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));
		subData.pSysMem = data;  // バッファ・データの初期値
		subData.SysMemPitch = 0;
		subData.SysMemSlicePitch = 0;

		ID3D11Buffer* pResult;

		// 頂点バッファの作成
		HRESULT hr = getDevice()->CreateBuffer(&vertexBufferDesc, &subData, &pResult);
		if (FAILED(hr)) {
			WinFunc::OutLog("頂点バッファ作成失敗");
		}

		CComPtr<ID3D11Buffer> _pResult;
		_pResult.Attach(pResult);
		return _pResult;
	}

	CComPtr<ID3D11Buffer> createIndexBuffer(UINT bufferSize, void * data)
	{
		D3D11_BUFFER_DESC idxBufferDesc;
		ZeroMemory(&idxBufferDesc, sizeof(D3D11_BUFFER_DESC));
		idxBufferDesc.Usage = D3D11_USAGE_DEFAULT;     // デフォルト使用法
		idxBufferDesc.ByteWidth = bufferSize;
		idxBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; // インデックス・バッファ
		idxBufferDesc.CPUAccessFlags = 0;
		idxBufferDesc.MiscFlags = 0;
		idxBufferDesc.StructureByteStride = 0;

		// インデックス・バッファのサブリソースの定義
		D3D11_SUBRESOURCE_DATA idxSubData;
		ZeroMemory(&idxSubData, sizeof(D3D11_SUBRESOURCE_DATA));
		idxSubData.pSysMem = data;  // バッファ・データの初期値
		idxSubData.SysMemPitch = 0;
		idxSubData.SysMemSlicePitch = 0;

		ID3D11Buffer* pResult;
		HRESULT hr = getDevice()->CreateBuffer(&idxBufferDesc, &idxSubData, &pResult);
		if (FAILED(hr)) {
			assert(false && "インデックスバッファ作成失敗");
		}
		CComPtr<ID3D11Buffer> _pResutl;
		_pResutl.Attach(pResult);

		return _pResutl;
	}

	CComPtr<ID3D11Buffer> createConstantBuffer(UINT bufferSize, void * data)
	{
		// 定数バッファの定義
		D3D11_BUFFER_DESC cBufferDesc;
		ZeroMemory(&cBufferDesc, sizeof(D3D11_BUFFER_DESC));
		cBufferDesc.Usage = D3D11_USAGE_DYNAMIC;    // 動的(ダイナミック)使用法
		cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // 定数バッファ
		cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;     // CPUから書き込む
		cBufferDesc.MiscFlags = 0;
		cBufferDesc.StructureByteStride = 0;
		cBufferDesc.ByteWidth = bufferSize;

		ID3D11Buffer* pResult;
		HRESULT hr;
		if (data == nullptr) {
			// 定数バッファの作成
			hr = getDevice()->CreateBuffer(&cBufferDesc, NULL, &pResult);
		}
		else {
			// 初期値がある場合はセットする
			D3D11_SUBRESOURCE_DATA subData;
			ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));
			subData.pSysMem = data;  // バッファ・データの初期値
			subData.SysMemPitch = 0;
			subData.SysMemSlicePitch = 0;
			// 定数バッファの作成
			hr = getDevice()->CreateBuffer(&cBufferDesc, &subData, &pResult);
		}

		if (FAILED(hr)) {
			WinFunc::OutLog("定数バッファ作成失敗");
		}

		CComPtr<ID3D11Buffer> _pResult;
		_pResult.Attach(pResult);
		return _pResult;
	}

	CComPtr<ID3DBlob> compileShader(std::string fileName, std::string entryName, std::string compileVersion, const std::string & includePath)
	{
		UINT compileFlag;

#ifdef _MDEBUG
		//!デバッグを行う 最適化を行わない
		compileFlag = D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#else
		//!列優先で処理する
		compileFlag = D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR;
#endif
		ID3DBlob* pResult, *error;
		LPCSTR include = includePath.c_str();
		LPD3D10INCLUDE inc;
		//シェーダのコードをコンパイル
		HRESULT hr = D3DX11CompileFromFile(
			fileName.c_str(),  // ファイル名
			NULL,          // マクロ定義(なし)
			NULL,         // インクルード・ファイル定義(なし)
			entryName.c_str(),
			compileVersion.c_str(),
			compileFlag, // コンパイル・オプション
			0,             // エフェクトのコンパイル・オプション(なし)
			NULL,          // 直ぐにコンパイルしてから関数を抜ける。
			&pResult,      // コンパイルされたバイト・コード
			&error,          // エラーメッセージ
			NULL);         // 戻り値

		if (FAILED(hr)) {
			WinFunc::OutLog(std::string(static_cast<char*>(error->GetBufferPointer())));
			assert(false && "シェーダコンパイル失敗");
		}

		CComPtr<ID3DBlob> _pResult;
		_pResult.Attach(pResult);
		return _pResult;
	}

	CComPtr<ID3D11InputLayout> createIA(const D3D11_INPUT_ELEMENT_DESC *layout, int layoutCount, ID3DBlob * vertexBlob)
	{
		ID3D11InputLayout *pResult;
		HRESULT hr = getDevice()->CreateInputLayout(
			layout,
			layoutCount,
			vertexBlob->GetBufferPointer(),
			vertexBlob->GetBufferSize(),
			&pResult);

		if (FAILED(hr)) {
			assert(false && "入力レイアウト作成失敗");
		}

		CComPtr<ID3D11InputLayout> _pResult;
		_pResult.Attach(pResult);
		return _pResult;
	}

	CComPtr<ID3D11InputLayout> createIA(const D3D11_INPUT_ELEMENT_DESC *layout, int layoutCount, void* data, int size)
	{
		ID3D11InputLayout *pResult;
		HRESULT hr = getDevice()->CreateInputLayout(
			layout,
			layoutCount,
			data,
			size,
			&pResult);

		if (FAILED(hr)) {
			assert(false && "入力レイアウト作成失敗");
		}

		CComPtr<ID3D11InputLayout> _pResult;
		_pResult.Attach(pResult);
		return _pResult;
	}

	CComPtr<ID3D11RasterizerState> createRasterizerSate(bool activeMultiSample, bool isCullFace)
	{
		D3D11_RASTERIZER_DESC rd;
		ZeroMemory(&rd, sizeof(rd));
		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = isCullFace ? D3D11_CULL_BACK : D3D11_CULL_NONE;
		rd.FrontCounterClockwise = FALSE;//時計周りが表
		rd.DepthBias = 0;
		rd.DepthBiasClamp = 0;
		rd.SlopeScaledDepthBias = 0;
		rd.DepthClipEnable = TRUE;
		rd.ScissorEnable = FALSE;
		rd.MultisampleEnable = activeMultiSample;
		rd.AntialiasedLineEnable = FALSE;

		ID3D11RasterizerState * pResult;
		HRESULT hr = getDevice()->CreateRasterizerState(&rd, &pResult);
		if (FAILED(hr))
		{
			assert(false && "ラスタライザ作成失敗");
		}

		CComPtr<ID3D11RasterizerState> _pResult;
		_pResult.Attach(pResult);
		return _pResult;
	}

	CComPtr<ID3D11DepthStencilState> createDepthStencilState()
	{
		D3D11_DEPTH_STENCIL_DESC dsd;
		ZeroMemory(&dsd, sizeof(dsd));
		dsd.DepthEnable = TRUE;
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsd.DepthFunc = D3D11_COMPARISON_LESS;
		dsd.StencilEnable = FALSE;
		dsd.StencilReadMask = 0;
		dsd.StencilWriteMask = 0;

		//表面を向いているばあのステンシルテスト設定
		dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;//維持
		dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsd.FrontFace.StencilFunc = D3D11_COMPARISON_NEVER;//常に失敗
		//裏面設定
		dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;//維持
		dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;//常に成功

		ID3D11DepthStencilState* pResult;
		HRESULT hr = getDevice()->CreateDepthStencilState(
			&dsd,
			&pResult
		);
		CComPtr<ID3D11DepthStencilState> _pResult;
		_pResult.Attach(pResult);

		return _pResult;
	}

	CComPtr<ID3D11BlendState> createBlendState(bool isBlendEnable, D3D11_BLEND_DESC* desc)
	{
		ID3D11BlendState* result;

		D3D11_BLEND_DESC blendDesc;
		if (desc == nullptr) {
			ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
			blendDesc.AlphaToCoverageEnable = FALSE;
			blendDesc.IndependentBlendEnable = TRUE;
			blendDesc.RenderTarget[0].BlendEnable = isBlendEnable;
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}
		else {
			blendDesc = *desc;
		}

		D3D11_RENDER_TARGET_BLEND_DESC targetDesc;
		ZeroMemory(&targetDesc, sizeof(D3D11_RENDER_TARGET_BLEND_DESC));
		targetDesc.BlendEnable = FALSE;
		targetDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		util::foreach(7, [&](int i) {
			//0番目は設定済み
			CopyMemory(&blendDesc.RenderTarget[i + 1], &targetDesc, sizeof(D3D11_RENDER_TARGET_BLEND_DESC));
		});

		HRESULT hr = util::getDevice()->CreateBlendState(&blendDesc, &result);

		if (FAILED(hr)) {
			assert(false && "ブレンドステートオブジェクト作成失敗");
		}

		CComPtr<ID3D11BlendState> _pResult;
		_pResult.Attach(result);
		return _pResult;
	}

	CComPtr<ID3D11SamplerState> createSamplerState(const SamplerType & samplerType)
	{
		D3D11_TEXTURE_ADDRESS_MODE mode;

		switch (samplerType)
		{
		case SamplerType::Wrap:
			mode = D3D11_TEXTURE_ADDRESS_WRAP;
			break;
		case SamplerType::Clamp:
			mode = D3D11_TEXTURE_ADDRESS_CLAMP;
			break;
		default:
			assert(false && "設定されているタイプが一致しません");
			break;
		}

		ID3D11SamplerState* pResult;
		D3D11_SAMPLER_DESC descSampler;
		descSampler.Filter = D3D11_FILTER_ANISOTROPIC;
		descSampler.AddressU = mode;
		descSampler.AddressV = mode;
		descSampler.AddressW = mode;
		descSampler.MipLODBias = 0.0f;
		descSampler.MaxAnisotropy = 2;
		descSampler.ComparisonFunc = D3D11_COMPARISON_NEVER;
		descSampler.BorderColor[0] = 0.0f;
		descSampler.BorderColor[1] = 0.0f;
		descSampler.BorderColor[2] = 0.0f;
		descSampler.BorderColor[3] = 0.0f;
		descSampler.MinLOD = -FLT_MAX;
		descSampler.MaxLOD = FLT_MAX;
		HRESULT hr = getDevice()->CreateSamplerState(&descSampler, &pResult);
		if (FAILED(hr)) {
			assert(false && "サンプラー作成失敗");
		}

		CComPtr<ID3D11SamplerState> _pResult;
		_pResult.Attach(pResult);
		return _pResult;
	}

	void setBlendState(ID3D11BlendState * state)
	{
		float blendMask[4] = { 0.0f,0.0f ,0.0f ,0.0f };
		util::getContext()->OMSetBlendState(state, blendMask, 0xffffffff);
	}

	void shadowViewMat(util::Mat4 & lightView, util::Mat4 & lightProj, util::Mat4 & world, util::Mat4 * result)
	{
		util::Mat4 textureMatrix(
			0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, -0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f
		);

		auto viewWorld = lightView.toXMMatrix() * world.toXMMatrix();

		*result = lightProj.toXMMatrix() * viewWorld;
	}

	void writeTexutre(ID3D11Texture2D* pTexture2D, std::function<void(D3D11_MAPPED_SUBRESOURCE*)> action, D3D11_MAP mapMode)
	{
		//読み込んだテクスチャをマップ
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		auto hr = util::getContext()->Map(pTexture2D, D3D11CalcSubresource(0, 0, 1), mapMode, 0, &mappedResource);
		assert(!FAILED(hr) && "テクスチャをマップ失敗");

		action(&mappedResource);

		util::getContext()->Unmap(pTexture2D, D3D11CalcSubresource(0, 0, 1));
	}

	void writeArrayTexutre(ID3D11Texture2D * pTexture2D, int location, ID3D11Texture2D * pResource, D3D11_MAP mapMode)
	{
		//読み込んだテクスチャをマップ
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		auto hr = util::getContext()->Map(pResource, D3D11CalcSubresource(0, 0, 1), mapMode, 0, &mappedResource);
		assert(!FAILED(hr) && "テクスチャをマップ失敗");

		D3D11_TEXTURE2D_DESC desc;
		pTexture2D->GetDesc(&desc);
		//ミップの数分回る
		util::foreach(desc.MipLevels, [&](int mip) {
			util::getContext()->UpdateSubresource(pTexture2D,
				D3D11CalcSubresource(mip, location, desc.MipLevels),
				NULL,
				mappedResource.pData,
				mappedResource.RowPitch,
				0);
		});
		util::getContext()->Unmap(pResource, D3D11CalcSubresource(0, 0, 1));
	}

	void writeArrayTexutre(ID3D11Texture2D * pTexture2D, int location, D3D11_MAPPED_SUBRESOURCE * mappedResource)
	{
		D3D11_TEXTURE2D_DESC desc;
		pTexture2D->GetDesc(&desc);

		//ミップの数分回る
		util::foreach(desc.MipLevels, [&](int mip) {
			util::getContext()->UpdateSubresource(pTexture2D,
				D3D11CalcSubresource(mip, location, desc.MipLevels),
				NULL,
				mappedResource->pData,
				mappedResource->RowPitch,
				0);
		});

	}

	CComPtr<ID3D11Texture2D> loadTexture2D(const std::string & file)
	{
		D3DX11_IMAGE_LOAD_INFO texFnfo;
		ZeroMemory(&texFnfo, sizeof(texFnfo));
		texFnfo.Width = D3DX11_DEFAULT;
		texFnfo.Height = D3DX11_DEFAULT;
		texFnfo.Depth = D3DX11_DEFAULT;
		texFnfo.FirstMipLevel = D3DX11_DEFAULT;          // テクスチャーの最高解像度のミップマップ レベル。
		texFnfo.MipLevels = 1;                           // ミップマップ数。ここでは1つのみとする。
		texFnfo.Usage = D3D11_USAGE_STAGING;             // GPU から CPU へのデータ転送 (コピー) をサポート
		texFnfo.BindFlags = 0;
		texFnfo.CpuAccessFlags = D3D11_CPU_ACCESS_READ;  // リソースをマップ可能にし、CPU がそのリソースの内容を読み取れるようにする
		texFnfo.MiscFlags = 0;
		texFnfo.Format = DXGI_FORMAT_FROM_FILE;
		texFnfo.Filter = D3DX11_FILTER_POINT;            // テクスチャー読み込み時に使用するフィルター
		texFnfo.MipFilter = D3DX11_FILTER_POINT;         // ミップマップ作成時に使用するフィルター

		ID3D11Resource* pLoadedRes = NULL;
		CComPtr<ID3D11Resource> pLoadResSmart;//スマートポインタ

											  //画像読み込み
		auto hr = D3DX11CreateTextureFromFile(util::getDevice(), file.c_str(), &texFnfo, NULL, &pLoadedRes, NULL);
		assert(!FAILED(hr) && "テクスチャ読み込み失敗");
		//スマートポインタにアタッチ
		pLoadResSmart.Attach(pLoadedRes);

		ID3D11Texture2D* pLoadedTexture2D = NULL;
		CComPtr<ID3D11Texture2D> pLoadedTexture2DSmart;
		hr = pLoadResSmart->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pLoadedTexture2D));
		assert(!FAILED(hr) && "Texture2D作成失敗");
		pLoadedTexture2DSmart.Attach(pLoadedTexture2D);

		//完成したTexture2Dを返す
		return pLoadedTexture2D;
	}

	void LoadAndDraw(const std::string & filePath, const util::Vec2 & pos, const util::Vec2& scale)
	{
		auto tex = std::make_shared<framework::Texture2D>(filePath);
		std::unique_ptr<framework::SpriteRenderer> renderer = std::make_unique<framework::SpriteRenderer>();
		renderer->setSize(tex->getSize());
		renderer->setTexture(tex);


		auto uiTarget = std::make_shared<framework::RenderTarget>(framework::Screen::UI_WIDTH, framework::Screen::UI_HEIGHT, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);
		//2D用ターゲットと同じサイズのビューポート
		util::setSingleViewPort(framework::Screen::WINDOW_WIDTH, framework::Screen::WINDOW_HEIGHT);
		//2D用ターゲットに切り替え
		util::getContext()->OMSetRenderTargets(1, uiTarget->getView(), NULL);
		uiTarget->clear(util::Vec4(0, 0, 0, 0));
		//TODO 2D描画
		util::Transform trans(util::Vec3(pos.x, pos.y, 0), util::Vec3(), util::Vec3(scale.x, scale.y, 1));
		renderer->draw(&trans);



		std::unique_ptr<framework::RenderTarget> target = std::make_unique<framework::RenderTarget>(getBackBuffer(), DXGI_FORMAT_R8G8B8A8_UNORM);
		getContext()->OMSetRenderTargets(1, target->getView(),NULL);
		target->clear(util::Vec4(0,0,0,1));

		//最終出力なのでバックバッファと同じサイズのビューポート
		util::setSingleViewPort(framework::Screen::PIXEL_WIDTH, framework::Screen::PIXEL_HEIGHT);

		//ターゲットの描画位置はウィンドウの中心位置
		trans = util::Transform(util::Vec3(framework::Screen::WINDOW_WIDTH_HALF, framework:: Screen::WINDOW_HEIGHT_HALF, 0), util::Vec3(0, 0, 0), util::Vec3(1, 1, 1));

		uiTarget->draw(&trans);

		getContext()->OMSetRenderTargets(0, NULL, NULL);

		framework::DirectXInstance::getInstance()->getSwapChain()->Present(0, 0);


	//	auto tex = std::make_shared<framework::Texture2D>(filePath);
	//	std::unique_ptr<framework::SpriteRenderer> renderer = std::make_unique<framework::SpriteRenderer>();
	//	renderer->setSize(tex->getSize());
	//	renderer->setTexture(tex);





	//	std::unique_ptr<framework::RenderTarget> target = std::make_unique<framework::RenderTarget>(getBackBuffer(), DXGI_FORMAT_R8G8B8A8_UNORM);
	//	getContext()->OMSetRenderTargets(1, target->getView(), NULL);
	//	target->clear(util::Vec4(0, 0, 0, 1));

	//	//最終出力なのでバックバッファと同じサイズのビューポート
	//	util::setSingleViewPort(framework::Screen::PIXEL_WIDTH, framework::Screen::PIXEL_HEIGHT);
	////TODO 2D描画
	//	util::Transform trans(util::Vec3(pos.x, pos.y, 0), util::Vec3(), util::Vec3(scale.x, scale.y, 1));
	//	renderer->draw(&trans);

	//	getContext()->OMSetRenderTargets(0, NULL, NULL);

	//	framework::DirectXInstance::getInstance()->getSwapChain()->Present(0, 0);
	}

	void SetMultiThreadFlag(bool isMultiThread)
	{
		ID3D10Multithread* pThread;
		getContext()->QueryInterface(__uuidof(ID3D10Multithread), (void **)&pThread);
		CComPtr<ID3D10Multithread> thread;
		thread.Attach(pThread);

		thread->SetMultithreadProtected(isMultiThread);
	}

}