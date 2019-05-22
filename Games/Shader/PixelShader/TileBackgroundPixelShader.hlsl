// ヘッダファイルの読み込み =================================================
#include "../Header/TileBackground.hlsli"


// 構造体の定義 ===========================================================
// 定数バッファの構造体
cbuffer TimeBuffer : register(b0)
{
    // 時間
    float4 time;
};


// グローバル変数の定義 =====================================================
// テクスチャ
Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
// サンプラー
SamplerState samplerState : register(s0);


// ピクセルシェーダーの定義 =================================================
float4 main(PS_INPUT input) : SV_TARGET
{
    // テクスチャの取得する
    float4 texData = tex.Sample(samplerState, input.texcoord);

    float2 uv = input.texcoord;

    // 透過部分を増やす
    uv.y *= 1.5f;

    // スクロールさせる
    uv.y -= time.x * 0.1f;

    // 計算後のuvでルール画像をサンプリング
    float4 texData2 = tex2.Sample(samplerState, uv);

    // 加算合成
    float4 color = input.color * texData * texData2;

    // 全体を薄く透過する
    color.a *= abs(sin(time.x * 0.5f)) + 0.5f;

    // 出力する
    return color;
}
