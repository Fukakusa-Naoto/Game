// ヘッダファイルの読み込み =================================================
#include "../Header/Model.hlsli"


// 構造体の定義 ===========================================================
// 定数バッファの構造体
cbuffer ConstBuffer : register(b0)
{
    // ワールド行列
    Matrix world;
    // ビュー行列
    Matrix view;
    // 射影行列
    Matrix projection;
};


// 頂点シェーダーの定義 ====================================================
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    // 座標変換
    output.position = mul(float4(input.position, 1.0f), world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);

    // uv座標
    output.texcoord = input.texcoord;

    return output;
}