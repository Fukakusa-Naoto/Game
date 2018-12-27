// ヘッダファイルの読み込み =================================================
#include "../Header/Model.hlsli"


// グローバル変数の定義 =====================================================
// テクスチャ
Texture2D tex : register(t0);
// サンプラー
SamplerState samplerState : register(s0);


// ピクセルシェーダーの定義 =================================================
float4 main(PS_INPUT input) : SV_TARGET
{
    // テクスチャの取得
    float4 textureData = tex.Sample(samplerState, input.texcoord);

    // テクスチャがない
    if ((textureData.x == 0.0f) && (textureData.y == 0.0f) && (textureData.z == 0.0f) && (textureData.w == 0.0f))
        return float4(1.0f, 1.0f, 1.0f, 1.0f);

    // 合成して出力する
    return textureData;
}
