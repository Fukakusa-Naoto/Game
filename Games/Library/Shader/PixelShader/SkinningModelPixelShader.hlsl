// �w�b�_�t�@�C���̓ǂݍ��� =================================================
#include "../Header/SkinningModel.hlsli"


// �s�N�Z���V�F�[�_�[�̒�` =================================================
float4 main(PS_INPUT input) : SV_TARGET
{
    float4 output = 0;

    // �e�N�X�`���̎擾
    float4 textureData = tex.Sample(samplerState, input.texcoord);

    // �e�N�X�`�����Ȃ�
    if ((textureData.x == 0.0f) && (textureData.y == 0.0f) && (textureData.z == 0.0f) && (textureData.w == 0.0f))
        return float4(1.0f, 1.0f, 1.0f, 1.0f);
    else
        return textureData;
}
