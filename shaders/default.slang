struct VertexInput
{
    float3 inPosition;
    float3 inNormal;
    float2 inUV;
};

struct VertexOutput
{
    float3 color;
    float2 uv;
    float4 sv_position : SV_Position;
};

[shader("vertex")]
VertexOutput vert_main(VertexInput input)
{
    VertexOutput output;
    output.sv_position = float4(input.inPosition, 1.0);
    output.color = input.inNormal;
    output.uv = input.inUV;

    return output;
}

[shader("fragment")]
float4 frag_main(VertexOutput in_vert) : SV_Target
{
    float3 color = in_vert.color;
    return float4(color, 1.0);
}
