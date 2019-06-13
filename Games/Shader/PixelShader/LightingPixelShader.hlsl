struct PS_INPUT
{
	// ���W
	float4 position : SV_POSITION;
	// �F
	float4 color : COLOR;
	// �@���x�N�g��
	float3 normal : NORMAL;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	// �������Ƀ��C�g�𓖂Ă�
	float3 light = float3(0.0f, -1.0f, 0.0f);

	float4 color= input.color * saturate(dot(input.normal, light));

	// ���C�g���������Ă��Ȃ����̐F��t����
	color += float4(input.color.rgb * 0.5f, input.color.a);
	return color;
}