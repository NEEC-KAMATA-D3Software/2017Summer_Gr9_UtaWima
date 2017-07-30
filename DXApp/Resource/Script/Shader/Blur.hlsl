// 定数バッファのデータ定義①　変更しないデータ
cbuffer ConstBuffer : register(b0) { // 常にスロット「0」を使う
	matrix Projection;   // 透視変換行列
};

// 定数バッファのデータ定義②　変更頻度の低いデータ
cbuffer UpdateBuffer : register(b1) { // 常にスロット「1」を使う
	matrix World;   // 座標変換行列
	float4 pixSize;//ピクセルサイズ
};

Texture2D diffuse: register(t0);   // テクスチャ
Texture2D velocityTex: register(t1);   // テクスチャ

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

VS_OUT VS(VS_INPUT input) {

	float4 pos;
	pos = mul(float4(input.pos, 1), World);
	pos = mul(pos, Projection);

	VS_OUT outPut;
	outPut.pos = pos;
	outPut.uv = input.uv;
	return outPut;
}


float4 velocityGradation(float2 uv, float2 dir, float length) {
	float rate = length ;
	float2 temp = dir * length * 500;
	//float2 temp = dir;
	float2 shift = float2(pixSize.x * temp.x, pixSize.y * temp.y) ;

	float4 color = diffuse.Sample(smpWrap, uv + shift);
	color += diffuse.Sample(smpWrap, uv - shift);

	return color/2;
}



float4 PS(VS_OUT input) : SV_TARGET
{
	float4 velocity = velocityTex.Sample(smpWrap,  input.uv);
	return velocityGradation(input.uv , velocity.xy, velocity.z);

	//return velocity;
}