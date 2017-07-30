
/**
* @file	DeferredLighting.hlsl
* @brief	�x�������_�����O�V�F�[�_�[
* @authro	���{�D�P
* @date	2017/03/26
*/

#define MAX_POINT_LIGHT_NUM 61
#define ANBIENT 0.15

// �萔�o�b�t�@�̃f�[�^��`�@�@�ύX���Ȃ��f�[�^
cbuffer ConstBuffer : register(b0) { // ��ɃX���b�g�u0�v���g��
	matrix Projection;   // �����ϊ��s��
};

// �萔�o�b�t�@�̃f�[�^��`�A�@�ύX�p�x�̒Ⴂ�f�[�^
cbuffer UpdateBuffer : register(b1) { // ��ɃX���b�g�u1�v���g��
	matrix World;   // ���W�ϊ��s��
	float4 DirectionalLightPos;
	float4 DirectionalLightColor;
	float4 LightPos[MAX_POINT_LIGHT_NUM];
	float4 Attrib[MAX_POINT_LIGHT_NUM];
	float4 PointColor[MAX_POINT_LIGHT_NUM];
	int  ActiveLightNum;
	float3 CameraPos;
};

Texture2D colorBuffer : register(t0);   //! �J���[�e�N�X�`��
Texture2D normalBuffer : register(t1);   //! �m�[�}���e�N�X�`��
Texture2D worldBuffer : register(t2);   //! ���[���h���W�e�N�X�`��
Texture2D paramBuffer : register(t3);   //! �}�e���Al�p�����[�^�e�N�X�`��
Texture2D specularBuffer : register(t4);   //! �X�y�L�����[�e�N�X�`��
Texture2D environmentBuffer : register(t5);   //! ���}�b�s���O�e�N�X�`��
Texture2D metalRoughnessBuffer : register(t6);   //! ���^���l�X&���t�l�X�e�N�X�`��


					 // �T���v��
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
	//�r���[�̃x�N�g���쐬
	float3 viewVec = normalize(CameraPos - worldPos);
	float3 H = normalize(lightVec + viewVec);

	float s = dot(normal, H);
	return max(pow(s, specularPower),0.0);

}

/**
* @brief	1������̃|�C���g���C�g�̌v�Z������
*/
float2 PointLight(float3 normal, float3 worldPos, int location,float specularPower) {

	float3 lihgtVec = LightPos[location] - worldPos;
	//!���C�g�̉e���x
	float lightLength = length(lihgtVec);
	//!���C�g�̕���
	lihgtVec = normalize(lihgtVec);

	// //�ő�̖��邳�@0(Max) ~ 1(�Œ�)
	// float maxBright = 0.5;
	// //�߂��������ǂ�قǏƂ炷��0(Max) ~ 1(�Œ�)
	// float lightNear = 0.5;
	// //���������܂œ͂��₷��0(Max) ~ 1(�Œ�)
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
* @brief	���̃s�N�Z���̂��ׂẴ|�C���g���C�g�̌v�Z������
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
* @brief	�U�d�̌v�Z
*/
float4 Dielectric(float4 diffuse, float specular, float4 env,float roughness) {

	float4 color = diffuse + specular;

	color = (1 - roughness) * color + env * roughness;

	return color;
}

/**
* @brief	���^�����v�Z
*/
float4 Metal(float4 diffuse,float specular ,float4 env) {

}

/**
* @brief	�s�N�Z���V�F�[�_�̃G���g���|�C���g
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
	outColor = max(pointColor, directionalColor);//�������ŏ㏑��

	float directionalSpecular = Specular(normal, world, light, param.x) * power;
	float s = 0;
	s = max(pointSpecular, directionalSpecular);//�������ŏ㏑��
	s *= specularPower;//�X�y�L�����e�N�X�`���̒l��������
	//const float m = 0.0;
	//outColor = (1.0 - m) * outColor + m * s;//���ʂ�ۂ����܂܃X�y�L�����𑫂�

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