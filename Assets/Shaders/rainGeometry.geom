#version 330 core
layout(points) in;
layout(line_strip, max_vertices = 2) out;

in VS_OUT {
    vec3 position;
} gs_in[];

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    vec4 worldPosition = model * vec4(gs_in[0].position, 1.0);
    gl_Position = projection * view * worldPosition;
    EmitVertex();

    vec4 endPosition = worldPosition + vec4(0.0, -100.0, 0.0, 0.0);
    gl_Position = projection * view * endPosition;
    EmitVertex();

    EndPrimitive();
}