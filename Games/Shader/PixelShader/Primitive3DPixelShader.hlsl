// ヘッダファイルの読み込み =================================================
#include "../Header/Primitive3D.hlsli"


// ピクセルシェーダーの定義 =================================================
float4 main(PS_INPUT input) : SV_TARGET
{
    // 色を出力する
    return input.color;
}