// �w�b�_�t�@�C���̓ǂݍ��� =================================================
#include "../Header/SpriteHeader.hlsli"

// ���_�V�F�[�_�[�̒�` =====================================================
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    output.position = float4(input.position, 1.0f);
    output.texcoord = input.texcoord;
    output.color = input.color;
    return output;
}