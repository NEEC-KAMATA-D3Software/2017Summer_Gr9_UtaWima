//最大描画数は100
#define MAX_DRAW_NUM 100
//キューブマッピング用ビュー行列の数
#define VIEW_NUM 6

// 定数バッファのデータ定義①　変更しないデータ
cbuffer ConstBuffer : register(b0) { // 常にスロット「0」を使う
	matrix Projection;   // 透視変換行列
};

// 定数バッファのデータ定義②　変更頻度の低いデータ
cbuffer UpdateBuffer : register(b1) { // 常にスロット「1」を使う
	matrix View[VIEW_NUM];   // ビュー変換行列
	matrix World[MAX_DRAW_NUM];   // 座標変換行列
	matrix Rotate[MAX_DRAW_NUM];   // 回転行列
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
	float2 uv : TEXTURE;
	uint instID : TEXTURE1;
};

//ジオメトリシェーダの出力
struct GS_OUT {
	float4 pos : SV_Position;
	float4 normal : NORMAL;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
	float2 uv : TEXTURE;
	uint instID : TEXTURE1;//ピクセルシェーダが回転値をインスタンスごとに取得するために渡す
	uint rtIndex : SV_RenderTargetArrayIndex;//描画ターターゲット
};


VS_OUT VS(VS_INPUT input) {

	float4 pos;
	pos = mul(float4(input.pos, 1), World[input.instID]);

	VS_OUT outPut;
	outPut.pos = pos;
	outPut.normal = float4(input.normal, 1);
	outPut.binormal = input.binormal;
	outPut.tangent = input.tangent;
	outPut.uv = input.uv;
	outPut.instID = input.instID;
	return outPut;
}

//キューブマップの各方向に描画するので18頂点
[maxvertexcount(18)]
void GS(triangle VS_OUT input[3],inout TriangleStream<GS_OUT> outStream) {

	//6面に描画する
	for (int f = 0; f != 6; ++f) {
		GS_OUT outPut;
		outPut.rtIndex = f;//f番目のフラグメントに出力

		//3頂点
		for (int v = 0; v != 3; ++v) {
			float4 pos = mul(input[v].pos, View[f]);
			outPut.pos = mul(pos, Projection);
			outPut.normal = input[v].normal;
			outPut.binormal = input[v].binormal;
			outPut.tangent = input[v].tangent;
			outPut.uv = input[v].uv;
			outPut.instID = input[v].instID;

			outStream.Append(outPut);
		}
		outStream.RestartStrip();
	}

}

float4 PS(GS_OUT input) : SV_Target
{
	return float4(input.uv,0,1);
}

