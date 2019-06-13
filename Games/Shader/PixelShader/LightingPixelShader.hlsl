struct PS_INPUT
{
	// 座標
	float4 position : SV_POSITION;
	// 色
	float4 color : COLOR;
	// 法線ベクトル
	float3 normal : NORMAL;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	// 下向きにライトを当てる
	float3 light = float3(0.0f, -1.0f, 0.0f);

	float4 color= input.color * saturate(dot(input.normal, light));

	// ライトが当たっていない分の色を付ける
	color += float4(input.color.rgb * 0.5f, input.color.a);
	return color;
}