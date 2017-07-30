// 定数バッファのデータ定義①　変更しないデータ
cbuffer ConstBuffer : register(b0) { // 常にスロット「0」を使う
	matrix Projection;   // 透視変換行列
};

// 定数バッファのデータ定義②　変更頻度の低いデータ
cbuffer UpdateBuffer : register(b1) { // 常にスロット「1」を使う
	matrix View;   // ビュー変換行列
	matrix World;   // 座標変換行列
};

Texture2D myTex2D;   // テクスチャ
					 // サンプラ
SamplerState smpWrap : register(s0);

struct VS_INPUT {
	float3 pos : SV_Position;
	float3 normal : NORMAL;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
	float2 uv : TEXTURE;
};

struct VS_OUT {
	float4 pos : SV_Position;
	float4 normal : NORMAL;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
	float2 uv : TEXTURE;
};


struct PS_INPUT {

};

VS_OUT VS(VS_INPUT input){

	float4 pos;
	pos = mul(float4(input.pos,1), World);
	pos = mul(pos, View);
	pos = mul(pos, Projection);

	VS_OUT outPut;
	outPut.pos = pos;
	outPut.normal = float4(input.normal,1);
	outPut.binormal = input.binormal;
	outPut.tangent = input.tangent;
	outPut.uv = input.uv;
	return outPut;
}

float4 PS(VS_OUT input) : SV_TARGET
{
	return float4(input.normal.xyz * 0.5 + 0.5,1);
	//return myTex2D.Sample(smpWrap,input.uv);
}