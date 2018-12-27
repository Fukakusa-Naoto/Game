// 定数バッファ
cbuffer ConstBuffer : register(b0)
{
    Matrix world;
    Matrix view;
    Matrix projection;
};


// 頂点シェーダーへの入力構造体
struct VS_INPUT
{
    // 座標
    float3 position : POSITION;
	// 色
	float4 color : COLOR;
};


// ピクセルシェーダーへの入力構造体
struct PS_INPUT
{
    // 座標
    float4 position : SV_POSITION;
	// 色
	float4 color : COLOR;
	// 法線ベクトル
	float3 normal : NORMAL;
};