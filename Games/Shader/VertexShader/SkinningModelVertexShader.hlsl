// ヘッダファイルの読み込み =================================================
#include "../Header/SkinningModel.hlsli"


// 頂点シェーダーの定義 =====================================================
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;

    float4 localPosition = float4(input.position, 1.0f);

    float4x4 skinTransform = float4x4(1.0f, 0.0f, 0.0f, 0.0f,
		                               0.0f, 1.0f, 0.0f, 0.0f,
									   0.0f, 0.0f, 1.0f, 0.0f,
									   0.0f, 0.0f, 0.0f, 1.0f);

	// スキニング処理
    skinTransform += Bones[input.skinIndex.x] * input.skinWeight.x;
    skinTransform += Bones[input.skinIndex.y] * input.skinWeight.y;
    skinTransform += Bones[input.skinIndex.z] * input.skinWeight.z;
    skinTransform += Bones[input.skinIndex.w] * input.skinWeight.w;

    // 座標変換
    output.position = mul(skinTransform, localPosition);
    output.position = mul(output.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);

	// 法線ベクトルを変換
    float3 transNormal = mul((float3x3)skinTransform, input.normal);
    output.normal = mul((float3x3) world, transNormal);

	// uv
    output.texcoord = input.texcoord;

    // 出力する
    return output;
}
