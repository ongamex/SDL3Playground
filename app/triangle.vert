#version 450

layout(location = 0) out vec3 fragColor;

vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

vec3 colors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

struct Vertex {
    float x;
    float y;
    float z;
    float padding;
};

layout(std140, set = 0, binding = 0) readonly buffer Vertices {
	Vertex positions[];
} vertices;

void main() {
    vec2 pt = vec2(vertices.positions[gl_VertexIndex].x, vertices.positions[gl_VertexIndex].y);
    gl_Position = vec4(pt, 0.0, 1.0);
    fragColor = colors[gl_VertexIndex];
}