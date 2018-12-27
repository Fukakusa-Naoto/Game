// ヘッダファイルの読み込み =================================================
#include "../Header/SpriteHeader.hlsli"

// 頂点シェーダーの定義 =====================================================
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    output.position = float4(input.position, 1.0f);
    output.texcoord = input.texcoord;
    output.color = input.color;
    return output;
}