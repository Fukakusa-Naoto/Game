// �w�b�_�t�@�C���̓ǂݍ��� =================================================
#include "../Header/TileBackground.hlsli"


// �\���̂̒�` ===========================================================
// �萔�o�b�t�@�̍\����
cbuffer TimeBuffer : register(b0)
{
    // ����
    float4 time;
};


// �O���[�o���ϐ��̒�` =====================================================
// �e�N�X�`��
Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
// �T���v���[
SamplerState samplerState : register(s0);


// �s�N�Z���V�F�[�_�[�̒�` =================================================
float4 main(PS_INPUT input) : SV_TARGET
{
    // �e�N�X�`���̎擾����
    float4 texData = tex.Sample(samplerState, input.texcoord);

    float2 uv = input.texcoord;

    // ���ߕ����𑝂₷
    uv.y *= 1.5f;

    // �X�N���[��������
    uv.y -= time.x * 0.1f;

    // �v�Z���uv�Ń��[���摜���T���v�����O
    float4 texData2 = tex2.Sample(samplerState, uv);

    // ���Z����
    float4 color = input.color * texData * texData2;

    // �S�̂𔖂����߂���
    color.a *= abs(sin(time.x * 0.5f)) + 0.5f;

    // �o�͂���
    return color;
}
