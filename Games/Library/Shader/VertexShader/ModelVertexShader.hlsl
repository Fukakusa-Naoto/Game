// �w�b�_�t�@�C���̓ǂݍ��� =================================================
#include "../Header/Model.hlsli"


// �\���̂̒�` ===========================================================
// �萔�o�b�t�@�̍\����
cbuffer ConstBuffer : register(b0)
{
    // ���[���h�s��
    Matrix world;
    // �r���[�s��
    Matrix view;
    // �ˉe�s��
    Matrix projection;
};


// ���_�V�F�[�_�[�̒�` ====================================================
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    // ���W�ϊ�
    output.position = mul(float4(input.position, 1.0f), world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);

    // uv���W
    output.texcoord = input.texcoord;

    return output;
}