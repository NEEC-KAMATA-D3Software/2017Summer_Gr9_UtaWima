// 定数バッファのデータ定義①　変更しないデータ
cbuffer ConstBuffer : register(b0) { // 常にスロット「0」を使う
	matrix Projection;   // 透視変換行列
};

// 定数バッファのデータ定義②　変更頻度の低いデータ
cbuffer UpdateBuffer : register(b1) { // 常にスロット「1」を使う
	matrix World;   // 座標変換行列
	float bright; //ブルーム輝度
	float2 pixSize;//ピクセルサイズ
	float damy;
};

Texture2D diffuse: register(t0);   // テクスチャ
Texture2D specular: register(t1);   // テクスチャ

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


struct PS_INPUT {

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

/**
* @brief	スペキュラのぼかし処理
*/
float4 gradationSpecular(float2 inUv) {
	float4 result = (float4)0;
	float2 uv = pixSize ;

	result = specular.Sample(smpWrap,  inUv + uv);
	result += specular.Sample(smpWrap, inUv + -uv);
	result += specular.Sample(smpWrap, inUv + float2(-uv.x, uv.y));
	result += specular.Sample(smpWrap, inUv + float2(uv.x, -uv.y));

	return result / 4;
}

float4 PS(VS_OUT input) : SV_TARGET
{
	float s = specular.Sample(smpWrap,  input.uv).x;
    float4 gs =  gradationSpecular(input.uv);
	float4 d = diffuse.Sample(smpWrap, input.uv);
	//return lerp(d, gs,s);
	
	return  diffuse.Sample(smpWrap,  input.uv) +( gradationSpecular(input.uv) * 1.5 );
	//return  diffuse.Sample(smpWrap,  input.uv);
	//return  gradationSpecular(input.uv);
	//return specular.Sample(smpWrap, input.uv);
}