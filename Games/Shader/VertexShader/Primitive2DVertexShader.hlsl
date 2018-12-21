#include "../Header/Primitive2D.hlsli"

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    output.position = float4(input.position, 1.0f);
    output.color = input.color;
    return output;
}