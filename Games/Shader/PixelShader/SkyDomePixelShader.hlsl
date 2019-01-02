#include "../Header/SkyDomeHeader.hlsli"


// グローバル変数の定義 =====================================================
// テクスチャ
Texture2D tex : register(t0);
// サンプラー
SamplerState samLinear : register(s0);


float4 main(PS_INPUT input) : SV_TARGET
{
    return tex.Sample(samLinear, input.Tex);
}