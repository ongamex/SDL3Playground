
struct Vertex {
    float x;
    float y;
    float z;
    float padding;
};

StructuredBuffer<Vertex> vertices;

struct VOut
    float4 position : SV_Position;
    float3 fragColor : My_VertexColor;
}

struct VIn {
    uint vidx : SV_VertexID;
};

[shader("vertex")]
VOut main() {
    float3 colors[3] = float3[](
        float3(1.0, 0.0, 0.0),
        float3(0.0, 1.0, 0.0),
        float3(0.0, 0.0, 1.0)
    );


    VOut res;
    res.position = float4(vertices.positions[vertexIdx].x, vertices.positions[vertexIdx].y, 0.f, 0.f);
    res.fragColor = colors[vertexIdx];

    return res;
}