struct VertexInput
{
    float3 inPosition;
    float3 inNormal;
    float2 inUV;
};

struct VertexOutput
{
    float4 position : SV_Position;
    float3 normal;
    float2 uv;
};

[vk::push_constant]
ConstantBuffer<float4x4> model_matrix;


[shader("vertex")]
VertexOutput vert_main(VertexInput input)
{
    VertexOutput output;
    output.position = mul(model_matrix, float4(input.inPosition, 1.0));
    output.normal = input.inNormal;
    output.uv = input.inUV;

    return output;
}


[shader("fragment")]
float4 frag_main(VertexOutput in_vert) : SV_Target
{
    float3 color = in_vert.normal;
    return float4(color, 1.0);
}
