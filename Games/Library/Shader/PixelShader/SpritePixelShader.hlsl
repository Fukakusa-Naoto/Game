// ヘッダファイルの読み込み =================================================
#include "../Header/SpriteHeader.hlsli"

// グローバル変数の定義 =====================================================
// テクスチャ
Texture2D tex : register(t0);
// サンプラー
SamplerState samplerState : register(s0);


// ピクセルシェーダーの定義 =================================================
float4 main(PS_INPUT input) : SV_TARGET
{
    // テクスチャの取得する
    float4 texData = tex.Sample(samplerState, input.texcoord);

    // 加算合成
    return input.color * texData;
}
