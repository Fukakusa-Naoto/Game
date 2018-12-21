// 頂点シェーダーへの入力構造体
struct VS_INPUT
{
    // 座標
    float3 position : POSITION;
    // uv座標
    float2 texcoord : TEXCOORD;
    // 色
    float4 color : COLOR;
};



// ピクセルシェーダーへの入力構造体
struct PS_INPUT
{
    // 座標
    float4 position : SV_POSITION;
    // uv座標
    float2 texcoord : TEXCOORD;
    // 色
    float4 color : COLOR;
};
