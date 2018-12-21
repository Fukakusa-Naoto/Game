// �w�b�_�t�@�C���̓ǂݍ��� =================================================
#include "../Header/Primitive3D.hlsli"


// ���_�V�F�[�_�[�̒�` =====================================================
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    // ���W�ϊ�
    output.position = mul(float4(input.position, 1.0f), world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);

    // �F
    output.color = input.color;

    return output;
}