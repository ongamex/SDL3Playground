
[shader("fragment")]
float4 main(float3 fragColor : My_VertexColor): SV_Target {
    return float4(fragColor.rgb, 1.0);
}