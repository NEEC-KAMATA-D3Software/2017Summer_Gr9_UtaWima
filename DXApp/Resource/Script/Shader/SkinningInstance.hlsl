//最大描画数は100
#define MAX_DRAW_NUM 50

//ボーンの最大数は128
#define BONE_NUM 128

#define PREFRAME 3

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
	int Damy;
};

//マテリアルのパラメータ用バッファ
cbuffer MaterialBuffer : register(b2) {
	float Height;
	float Specular;
	float isNotLighting;
	float damy;
}

//更新頻度の高いバッファ
cbuffer SkinningBuffer : register(b3)
{
	//!ボーン行列 メッシュの数　ボーンの数
	//matrix BoneMatrix[BONE_NUM];
	//int DrawData[MAX_DRAW_NUM];
	float4 DrawData[MAX_DRAW_NUM];//フレーム番号　アニメーション番号 アニメータがあるかどうか
};

Texture2D colorTex : register(t0);   // テクスチャ
Texture2D normalTex : register(t1);   // テクスチャ
Texture2D heightTex : register(t2);   // テクスチャ
Texture2D specularTex : register(t3);   // テクスチャ
Texture2D metalnessTex : register(t4);   // テクスチャ
Texture2D roughnessTex : register(t5);   // テクスチャ

Texture2D shadowTex : register(t9);   // シャドウマップ
TextureCube environmentTex : register(t10);   // 環境テクスチャ
Texture2D boneTexture : register(t11);
//Texture3D boneTexture : register(t11);
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
	uint4 boneIndex : TEXTURE1;
	float4 weight : TEXTURE2;
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
	float4 depthCoord : TEXTURE3;
	float4 projPos : TEXTURE4;
	float4 prePos : TEXTURE5;
	float3 velocity : TEXTURE6;
	bool isBlur : TEXTURE7;
};

// ハルシェーダーのパッチ定数フェーズ用の出力パラメータ
struct CONSTANT_HS_OUT
{
	float Edges[3] : SV_TessFactor;        // パッチのエッジのテッセレーション係数
	float Inside : SV_InsideTessFactor; // パッチ内部のテッセレーション係数
};

//// ハルシェーダーのコントロール ポイント フェーズ用の出力パラメータ
//struct HS_OUT
//{
//	float3 pos   : POSITION;
//	float2 texel : TEXCOORD0;
//};
//
//// ドメインシェーダーの出力パラメータ
//struct DS_OUT
//{
//	float4 pos   : SV_POSITION;
//	float2 texel : TEXCOORD0;
//};


//ピクセルシェーダの出力
struct PS_OUT {
	float4 color : SV_TARGET0;
	float4 normal : SV_TARGET1;
	float4 world : SV_TARGET2;
	float4 materialParam : SV_TARGET3;
	float4 specular : SV_TARGET4;
	float4 environment : SV_TARGET5;
	float4 metalness : SV_TARGET6;
	float4 velocity : SV_TARGET7;
};

matrix LoadBoneTex(int boneID, int frame, int animeNum) {

	int animeOffset = (120 * 4) *
		animeNum;
	int frameOffset = frame * 4 + animeOffset;

	float4 bone1 = boneTexture.Load(int3(boneID, frameOffset + 0, 0));
	float4 bone2 = boneTexture.Load(int3(boneID, frameOffset + 1, 0));
	float4 bone3 = boneTexture.Load(int3(boneID, frameOffset + 2, 0));
	float4 bone4 = boneTexture.Load(int3(boneID, frameOffset + 3, 0));
	return matrix(
		bone1,
		bone2,
		bone3,
		bone4);
}


matrix IdentityMat() {
	return matrix(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}

/**
* @brief			モーション行列から移動成分を削除する
* @param inMat		モーション行列
*/
matrix removeMoveVec(matrix inMat) {
	return matrix(
		inMat[0][0], inMat[0][1], inMat[0][2], inMat[0][3],
		inMat[1][0], inMat[1][1], inMat[1][2], inMat[1][3],
		inMat[2][0], inMat[2][1], inMat[2][2], inMat[2][3],
		0.0, 0.0, 0.0, 1.0
		);
}

bool isBlur(float3 dir, float3 normal ,int frame) {
	return dot(normalize(dir), normal) > 0.6 && frame > PREFRAME;
}

/**
* @brief			ビュープロジェクション行列
*/
matrix VP() {
	return mul(View, Projection);
}


VS_OUT VS(VS_INPUT input) {

	matrix boneMat = IdentityMat();
	matrix preBoneMat = IdentityMat();

	bool isAnimatorFind = DrawData[input.instID].z;

	if (isAnimatorFind) {
		boneMat = LoadBoneTex(input.boneIndex.x, DrawData[input.instID].x, DrawData[input.instID].y) * input.weight.x;
		boneMat += LoadBoneTex(input.boneIndex.y, DrawData[input.instID].x, DrawData[input.instID].y) * input.weight.y;
		boneMat += LoadBoneTex(input.boneIndex.z, DrawData[input.instID].x, DrawData[input.instID].y) * input.weight.z;
		boneMat += LoadBoneTex(input.boneIndex.w, DrawData[input.instID].x, DrawData[input.instID].y) * input.weight.w;

		if (DrawData[input.instID].x > PREFRAME) {
			//!１つ前のフレームのボーン行列を取得
			int preFrame = DrawData[input.instID].x - PREFRAME;
			preBoneMat = LoadBoneTex(input.boneIndex.x, preFrame, DrawData[input.instID].y) * input.weight.x;
			preBoneMat += LoadBoneTex(input.boneIndex.y, preFrame, DrawData[input.instID].y) * input.weight.y;
			preBoneMat += LoadBoneTex(input.boneIndex.z, preFrame, DrawData[input.instID].y) * input.weight.z;
			preBoneMat += LoadBoneTex(input.boneIndex.w, preFrame, DrawData[input.instID].y) * input.weight.w;
		}
		else {
			preBoneMat = boneMat;
		}
	}

	matrix boneRotate = removeMoveVec(boneMat);

	float4 localPos = mul(float4(input.pos, 1), boneMat);
	float4 world = mul(localPos, World[input.instID]);

	//1フレーム前のポジション
	float4 prePos = mul(float4(input.pos, 1), preBoneMat);
	prePos = mul(prePos, World[input.instID]);


	float3 dir = prePos.xyz - world.xyz;
	float isB;
	if (isB = isBlur(dir, input.normal, DrawData[input.instID].x)) {
		//world = prePos;
	}

	matrix vp = VP();

	float4 pos = mul(world, vp);

	float4 lightViewPos = mul(world, ShadowViewMatrix);
	lightViewPos = mul(lightViewPos, Projection);


	VS_OUT outPut;
	outPut.pos = pos;
	outPut.prePos = mul(prePos, vp);
	outPut.worldPos = world;
	outPut.normal = mul(float4(input.normal, 1), boneRotate);
	outPut.binormal = input.binormal;
	outPut.tangent = input.tangent;
	outPut.uv = input.uv;
	outPut.instID = input.instID;
	outPut.isBlur = isB;


	float velocity = clamp(length(dir) / 500, 0, 1);
	if (dir.x == 0 && dir.y == 0) {
		outPut.velocity = float4(0, 0, velocity, 1);
	}
	else {
		outPut.velocity = float4(normalize(dir).xy, velocity, 1);
	}


	float2 depthCoord;
	depthCoord.x = (input.pos.x + 1) * 0.5;
	depthCoord.y = (1 - input.pos.y) * 0.5;

	outPut.depthCoord = lightViewPos;
	outPut.depthCoord = pos;

	return outPut;
}

//
//// *****************************************************************************************
//// ハルシェーダーのパッチ定数フェーズ
//// *****************************************************************************************
//CONSTANT_HS_OUT ConstantsHS(InputPatch<VS_OUT, 3> ip, uint PatchID : SV_PrimitiveID)
//{
//	CONSTANT_HS_OUT Out;
//
//	// 定数バッファの値をそのまま渡す
//	Out.Edges[0] = Out.Edges[1] = Out.Edges[2] = 1;  // パッチのエッジのテッセレーション係数
//	Out.Inside = 1;  // パッチ内部の横方法のテッセレーション係数
//
//	return Out;
//}
//
//// *****************************************************************************************
//// ハルシェーダーのコントロール ポイント フェーズ
//// *****************************************************************************************
//[domain("tri")]                             // テッセレートするメッシュの形状を指定する
//[partitioning("integer")]                    // パッチの分割に使用するアルゴリズムを指定する
//[outputtopology("triangle_cw")]             // メッシュの出力方法を指定する
//[outputcontrolpoints(3)]                     // ハルシェーダーのコントロール ポイント フェーズがコールされる回数
//[patchconstantfunc("ConstantsHS")]      // 対応するハルシェーダーのパッチ定数フェーズのメイン関数
//VS_OUT HS(InputPatch<VS_OUT, 3> In, uint i : SV_OutputControlPointID, uint PatchID : SV_PrimitiveID)
//{
//	VS_OUT Out;
//
//	// そのまま渡す
//	Out = In[i];
//	return Out;
//}
//
//
////頂点シェーダの出力
////struct VS_OUT {
////	float4 pos : SV_Position;
////	float4 normal : NORMAL;
////	float3 binormal : BINORMAL;
////	float3 tangent : TANGENT;
////	float2 uv : TEXTURE0;
////	uint instID : TEXTURE1;
////	float4 worldPos : TEXTURE2;
////	float4 depthCoord : TEXTURE3;
////	float4 projPos : TEXTURE4;
////};
//
//// *****************************************************************************************
//// ドメインシェーダー
//// *****************************************************************************************
//[domain("tri")]
//VS_OUT DS(CONSTANT_HS_OUT In, float3 loc : SV_DomainLocation, const OutputPatch<VS_OUT, 3> patch)
//{
//	VS_OUT Out;
//
//	Out.pos = patch[0].pos * loc.z + patch[1].pos * loc.x + patch[2].pos * loc.y;
//
//	return Out;
//}

//struct VS_OUT {
//	float4 pos : SV_Position;
//	float4 normal : NORMAL;
//	float3 binormal : BINORMAL;
//	float3 tangent : TANGENT;
//	float2 uv : TEXTURE0;
//	uint instID : TEXTURE1;
//	float4 worldPos : TEXTURE2;
//	float4 depthCoord : TEXTURE3;
//	float4 projPos : TEXTURE4;
//};

//キューブマップの各方向に描画するので18頂点
[maxvertexcount(18)]
void GS(triangle VS_OUT input[3], inout TriangleStream<VS_OUT> outStream) {
	VS_OUT outPut;
	//3頂点
	for (int v = 0; v != 3; ++v) {
		outPut.pos = input[v].pos;
		outPut.prePos = input[v].prePos;
		outPut.normal = input[v].normal;
		outPut.binormal = input[v].binormal;
		outPut.tangent = input[v].tangent;
		outPut.uv = input[v].uv;
		outPut.instID = input[v].instID;
		outPut.worldPos = input[v].worldPos;
		outPut.depthCoord = input[v].depthCoord;
		outPut.projPos = input[v].projPos;
		outPut.velocity = input[v].velocity;
		outPut.isBlur = false;//適当に送る

		outStream.Append(outPut);
	}
	outStream.RestartStrip();

	//for (int i = 0; i != PREFRAME; ++i) {

	//	for (int v = 0; v != 3; ++v) {
	//		outPut.pos = input[v].prePos;
	//		outPut.prePos = input[v].prePos;
	//		outPut.normal = input[v].normal;
	//		outPut.binormal = input[v].binormal;
	//		outPut.tangent = input[v].tangent;
	//		outPut.uv = input[v].uv;
	//		outPut.instID = input[v].instID;
	//		outPut.worldPos = input[v].worldPos;
	//		outPut.depthCoord = input[v].depthCoord;
	//		outPut.projPos = input[v].projPos;
	//		outPut.velocity = input[v].velocity;
	//		outPut.isBlur = true;//適当に送る

	//		outStream.Append(outPut);
	//	}
	//	outStream.RestartStrip();
	//}

	//for (int i = 0; i != 3; ++i) {
	//	if (!input[i].isBlur) continue;

	//	//2点を古いポジションで生成
	//	for (int v = 0; v != 2; ++v) {
	//		outPut.pos = input[v].pos;
	//		outPut.prePos = input[v].prePos;
	//		outPut.normal = input[v].normal;
	//		outPut.binormal = input[v].binormal;
	//		outPut.tangent = input[v].tangent;
	//		outPut.uv = input[v].uv;
	//		outPut.instID = input[v].instID;
	//		outPut.worldPos = input[v].worldPos;
	//		outPut.depthCoord = input[v].depthCoord;
	//		outPut.projPos = input[v].projPos;
	//		outPut.velocity = input[v].velocity;

	//		outStream.Append(outPut);
	//	}

	//	outPut.pos = input[2].prePos;
	//	outPut.prePos = input[2].prePos;
	//	outPut.normal = input[2].normal;
	//	outPut.binormal = input[2].binormal;
	//	outPut.tangent = input[2].tangent;
	//	outPut.uv = input[2].uv;
	//	outPut.instID = input[2].instID;
	//	outPut.worldPos = input[2].worldPos;
	//	outPut.depthCoord = input[2].depthCoord;
	//	outPut.projPos = input[2].projPos;
	//	outPut.velocity = input[2].velocity;

	//	outStream.Append(outPut);

	//	outStream.RestartStrip();

	//	//2点を新しポジションで生成
	//	for (int v = 0; v != 2; ++v) {
	//		outPut.pos = input[v].prePos;
	//		outPut.prePos = input[v].prePos;
	//		outPut.normal = input[v].normal;
	//		outPut.binormal = input[v].binormal;
	//		outPut.tangent = input[v].tangent;
	//		outPut.uv = input[v].uv;
	//		outPut.instID = input[v].instID;
	//		outPut.worldPos = input[v].worldPos;
	//		outPut.depthCoord = input[v].depthCoord;
	//		outPut.projPos = input[v].projPos;
	//		outPut.velocity = input[v].velocity;

	//		outStream.Append(outPut);
	//	}

	//	outPut.pos = input[2].pos;
	//	outPut.prePos = input[2].prePos;
	//	outPut.normal = input[2].normal;
	//	outPut.binormal = input[2].binormal;
	//	outPut.tangent = input[2].tangent;
	//	outPut.uv = input[2].uv;
	//	outPut.instID = input[2].instID;
	//	outPut.worldPos = input[2].worldPos;
	//	outPut.depthCoord = input[2].depthCoord;
	//	outPut.projPos = input[2].projPos;
	//	outPut.velocity = input[2].velocity;

	//	outStream.Append(outPut);

	//	outStream.RestartStrip();


	//}
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
	float4 color = colorTex.Sample(smpWrap, input.uv);
	//0~1を-1~1の範囲に変更する
	float4 normal = normalTex.Sample(smpWrap, input.uv) * 2.0 - 1.0;
	//float4 normal = normalEyeMap(ViewVec(input.worldPos), input.uv);
	//ノーマルマップを接空間に移動する
	normal = mul(normal, InvTangentMatrix(input.tangent, input.binormal, input.normal));
	normal = mul(normal, Roate[input.instID]);
	normal = normalize(normal);

	//matrix textureMat = matrix(
	//	0.5, 0.0, 0.0, 0.0,
	//	0.0, -0.5, 0.0, 0.0,
	//	0.0, 0.0, 1.0, 0.0,
	//	0.5, 0.5, 0.0, 1.0);

	//float ZValue = input.depthCoord.z / input.depthCoord.w;
	//float2 depthTexCoord;
	//depthTexCoord.x = (1.0 + input.depthCoord.x / input.depthCoord.w) * 0.5;
	//depthTexCoord.y = (1.0 - input.depthCoord.y / input.depthCoord.w) * 0.5;

	//float isShadow = shadowTex.Sample(smpWrap, input.depthCoord).r + 0.0000000005;

	//isShadow = isShadow > input.projPos.z;


	//outColor.color = float4(isShadow, isShadow, isShadow,1);


	//outColor.color = boneTexture.Load(int4(0, 0, input.instID, 0));
	//outColor.color = boneTexture.Sample(smpWrap,float3(0,0,1));
	//outColor.color = isShadow;

	if (input.isBlur) {
		outColor.color = (float4)0;
		outColor.environment = (float4)0;
		outColor.normal = (float4)0;;
		outColor.world = (float4)0;
		outColor.materialParam = (float4)0;
		outColor.specular = (float4)0;
		outColor.metalness.x = (float4)0;
		outColor.metalness.y = (float4)0;
		outColor.velocity = float4(input.velocity, 1);
		return outColor;
	}
	
	outColor.color = color;
	outColor.color.w = 1.0;
	float3 ref = reflect(input.worldPos - EyePos, normal);
	outColor.environment = environmentTex.Sample(smpWrap, float4(ref, 0));
	outColor.normal = normal;
	outColor.world = input.worldPos;
	outColor.materialParam = float4(Specular, isNotLighting, 0, 1);
	outColor.specular = specularTex.Sample(smpWrap, input.uv);
	outColor.metalness.x = metalnessTex.Sample(smpWrap, input.uv).x;
	outColor.metalness.y = roughnessTex.Sample(smpWrap, input.uv).x;
	outColor.velocity = float4(input.velocity, 1);
	return outColor;
	//return float4(Height,Specular, 0, 1);
}