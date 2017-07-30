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
	matrix Roate[MAX_DRAW_NUM];   // 座標変換行列
};

// 定数バッファのデータ定義③　変更頻度の低いデータ
cbuffer UpdateBuffer2 : register(b2) { // 常にスロット「2」を使う
	float4 Color[MAX_DRAW_NUM];
};


//頂点シェーダの入力
struct VS_INPUT {
	float3 pos : SV_Position;
	float3 normal : NORMAL;
	uint instID : SV_InstanceID;
};
//頂点シェーダの出力
struct VS_OUT {
	float4 pos : SV_Position;
	float4 color : COLOR;
	float3 normal : NORMAL;
	uint instID : TEXTURE1;
	float4 worldPos : TEXTURE2;
};

//ピクセルシェーダの出力
struct PS_OUT {
	float4 color : SV_TARGET0;
	float4 normal : SV_TARGET1;
	float4 world : SV_TARGET2;
};

VS_OUT VS(VS_INPUT input) {

	float4 world = mul(float4(input.pos, 1), World[input.instID]);
	float4 pos = mul(world, View);
	pos = mul(pos, Projection);

	VS_OUT outPut;
	outPut.pos = pos;
	outPut.worldPos = world;
	outPut.normal = input.normal;
	outPut.color = Color[input.instID];
	outPut.instID = input.instID;
	return outPut;
}

PS_OUT PS(VS_OUT input)
{
	PS_OUT outColor;

	outColor.color = input.color;
	outColor.normal = mul(float4(input.normal,1) , Roate[input.instID]);
	outColor.normal = normalize(outColor.normal);
	outColor.world = input.worldPos;

	return outColor;
}