// �w�b�_�t�@�C���̓ǂݍ��� =================================================
#include "../Header/Model.hlsli"


// �O���[�o���ϐ��̒�` =====================================================
// �e�N�X�`��
Texture2D tex : register(t0);
// �T���v���[
SamplerState samplerState : register(s0);


// �s�N�Z���V�F�[�_�[�̒�` =================================================
float4 main(PS_INPUT input) : SV_TARGET
{
    // �e�N�X�`���̎擾
    float4 textureData = tex.Sample(samplerState, input.texcoord);

    // �e�N�X�`�����Ȃ�
    if ((textureData.x == 0.0f) && (textureData.y == 0.0f) && (textureData.z == 0.0f) && (textureData.w == 0.0f))
        return float4(1.0f, 1.0f, 1.0f, 1.0f);

    // �������ďo�͂���
    return textureData;
}
