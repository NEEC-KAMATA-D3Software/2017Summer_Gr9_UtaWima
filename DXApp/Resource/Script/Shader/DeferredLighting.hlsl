
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
	//�r���[�̃x�N�g���쐬
	float3 viewVec = normalize(CameraPos - worldPos);
	float3 H = normalize(lightVec + viewVec);

	float s = dot(normal, H);
	if (s < 0) {
		return 0;
	}
	return max(pow(s, specularPower), 0.0);

}

/**
* @brief	1������̃|�C���g���C�g�̌v�Z������
*/
float2 PointLight(float3 normal, float3 worldPos, int location, float specularPower) {

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
* @brief	���̃s�N�Z���̂��ׂẴ|�C���g���C�g�̌v�Z������
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
* @brief	���^���l�X�v�Z
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
* @brief	���^�����v�Z
*/
float4 Metal(float4 diffuse, float specular, float4 env) {

}


//�s�N�Z���V�F�[�_�̏o��
struct PS_OUT {
	float4 color : SV_TARGET0;
	float4 specular : SV_TARGET1;
};

/**
* @brief	�s�N�Z���V�F�[�_�̃G���g���|�C���g
*/
PS_OUT PS(VS_OUT input)
{
	float4 diffuse = pow(colorBuffer.Sample(smpWrap,input.uv),2.2);

	float3 normal = normalBuffer.Sample(smpWrap, input.uv).xyz;

	float4 environment = pow(environmentBuffer.Sample(smpWrap, input.uv),2.2);

	float3 world = worldBuffer.Sample(smpWrap, input.uv).xyz;

	float4 param = paramBuffer.Sample(smpWrap, input.uv);

	//���C�e�B���O���Ȃ����̂͂��̂܂܏o��
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
	//outColor = max(pointColor, directionalColor);//�������ŏ㏑��
	//outColor = directionalColor;
	outColor = pointColor + directionalColor;

	float directionalSpecular = Specular(normal, world, light, param.x) * power;
	float s = 0;
	s = max(pointSpecular, directionalSpecular);//�������ŏ㏑��
	//s = directionalSpecular;
	s *= specularPower;//�X�y�L�����e�N�X�`���̒l��������
	//s = pow(s,1/2.2);

	float3 viewVec = normalize(CameraPos - world);
	float3 H = normalize(DirectionalLightPos + viewVec);
	environment = max(dot(normal, H), 0.0) * environment * specularPower;

	//!�ŏI�I�ɏo�͂������
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