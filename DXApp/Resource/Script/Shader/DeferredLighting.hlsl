
/**
* @file	DeferredLighting.hlsl
* @brief	遅延レンダリングシェーダー
* @authro	高須優輝
* @date	2017/03/26
*/

#define MAX_POINT_LIGHT_NUM 61
#define ANBIENT 0.15

// 定数バッファのデータ定義①　変更しないデータ
cbuffer ConstBuffer : register(b0) { // 常にスロット「0」を使う
	matrix Projection;   // 透視変換行列
};

// 定数バッファのデータ定義②　変更頻度の低いデータ
cbuffer UpdateBuffer : register(b1) { // 常にスロット「1」を使う
	matrix World;   // 座標変換行列
	float4 DirectionalLightPos;
	float4 DirectionalLightColor;
	float4 LightPos[MAX_POINT_LIGHT_NUM];
	float4 Attrib[MAX_POINT_LIGHT_NUM];
	float4 PointColor[MAX_POINT_LIGHT_NUM];
	int  ActiveLightNum;
	float3 CameraPos;
};

Texture2D colorBuffer : register(t0);   //! カラーテクスチャ
Texture2D normalBuffer : register(t1);   //! ノーマルテクスチャ
Texture2D worldBuffer : register(t2);   //! ワールド座標テクスチャ
Texture2D paramBuffer : register(t3);   //! マテリアlパラメータテクスチャ
Texture2D specularBuffer : register(t4);   //! スペキュラーテクスチャ
Texture2D environmentBuffer : register(t5);   //! 環境マッピングテクスチャ
Texture2D metalRoughnessBuffer : register(t6);   //! メタルネス&ラフネステクスチャ


					 // サンプラ
SamplerState smpWrap : register(s0);

struct VS_INPUT {
	float3 pos : SV_Position;
	float2 uv : TEXTURE;
};

struct VS_OUT {
	float4 pos : SV_Position;
	float2 uv : TEXTURE;
};

VS_OUT VS(VS_INPUT input) {

	float4 pos;
	pos = mul(float4(input.pos, 1), World);
	pos = mul(pos, Projection);

	VS_OUT outPut;
	outPut.pos = pos;
	outPut.uv = input.uv;
	return outPut;
}

float4 Decode(float4 color) {
	return color * 0.5 - 0.5;
}

float Specular(float3 normal, float3 worldPos, float3 lightVec, float specularPower) {
	//ビューのベクトル作成
	float3 viewVec = normalize(CameraPos - worldPos);
	float3 H = normalize(lightVec + viewVec);

	float s = dot(normal, H);
	if (s < 0) {
		return 0;
	}
	return max(pow(s, specularPower), 0.0);

}

/**
* @brief	1つあたりのポイントライトの計算をする
*/
float2 PointLight(float3 normal, float3 worldPos, int location, float specularPower) {

	float3 lihgtVec = LightPos[location] - worldPos;
	//!ライトの影響度
	float lightLength = length(lihgtVec);
	//!ライトの方向
	lihgtVec = normalize(lihgtVec);

	// //最大の明るさ　0(Max) ~ 1(最低)
	// float maxBright = 0.5;
	// //近い距離をどれほど照らすか0(Max) ~ 1(最低)
	// float lightNear = 0.5;
	// //遠い距離まで届きやすさ0(Max) ~ 1(最低)
	// float lightFar = 0.0;
	float num = (Attrib[location].x +
		Attrib[location].y * lightLength +
		Attrib[location].z  * pow(lightLength, 2));
	float attenution = 0;
	if (num != 0)
		attenution = 1.0 / num;

	float pointPower = max(dot(lihgtVec, normal), ANBIENT) * attenution;
	float specular = Specular(normal, worldPos, lihgtVec, specularPower) * attenution;
	return float2(pointPower, specular);
}

/**
* @brief	このピクセルのすべてのポイントライトの計算をする
*/
void  AllPointLighting(float3 normal, float3 worldPos, float specularPower, out float4 pointBright, out float outSpecular) {
	if (ActiveLightNum >= MAX_POINT_LIGHT_NUM) {
		pointBright = (float4)0;
		outSpecular = 0;
		return;
	}
	float4 totalBright = (float4)0;
	float4 bright = (float4)0;
	float totalSpecular = (float4)0;
	float specular = (float4)0;
	for (int i = 0; i != ActiveLightNum; i++) {
		float2 attrib = PointLight(normal, worldPos, i, specularPower);
		bright = max(PointColor[i] * attrib.x, bright);
		totalBright += bright;
		specular = max(attrib.y, specular);
		totalSpecular += specular;
	}
	pointBright = totalBright;
	outSpecular = totalSpecular;
}


/**
* @brief	メタルネス計算
*/
float4 MetalCaluc(float4 diffuse, float4 env, float metalness) {

	float4 color = 0;

	color = lerp(diffuse, env, metalness);

	color.a = 1.0;

	return color;

	//float m = 0.06;
	//float4 color = lerp(diffuse, env,m);

	//return color;
}

/**
* @brief	メタル質計算
*/
float4 Metal(float4 diffuse, float specular, float4 env) {

}


//ピクセルシェーダの出力
struct PS_OUT {
	float4 color : SV_TARGET0;
	float4 specular : SV_TARGET1;
};

/**
* @brief	ピクセルシェーダのエントリポイント
*/
PS_OUT PS(VS_OUT input)
{
	float4 diffuse = pow(colorBuffer.Sample(smpWrap,input.uv),2.2);

	float3 normal = normalBuffer.Sample(smpWrap, input.uv).xyz;

	float4 environment = pow(environmentBuffer.Sample(smpWrap, input.uv),2.2);

	float3 world = worldBuffer.Sample(smpWrap, input.uv).xyz;

	float4 param = paramBuffer.Sample(smpWrap, input.uv);

	//ライティングしないものはそのまま出力
	if (param.y == 1) {
		PS_OUT result;
		result.color = pow(diffuse* DirectionalLightColor, 1 / 2.2);
		return result;
	}

	float specularPower = pow(specularBuffer.Sample(smpWrap, input.uv).x ,1);

	float2 metalRoghness = pow( metalRoughnessBuffer.Sample(smpWrap, input.uv).xy,2.2 ) ;

	float3 light = normalize(DirectionalLightPos.xyz - float3(0,0,0));

	float power = max(dot(normalize(normal),light) , ANBIENT);


	float4 pointPower;
	float pointSpecular;
	AllPointLighting(normal, world, param.x, pointPower, pointSpecular);

	float4 outColor = (float4)0;
	float4 pointColor = diffuse *  pointPower;
	float4 directionalColor = diffuse * DirectionalLightColor * power;
	//outColor = max(pointColor, directionalColor);//強い光で上書き
	//outColor = directionalColor;
	outColor = pointColor + directionalColor;

	float directionalSpecular = Specular(normal, world, light, param.x) * power;
	float s = 0;
	s = max(pointSpecular, directionalSpecular);//強い光で上書き
	//s = directionalSpecular;
	s *= specularPower;//スペキュラテクスチャの値をかける
	//s = pow(s,1/2.2);

	float3 viewVec = normalize(CameraPos - world);
	float3 H = normalize(DirectionalLightPos + viewVec);
	environment = max(dot(normal, H), 0.0) * environment * specularPower;

	//!最終的に出力される情報
	PS_OUT result;

	//outColor += directionalSpecular;
	outColor.w = 1.0f;


	//s = pow(s,1/2.2);
	result.color = pow(MetalCaluc(outColor, environment, metalRoghness.x), 1 / 2.2) + s;

	//result.color = float4(s, s, s, 1);

	result.color.a = 1.0;

	result.specular = float4(s, s, s, 1);
	return result;
}