// �萔�̒�` =============================================================
// �{�[���̍ő吔
#define MAX_BONES (48)


// �O���[�o���ϐ��̒�` ====================================================
// �e�N�X�`��
Texture2D tex : register(t0);
// �T���v���[
SamplerState samplerState : register(s0);

// �萔�o�b�t�@
cbuffer ConstBuffer : register(b0)
{
    // ���[���h�s��
    Matrix world;
    // �r���[�s��
    Matrix view;
    // �ˉe�s��
    Matrix projection;
    // ���C�g
    float4 light;
};


// �X�L�j���O�o�b�t�@
cbuffer SkinningBuffer : register(b1)
{
    float4x4 Bones[MAX_BONES];
};


// �\���̂̒�` ===========================================================
// ���_�V�F�[�_�[���͍\����
struct VS_INPUT
{
    // ���W
    float3 position : POSITION;
    // �@��
    float3 normal : NORMAL;
    // uv���W
    float2 texcoord : TEXCOORD;
    // �{�[���C���f�b�N�X
    uint4 skinIndex : SKIN_INDEX;
    // �E�F�C�g
    float4 skinWeight : SKIN_WEIGHT;
};


// �s�N�Z���V�F�[�_�[���͍\����
struct PS_INPUT
{
    // ���W
    float4 position : SV_POSITION;
    // uv���W
    float2 texcoord : TEXCOORD0;
    // �@��
    float3 normal : TEXCOORD1;
};
