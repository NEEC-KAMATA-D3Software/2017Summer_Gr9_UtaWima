
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

VS_OUT VS(VS_INPUT input){

	float4 pos;
	pos = mul(float4(input.pos,1), World);
	pos = mul(pos, Projection);

	VS_OUT outPut;
	outPut.pos = pos;
	outPut.uv = input.uv;
	return outPut;
}

float4 Decode(float4 color) {
	return color * 0.5 - 0.5;
}

float Specular(float3 normal, float3 worldPos,float3 lightVec,float specularPower) {
	//ビューのベクトル作成
	float3 viewVec = normalize(CameraPos - worldPos);
	float3 H = normalize(lightVec + viewVec);

	float s = dot(normal, H);
	return max(pow(s, specularPower),0.0);

}

/**
* @brief	1つあたりのポイントライトの計算をする
*/
float2 PointLight(float3 normal, float3 worldPos, int location,float specularPower) {

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
	float num = (Attrib[location].x+
				 Attrib[location].y * lightLength +
				 Attrib[location].z  * pow(lightLength, 2));
	float attenution = 0;
	if (num != 0)
		attenution = 1.0 / num;

	float pointPower = max(dot(lihgtVec, normal), ANBIENT) * attenution;
	float specular = Specular(normal, worldPos, lihgtVec, specularPower) * attenution;
	return float2(pointPower , specular);
}

/**
* @brief	このピクセルのすべてのポイントライトの計算をする
*/
void  AllPointLighting(float3 normal, float3 worldPos, float specularPower, out float4 pointBright, out float outSpecular) {
	if (ActiveLightNum >= MAX_POINT_LIGHT_NUM){
		pointBright = (float4)0;
		outSpecular = 0;
		return;
	}
	float4 bright = (float4)0;
	float specular = (float4)0;
	for (int i = 0; i != ActiveLightNum; i++) {
		float2 attrib = PointLight(normal, worldPos, i, specularPower);
		bright = max(PointColor[i] * attrib.x, bright);
		specular = max(attrib.y, specular);
	}
	pointBright = bright;
	outSpecular = specular;
}

/**
* @brief	誘電体計算
*/
float4 Dielectric(float4 diffuse, float specular, float4 env,float roughness) {

	float4 color = diffuse + specular;

	color = (1 - roughness) * color + env * roughness;

	return color;
}

/**
* @brief	メタル質計算
*/
float4 Metal(float4 diffuse,float specular ,float4 env) {

}

/**
* @brief	ピクセルシェーダのエントリポイント
*/
float4 PS(VS_OUT input) : SV_TARGET
{
	float4 diffuse = colorBuffer.Sample(smpWrap,input.uv);

	float4 environment = environmentBuffer.Sample(smpWrap, input.uv);
	
	float3 normal = normalBuffer.Sample(smpWrap, input.uv).xyz;

	float3 world = worldBuffer.Sample(smpWrap, input.uv).xyz;

	float4 param = paramBuffer.Sample(smpWrap, input.uv);
	
	float specularPower = specularBuffer.Sample(smpWrap, input.uv).x;

	float2 metalRoghness = metalRoughnessBuffer.Sample(smpWrap, input.uv).xy;

	float3 light = normalize(DirectionalLightPos.xyz - float3(0,0,0));

	float power = max(dot(normalize(normal),light) , ANBIENT);


	float4 pointPower; 
	float pointSpecular;
	AllPointLighting(normal, world, param.x, pointPower, pointSpecular);

	float4 outColor = (float4)0;
	float4 pointColor = diffuse *  pointPower;
	float4 directionalColor = diffuse * DirectionalLightColor * power;
	outColor = max(pointColor, directionalColor);//強い光で上書き

	float directionalSpecular = Specular(normal, world, light, param.x) * power;
	float s = 0;
	s = max(pointSpecular, directionalSpecular);//強い光で上書き
	s *= specularPower;//スペキュラテクスチャの値をかける
	//const float m = 0.0;
	//outColor = (1.0 - m) * outColor + m * s;//質量を保ったままスペキュラを足す

	//if (metalRoghness.x == 0) {
	//	outColor = Dielectric(outColor, s, environment);
	//}
	//else {
	//	
	//}

	outColor = pow(Dielectric(outColor, s, environment, metalRoghness.y), 1/2.2);
	outColor.w = 1.0f;
	//float power = dot(normalize(normal.xyz), light)  * 0.5 + 0.5;
	//return float4(normalize(CameraPos - world),1);
	return outColor;
	//return float4(pointSpecular, pointSpecular, pointSpecular, 1)  * power;
	//return float4(normalize(CameraPos - world), 1);
	//return float4(s, s, s, 1);
}