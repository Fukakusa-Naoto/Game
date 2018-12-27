// 定数の定義 =============================================================
// ボーンの最大数
#define MAX_BONES (48)


// グローバル変数の定義 ====================================================
// テクスチャ
Texture2D tex : register(t0);
// サンプラー
SamplerState samplerState : register(s0);

// 定数バッファ
cbuffer ConstBuffer : register(b0)
{
    // ワールド行列
    Matrix world;
    // ビュー行列
    Matrix view;
    // 射影行列
    Matrix projection;
    // ライト
    float4 light;
};


// スキニングバッファ
cbuffer SkinningBuffer : register(b1)
{
    float4x4 Bones[MAX_BONES];
};


// 構造体の定義 ===========================================================
// 頂点シェーダー入力構造体
struct VS_INPUT
{
    // 座標
    float3 position : POSITION;
    // 法線
    float3 normal : NORMAL;
    // uv座標
    float2 texcoord : TEXCOORD;
    // ボーンインデックス
    uint4 skinIndex : SKIN_INDEX;
    // ウェイト
    float4 skinWeight : SKIN_WEIGHT;
};


// ピクセルシェーダー入力構造体
struct PS_INPUT
{
    // 座標
    float4 position : SV_POSITION;
    // uv座標
    float2 texcoord : TEXCOORD0;
    // 法線
    float3 normal : TEXCOORD1;
};
