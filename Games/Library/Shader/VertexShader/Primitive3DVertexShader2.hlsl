// �w�b�_�t�@�C���̓ǂݍ��� =================================================
#include "../Header/Primitive3D.hlsli"


// ���_�V�F�[�_�[�̒�` =====================================================
PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	output.position = float4(input.position, 1.0f);
	output.position = mul(output.position, world);
	output.position = mul(output.position, view);

	output.color = input.color;

	output.normal = float3(0.0f, 0.0f, 0.0f);
	return output;
}