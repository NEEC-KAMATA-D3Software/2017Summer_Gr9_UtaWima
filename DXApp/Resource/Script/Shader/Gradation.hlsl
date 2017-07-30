// �萔�o�b�t�@�̃f�[�^��`�@�@�ύX���Ȃ��f�[�^
cbuffer ConstBuffer : register(b0) { // ��ɃX���b�g�u0�v���g��
	matrix Projection;   // �����ϊ��s��
};

// �萔�o�b�t�@�̃f�[�^��`�A�@�ύX�p�x�̒Ⴂ�f�[�^
cbuffer UpdateBuffer : register(b1) { // ��ɃX���b�g�u1�v���g��
	matrix World;   // ���W�ϊ��s��
	float2 pixSize;//�s�N�Z���̃T�C�Y
	float2 damy;
};

Texture2D tex: register(t0);   // �e�N�X�`��
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
* @brief	�ڂ�������
*/
float4 gradation(float2 inUv) {
	float4 result = (float4)0;
	float2 uv = pixSize;

	result = tex.Sample(smpWrap,  inUv + uv);
	result +=tex.Sample(smpWrap, inUv + -uv);
	result +=tex.Sample(smpWrap, inUv + float2(-uv.x, uv.y));
	result +=tex.Sample(smpWrap, inUv + float2(uv.x, -uv.y));

	return result / 4;
}

float4 PS(VS_OUT input) : SV_TARGET
{
	return  gradation(input.uv);
}