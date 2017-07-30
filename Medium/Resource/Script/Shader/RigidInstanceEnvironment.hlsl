//�ő�`�搔��100
#define MAX_DRAW_NUM 100
//�L���[�u�}�b�s���O�p�r���[�s��̐�
#define VIEW_NUM 6

// �萔�o�b�t�@�̃f�[�^��`�@�@�ύX���Ȃ��f�[�^
cbuffer ConstBuffer : register(b0) { // ��ɃX���b�g�u0�v���g��
	matrix Projection;   // �����ϊ��s��
};

// �萔�o�b�t�@�̃f�[�^��`�A�@�ύX�p�x�̒Ⴂ�f�[�^
cbuffer UpdateBuffer : register(b1) { // ��ɃX���b�g�u1�v���g��
	matrix View[VIEW_NUM];   // �r���[�ϊ��s��
	matrix World[MAX_DRAW_NUM];   // ���W�ϊ��s��
	matrix Rotate[MAX_DRAW_NUM];   // ��]�s��
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
	float2 uv : TEXTURE;
	uint instID : TEXTURE1;
};

//�W�I���g���V�F�[�_�̏o��
struct GS_OUT {
	float4 pos : SV_Position;
	float4 normal : NORMAL;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
	float2 uv : TEXTURE;
	uint instID : TEXTURE1;//�s�N�Z���V�F�[�_����]�l���C���X�^���X���ƂɎ擾���邽�߂ɓn��
	uint rtIndex : SV_RenderTargetArrayIndex;//�`��^�[�^�[�Q�b�g
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

//�L���[�u�}�b�v�̊e�����ɕ`�悷��̂�18���_
[maxvertexcount(18)]
void GS(triangle VS_OUT input[3],inout TriangleStream<GS_OUT> outStream) {

	//6�ʂɕ`�悷��
	for (int f = 0; f != 6; ++f) {
		GS_OUT outPut;
		outPut.rtIndex = f;//f�Ԗڂ̃t���O�����g�ɏo��

		//3���_
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

