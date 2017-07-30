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
	matrix Roate[MAX_DRAW_NUM];   // ���W�ϊ��s��
};

// �萔�o�b�t�@�̃f�[�^��`�B�@�ύX�p�x�̒Ⴂ�f�[�^
cbuffer UpdateBuffer2 : register(b2) { // ��ɃX���b�g�u2�v���g��
	float4 Color[MAX_DRAW_NUM];
};


//���_�V�F�[�_�̓���
struct VS_INPUT {
	float3 pos : SV_Position;
	float3 normal : NORMAL;
	uint instID : SV_InstanceID;
};
//���_�V�F�[�_�̏o��
struct VS_OUT {
	float4 pos : SV_Position;
	float4 color : COLOR;
	float3 normal : NORMAL;
	uint instID : TEXTURE1;
	float4 worldPos : TEXTURE2;
};

//�s�N�Z���V�F�[�_�̏o��
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