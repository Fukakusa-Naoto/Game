// �萔�o�b�t�@
cbuffer ConstBuffer : register(b0)
{
    Matrix world;
    Matrix view;
    Matrix projection;
};


// ���_�V�F�[�_�[�ւ̓��͍\����
struct VS_INPUT
{
    // ���W
    float3 position : POSITION;
	// �F
	float4 color : COLOR;
};


// �s�N�Z���V�F�[�_�[�ւ̓��͍\����
struct PS_INPUT
{
    // ���W
    float4 position : SV_POSITION;
	// �F
	float4 color : COLOR;
	// �@���x�N�g��
	float3 normal : NORMAL;
};