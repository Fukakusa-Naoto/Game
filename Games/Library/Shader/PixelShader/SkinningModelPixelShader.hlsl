// ヘッダファイルの読み込み =================================================
#include "../Header/SkinningModel.hlsli"


// ピクセルシェーダーの定義 =================================================
float4 main(PS_INPUT input) : SV_TARGET
{
    float4 output = 0;

    // テクスチャの取得
    float4 textureData = tex.Sample(samplerState, input.texcoord);

    // テクスチャがない
    if ((textureData.x == 0.0f) && (textureData.y == 0.0f) && (textureData.z == 0.0f) && (textureData.w == 0.0f))
        return float4(1.0f, 1.0f, 1.0f, 1.0f);
    else
        return textureData;
}
