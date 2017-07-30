//�ő�`�搔��100
#define MAX_DRAW_NUM 50

// �萔�o�b�t�@�̃f�[�^��`�@�@�ύX���Ȃ��f�[�^
cbuffer ConstBuffer : register(b0) { // ��ɃX���b�g�u0�v���g��
	matrix Projection;   // �����ϊ��s��
};

// �萔�o�b�t�@�̃f�[�^��`�A�@�ύX�p�x�̒Ⴂ�f�[�^
cbuffer UpdateBuffer : register(b1) { // ��ɃX���b�g�u1�v���g��
	matrix View;   // �r���[�ϊ��s��
	matrix World[MAX_DRAW_NUM];   // ���W�ϊ��s��
	matrix Roate[MAX_DRAW_NUM];   //�@������]�����邽�߂̉�]�s��
	matrix ShadowViewMatrix; //���V���h�E�}�b�s���O�p�s��
	float3 EyePos;
	float Damy1;
};

//�}�e���A���̃p�����[�^�p�o�b�t�@
cbuffer MaterialBuffer : register(b2) {
	float Height;
	float Specular;
	float2 Damy2;
}

Texture2D colorTex : register(t0);   // �e�N�X�`��
Texture2D normalTex : register(t1);   // �e�N�X�`��
Texture2D heightTex : register(t2);   // �e�N�X�`��
Texture2D specularTex : register(t3);   // �e�N�X�`��
Texture2D metalnessTex : register(t4);   // �e�N�X�`��
Texture2D roughnessTex : register(t5);   // �e�N�X�`��

Texture2D shadowTex : register(t9);   // �V���h�E�}�b�v
TextureCubeArray environmentTex : register(t10);   // ���e�N�X�`��
//TextureCube environmentTex[6] : register(t10);   // �T���v��
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


//���_�V�F�[�_�̓���
struct VS_INPUT {
	float3 pos : SV_Position;
	float3 normal : NORMAL;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
	float2 uv : TEXTURE;
	uint instID : SV_InstanceID;
};
//���_�V�F�[�_�̏o��
struct VS_OUT {
	float4 pos : SV_Position;
	float4 normal : NORMAL;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
	float2 uv : TEXTURE0;
	uint instID : TEXTURE1;
	float4 worldPos : TEXTURE2;
};

//�s�N�Z���V�F�[�_�̏o��
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
* @brief				�m�[�}���}�b�v��ڋ�Ԃɂ��邽�߂̍s��
* @param tangent		�^���W�F���g
* @param binormal		�o�C�m�[�}��
* @param vertexNormal	���_�̖@��
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
* @brief				���_�x�N�g���Z�o
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
	//!�e������
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
	//0~1��-1~1�͈̔͂ɕύX����
	float4 normal = normalTex.Sample(smpWrap, input.uv) * 2.0 - 1.0;
	//float4 normal = normalEyeMap(ViewVec(input.worldPos), input.uv);
	//�m�[�}���}�b�v��ڋ�ԂɈړ�����
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