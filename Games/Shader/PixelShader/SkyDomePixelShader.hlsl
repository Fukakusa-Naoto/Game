#include "../Header/SkyDomeHeader.hlsli"


// �O���[�o���ϐ��̒�` =====================================================
// �e�N�X�`��
Texture2D tex : register(t0);
// �T���v���[
SamplerState samLinear : register(s0);


float4 main(PS_INPUT input) : SV_TARGET
{
    return tex.Sample(samLinear, input.Tex);
}