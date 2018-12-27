// �w�b�_�t�@�C���̓ǂݍ��� =================================================
#include "../Header/SpriteHeader.hlsli"

// �O���[�o���ϐ��̒�` =====================================================
// �e�N�X�`��
Texture2D tex : register(t0);
// �T���v���[
SamplerState samplerState : register(s0);


// �s�N�Z���V�F�[�_�[�̒�` =================================================
float4 main(PS_INPUT input) : SV_TARGET
{
    // �e�N�X�`���̎擾����
    float4 texData = tex.Sample(samplerState, input.texcoord);

    // ���Z����
    return input.color * texData;
}
