//最大描画数は100
#define MAX_DRAW_NUM 50

// 定数バッファのデータ定義①　変更しないデータ
cbuffer ConstBuffer : register(b0) { // 常にスロット「0」を使う
	matrix Projection;   // 透視変換行列
};

// 定数バッファのデータ定義②　変更頻度の低いデータ
cbuffer UpdateBuffer : register(b1) { // 常にスロット「1」を使う
	matrix View;   // ビュー変換行列
	matrix World[MAX_DRAW_NUM];   // 座標変換行列
	matrix Roate[MAX_DRAW_NUM];   //法線を回転させるための回転行列
	matrix ShadowViewMatrix; //差シャドウマッピング用行列
	float3 EyePos;
	float Damy1;
};

//マテリアルのパラメータ用バッファ
cbuffer MaterialBuffer : register(b2) {
	float Height;
	float Specular;
	float2 Damy2;
}

Texture2D colorTex : register(t0);   // テクスチャ
Texture2D normalTex : register(t1);   // テクスチャ
Texture2D heightTex : register(t2);   // テクスチャ
Texture2D specularTex : register(t3);   // テクスチャ
Texture2D metalnessTex : register(t4);   // テクスチャ
Texture2D roughnessTex : register(t5);   // テクスチャ

Texture2D shadowTex : register(t9);   // シャドウマップ
TextureCubeArray environmentTex : register(t10);   // 環境テクスチャ
//TextureCube environmentTex[6] : register(t10);   // サンプラ
SamplerState smpWrap : register(s0);

SamplerComparisonState shadowSampler
{
	// sampler state
	Filter = MIN_MAG_LINEAR_MIP_POINT;
	AddressU = MIRROR;
	AddressV = MIRROR;

	// sampler comparison state
	ComparisonFunc = LESS;
	ComparisonFilter = COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
};


//頂点シェーダの入力
struct VS_INPUT {
	float3 pos : SV_Position;
	float3 normal : NORMAL;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
	float2 uv : TEXTURE;
	uint instID : SV_InstanceID;
};
//頂点シェーダの出力
struct VS_OUT {
	float4 pos : SV_Position;
	float4 normal : NORMAL;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
	float2 uv : TEXTURE0;
	uint instID : TEXTURE1;
	float4 worldPos : TEXTURE2;
};

//ピクセルシェーダの出力
struct PS_OUT {
	float4 color : SV_TARGET0;
	float4 normal : SV_TARGET1;
	float4 world : SV_TARGET2;
	float4 materialParam : SV_TARGET3;
	float4 specular : SV_TARGET4;
	float4 environment : SV_TARGET5;
	float4 metalness : SV_TARGET6;
};

VS_OUT VS(VS_INPUT input) {

	float4 world = mul(float4(input.pos, 1), World[input.instID]);
	float4 pos = mul(world, View);
	pos = mul(pos, Projection);

	VS_OUT outPut;
	outPut.pos = pos;
	outPut.worldPos = world;
	outPut.normal = float4(input.normal, 1);
	outPut.binormal = input.binormal;
	outPut.tangent = input.tangent;
	outPut.uv = input.uv;
	outPut.instID = input.instID;
	return outPut;
}

/**
* @brief				ノーマルマップを接空間にするための行列
* @param tangent		タンジェント
* @param binormal		バイノーマル
* @param vertexNormal	頂点の法線
*/
matrix InvTangentMatrix(float3 tangent, float3 binormal, float3 vertexNormal) {
	matrix mat = matrix(
		float4(tangent, 0),
		float4(binormal, 0),
		float4(vertexNormal, 0),
		float4(0, 0, 0, 1));
	return mat;
}

/**
* @brief				視点ベクトル算出
*/
float3 ViewVec(float3 worldPos) {
	return normalize(EyePos - worldPos);
}

/**
* @brief
*/
float3 DiffuseLambert(float3 diffuse) {
	return diffuse * 0.3;
}


float D_GGX(float Rougness, float ret) {

	const float roughness = 1.0;
	float m = roughness * roughness;
	float m2 = m * m;
	float d = (ret * m2 - ret) * ret + 1.0;
	return m2 / (0.3 * d * d);
}

float SchlickFunc(float v, float k) {
	return 1.0 / (v * (1.0 - k) + k);
}

float Vis_Schlick(float roughness, float noV, float noL) {
	float k = (roughness * roughness) * 0.5;
	return SchlickFunc(noL, k) * SchlickFunc(noV, k);
}

//float3 F_Schlick(float3 specularColor,float halfVec) {
//	float fc = pow(1.0 - halfVec, 5.0);
//	return satrate(50.0 * specularColor.g);
//}

float3 Rough(float3 h, float3 normal) {

	float seed = 0.1;
	//!粗さ乱数
	float3 r = pow(2.0, (1.0 - seed) * 8192);

	return pow(dot(normal, h), r);
}

float4 normalEyeMap(float3 eyeVec, float2 uv) {
	float hScale = heightTex.Sample(smpWrap, uv).r * Height;
	float2 texCoord = uv - hScale * eyeVec.xy;
	return normalTex.Sample(smpWrap, texCoord) * 2.0 - 1.0;
}


PS_OUT PS(VS_OUT input)
{
	PS_OUT outColor;
	float4 color = pow(colorTex.Sample(smpWrap, input.uv),2.2);
	//0~1を-1~1の範囲に変更する
	float4 normal = normalTex.Sample(smpWrap, input.uv) * 2.0 - 1.0;
	//float4 normal = normalEyeMap(ViewVec(input.worldPos), input.uv);
	//ノーマルマップを接空間に移動する
	normal = mul(normal, InvTangentMatrix(input.tangent, input.binormal, input.normal));
	normal = mul(normal, Roate[input.instID]);
	normal = normalize(normal);


	matrix textureMat = matrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, -0.5, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.5, 0.5, 0.0, 1.0);

	float4 shadowCoord = mul(input.worldPos, textureMat);

	float isShadow = shadowTex.SampleCmpLevelZero(shadowSampler, shadowCoord.xy, shadowCoord.z - 0.01);

	//outColor.color = float4(isShadow, isShadow, isShadow,1);
	outColor.color = color;
	outColor.color.w = 1.0;
	float3 viewVec = ViewVec(input.worldPos);
	outColor.environment = environmentTex.Sample(smpWrap, float4(reflect(viewVec, normal), 0));
	outColor.normal = normal;
	outColor.world = input.worldPos;
	outColor.materialParam = float4(Specular, 0, 0, 1);
	outColor.specular = pow(specularTex.Sample(smpWrap, input.uv),2.2);
	outColor.metalness.x = metalnessTex.Sample(smpWrap, input.uv).x;
	outColor.metalness.y = roughnessTex.Sample(smpWrap, input.uv).x;
	return outColor;
	//return float4(Height,Specular, 0, 1);
}