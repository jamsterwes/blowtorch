#version 330

in vec2 uv;
uniform sampler2D image;

out vec4 out_color;

void main() {
    vec4 col = texture2D(image, uv);
    col *= col.a;
    
    vec4 col_orig = col;
    col.r = mod(col.r * 2, 1.0);
    col.g = mod(col.r * 3.2, 1.0);
    col.b = mod(col.r * 4.4, 1.0);
    col = mix(col, col_orig, 0.5);
    out_color = vec4(col.rgb, 1.0);
}