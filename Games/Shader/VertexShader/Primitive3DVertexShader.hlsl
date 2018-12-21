// ヘッダファイルの読み込み =================================================
#include "../Header/Primitive3D.hlsli"


// 頂点シェーダーの定義 =====================================================
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    // 座標変換
    output.position = mul(float4(input.position, 1.0f), world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);

    // 色
    output.color = input.color;

    return output;
}