// �萔�o�b�t�@�̃f�[�^��`�@�@�ύX���Ȃ��f�[�^
cbuffer ConstBuffer : register(b0) { // ��ɃX���b�g�u0�v���g��
	matrix Projection;   // �����ϊ��s��
};

// �萔�o�b�t�@�̃f�[�^��`�A�@�ύX�p�x�̒Ⴂ�f�[�^
cbuffer UpdateBuffer : register(b1) { // ��ɃX���b�g�u1�v���g��
	matrix World;   // ���W�ϊ��s��
	float4 pixSize;//�s�N�Z���T�C�Y
};

Texture2D diffuse: register(t0);   // �e�N�X�`��
Texture2D velocityTex: register(t1);   // �e�N�X�`��

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