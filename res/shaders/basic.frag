#version 460

in vec2 v_uv;
in vec3 v_normal;

uniform sampler2D u_texture;
uniform vec3 u_lightDir;

out vec4 frag_color;

const float AMBIENT = 0.25;

void main() {
    vec3 n = normalize(v_normal);
    float diffuse = max(dot(n, u_lightDir), 0.0);
    vec3 base = texture(u_texture, v_uv).rgb;
    frag_color = vec4(base * (AMBIENT + (1.0 - AMBIENT) * diffuse), 1.0);
}
