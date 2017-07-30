#include"ResourceManager.h"
#include<Source\Resource\Model\Model.h>
#include<Source\Util\Render\DXFunc.h>
#include<Source\Resource\Texture\Texture2D.h>
#include<Source\Device\Render\Shader\IMaterial.h>
#include<Source\Device\Render\Shader\StandardMaterial.h>
#include<Source\Util\Win\WinFunc.h>
#include<Source\Util\IO\CSVLoader.h>
#include<Source\Device\Render\Shader\GeometoryMaterial.h>
#include<Source\Util\WrapFunc.h>
#include<Source\Resource\Motion\Motion.h>
#include<Source\Resource\Sound\SoundResource.h>
#include<Source\Device\Render\Shader\TessellationMaterial.h>

namespace framework {

	std::shared_ptr<ResourceManager> ResourceManager::instance = std::shared_ptr<ResourceManager>(new ResourceManager());

	ResourceManager::ResourceManager()
	{
	}

	ResourceManager::~ResourceManager()
	{
	}

	ResourceManager * ResourceManager::getInstance()
	{
		return instance.get();
	}

	void ResourceManager::importRigidModel(const std::string & key, const std::string & filePath)
	{
		if (m_ModelContainer.find(key) != std::end(m_ModelContainer)) {
			util::WinFunc::OutLog("key‚ªd•¡‚µ‚Ä‚Ü‚·" + filePath);
			return;
		}
		auto model = std::make_shared<RigidModel>(filePath);
		model->type = ModelType::rigid;
	
		m_ModelContainer[key] = model;
	}

	void ResourceManager::importSkinningModel(const std::string & key, const std::string & filePath)
	{
		if (m_ModelContainer.find(key) != std::end(m_ModelContainer)) {
			util::WinFunc::OutLog("key‚ªd•¡‚µ‚Ä‚Ü‚·" + filePath);
			return;
		}
		auto model = std::make_shared<SkinningModel>(filePath);
		model->type = ModelType::skin;

		m_ModelContainer[key] = model;
	}

	void ResourceManager::importAnimation(const std::string & key, const std::string & filePath)
	{
		if (m_MotionContainer.find(key) != std::end(m_MotionContainer))
		{
			util::WinFunc::OutLog("key‚ªd•¡‚µ‚Ä‚Ü‚·" + filePath);
			return;
		}

		auto motion =  std::make_shared<Motion>(filePath);

		m_MotionContainer[key] = motion;
	} 

	void ResourceManager::importTexture2D(const std::string & key, const std::string & filePath)
	{
		if (m_Texture2DContainer.find(key) != std::end(m_Texture2DContainer)) {
			util::WinFunc::OutLog("key‚ªd•¡‚µ‚Ä‚Ü‚·" + filePath);
			return;
		}

		m_Texture2DContainer[key] = std::make_shared<Texture2D>(filePath);
	}

	void ResourceManager::importTexture2D(const std::string & key, std::shared_ptr<Texture2D> texture2D)
	{
		if (m_Texture2DContainer.find(key) != std::end(m_Texture2DContainer)) {
			util::WinFunc::OutLog("key‚ªd•¡‚µ‚Ä‚Ü‚·" );
			return;
		}

		m_Texture2DContainer[key] = texture2D;
	}

	void ResourceManager::importTextureCube(const std::string & key, const std::string filePath[6])
	{
		if (m_TextureCubeContainer.find(key) != std::end(m_TextureCubeContainer)) {
			util::WinFunc::OutLog("key‚ªd•¡‚µ‚Ä‚Ü‚·" );
			return;
		}

		m_TextureCubeContainer[key] = std::make_shared<CubeTexture>(filePath);
	}

	void ResourceManager::importStandardMaterial(const std::string & key, const std::string & filePath)
	{
		if (m_MaterialContainer.find(key) != std::end(m_MaterialContainer)) {
			util::WinFunc::OutLog("key‚ªd•¡‚µ‚Ä‚Ü‚·" + filePath);
			return;
		}

		auto dcV = util::compileShader(filePath, "VS", "vs_4_1");
		auto dcP = util::compileShader(filePath, "PS", "ps_4_1");

		ID3D11VertexShader* shaderV;
		util::getDevice()->CreateVertexShader(dcV->GetBufferPointer(), dcV->GetBufferSize(), NULL, &shaderV);
		CComPtr<ID3D11VertexShader> vertex;
		vertex.Attach(shaderV);

		ID3D11PixelShader* shaderP;
		util::getDevice()->CreatePixelShader(dcP->GetBufferPointer(), dcP->GetBufferSize(), NULL, &shaderP);
		CComPtr<ID3D11PixelShader> pixel;
		pixel.Attach(shaderP);

		framework::ShaderData<framework::ShaderType::Vertex> vertexShaderData;
		vertexShaderData.shaderAccess = vertex;
		vertexShaderData.byteData = dcV;
		m_MaterialContainer[key] = std::make_shared <framework::StandardMaterial>(vertexShaderData, pixel);

	}

	void ResourceManager::importGeometryMaterial(const std::string & key, const std::string & filePath)
	{
		if (m_MaterialContainer.find(key) != std::end(m_MaterialContainer)) {
			util::WinFunc::OutLog("key‚ªd•¡‚µ‚Ä‚Ü‚·" + filePath);
			return;
		}

		auto dcV = util::compileShader(filePath, "VS", "vs_4_1");
		auto dcG = util::compileShader(filePath, "GS", "gs_4_1");
		auto dcP = util::compileShader(filePath, "PS", "ps_4_1");

		ID3D11VertexShader* shaderV;
		util::getDevice()->CreateVertexShader(dcV->GetBufferPointer(), dcV->GetBufferSize(), NULL, &shaderV);
		CComPtr<ID3D11VertexShader> vertex;
		vertex.Attach(shaderV);

		ID3D11GeometryShader* shaderG;
		util::getDevice()->CreateGeometryShader(dcG->GetBufferPointer(), dcG->GetBufferSize(), NULL, &shaderG);
		CComPtr<ID3D11GeometryShader> geometry;
		geometry.Attach(shaderG);

		ID3D11PixelShader* shaderP;
		util::getDevice()->CreatePixelShader(dcP->GetBufferPointer(), dcP->GetBufferSize(), NULL, &shaderP);
		CComPtr<ID3D11PixelShader> pixel;
		pixel.Attach(shaderP);

		framework::ShaderData<framework::ShaderType::Vertex> vertexShaderData;
		vertexShaderData.shaderAccess = vertex;
		vertexShaderData.byteData = dcV;
		m_MaterialContainer[key] = std::make_shared <framework::GeometoryMaterial>(vertexShaderData, geometry, pixel);
	}

	void ResourceManager::importTessellationMaterial(const std::string & key, const std::string & filePath)
	{
		if (m_MaterialContainer.find(key) != std::end(m_MaterialContainer)) {
			util::WinFunc::OutLog("key‚ªd•¡‚µ‚Ä‚Ü‚·" + filePath);
			return;
		}

		//auto dcV = util::compileShader(filePath, "VS", "vs_4_1");
		//auto dcH = util::compileShader(filePath, "HS", "hs_4_1");
		//auto dcD = util::compileShader(filePath, "DS", "ds_4_1");
		//auto dcP = util::compileShader(filePath, "PS", "ps_4_1");

		auto dcV = util::compileShader(filePath, "VS", "vs_5_0");
		auto dcH = util::compileShader(filePath, "HS", "hs_5_0");
		auto dcD = util::compileShader(filePath, "DS", "ds_5_0");
		auto dcP = util::compileShader(filePath, "PS", "ps_5_0");


		ID3D11VertexShader* shaderV;
		util::getDevice()->CreateVertexShader(dcV->GetBufferPointer(), dcV->GetBufferSize(), NULL, &shaderV);
		CComPtr<ID3D11VertexShader> vertex;
		vertex.Attach(shaderV);

		ID3D11PixelShader* shaderP;
		util::getDevice()->CreatePixelShader(dcP->GetBufferPointer(), dcP->GetBufferSize(), NULL, &shaderP);
		CComPtr<ID3D11PixelShader> pixel;
		pixel.Attach(shaderP);

		ID3D11HullShader* shaderH;
		util::getDevice()->CreateHullShader(dcH->GetBufferPointer(), dcH->GetBufferSize(), NULL, &shaderH);
		CComPtr<ID3D11HullShader> hull;
		hull.Attach(shaderH);

		ID3D11DomainShader* shaderD;
		util::getDevice()->CreateDomainShader(dcD->GetBufferPointer(), dcD->GetBufferSize(), NULL, &shaderD);
		CComPtr<ID3D11DomainShader> domain;
		domain.Attach(shaderD);

		framework::ShaderData<framework::ShaderType::Vertex> vertexShaderData;
		vertexShaderData.shaderAccess = vertex;
		vertexShaderData.byteData = dcV;
		m_MaterialContainer[key] = std::make_shared <framework::TessellationMaterial>(vertexShaderData, pixel, hull, domain);

	}

	void ResourceManager::removeTexture(const std::string & key)
	{
		auto find = m_Texture2DContainer.find(key);

		if(find != m_Texture2DContainer.end())
			m_Texture2DContainer.erase(find);
	}

	void ResourceManager::setSoundDevice(std::shared_ptr<SoundPlayer> soundDevice)
	{
		m_pSoundDevice = soundDevice;
	}

	void ResourceManager::importSound(const std::string & key, const std::string & filePath)
	{
		m_pSoundDevice->importResource(key,filePath);
	}

	std::shared_ptr<Model> ResourceManager::getModel(const std::string & key)
	{
#ifdef _MDEBUG
		if (m_ModelContainer.find(key) == std::end(m_ModelContainer)) {
			util::WinFunc::OutLog("key‚ª“o˜^‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ : " + key);
			assert(false && "key‚ª“o˜^‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ");
		}
#endif
		return m_ModelContainer[key];
	}

	std::shared_ptr<Texture2D> ResourceManager::getTexture2D(const std::string & key)
	{

#ifdef _MDEBUG
		if (m_Texture2DContainer.find(key) == std::end(m_Texture2DContainer)) {
			util::WinFunc::OutLog("key‚ª“o˜^‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ : " + key);
			assert(false && "key‚ª“o˜^‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ");
		}
#endif
		return m_Texture2DContainer[key];
	}

	std::shared_ptr<CubeTexture> ResourceManager::getTextureCube(const std::string & key)
	{
#ifdef _MDEBUG
		if (m_TextureCubeContainer.find(key) == std::end(m_TextureCubeContainer)) {
			util::WinFunc::OutLog("key‚ª“o˜^‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ : " + key);
			assert(false && "key‚ª“o˜^‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ");
		}
#endif
		return m_TextureCubeContainer[key];
	}

	std::shared_ptr<IMaterial> ResourceManager::getMaterial(const std::string& key)
	{
#ifdef _MDEBUG
		if (m_MaterialContainer.find(key) == std::end(m_MaterialContainer)) {
			util::WinFunc::OutLog("key‚ª“o˜^‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ : " + key);
			assert(false && "key‚ª“o˜^‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ");
		}
#endif
		return m_MaterialContainer[key];
	}

	std::shared_ptr<Motion> ResourceManager::getMotion(const std::string & key)
	{
#ifdef _MDEBUG
		if (m_MotionContainer.find(key) == std::end(m_MotionContainer)) {
			util::WinFunc::OutLog("key‚ª“o˜^‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ : " + key);
			assert(false && "key‚ª“o˜^‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ");
		}
#endif
		return m_MotionContainer[key];
	}

	void ResourceManager::playSound(const std::string & key)
	{
		m_pSoundDevice->playSound(key);
	}

	bool ResourceManager::isPlaying(const std::string & key)
	{
		return m_pSoundDevice->isPlaying(key);
	}

	void ResourceManager::clear()
	{
		m_ModelContainer.clear();
		m_MotionContainer.clear();
		m_Texture2DContainer.clear();
		m_MaterialContainer.clear();
		m_TextureCubeContainer.clear();
		m_pSoundDevice->clear();
	}


}