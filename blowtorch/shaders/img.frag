#version 330

in vec2 uv;
uniform sampler2D image;

out vec4 out_color;

const float smoothEP = 0.001;

float wave(float t, float period) {
    float ramp = mod(t * period, 1.0) * 6.28;
    return 1.0 - (cos(ramp) + 1.0) * 0.5;
}

float band(float t, float bands) {
    float large = round(t * bands);
    return large / bands;
}

float smoothBand(float t, float bands) {
    t = clamp(t, 0, 1);
    return (band(t - smoothEP, bands) + band(t, bands) + band(t + smoothEP, bands)) / 3;
}

void main() {
    vec4 col = texture2D(image, uv);
    col *= col.a;

    vec4 col_orig = col;

    vec4 col2 = col;
    col2.r = wave(col.g, 2.2);
    col2.g = wave(col.b, 2.2);
    col2.b = wave(col.r, 2.2);

    vec4 col3 = col2;
    col3.b = smoothBand(col2.r, 5);
    col3.r = smoothBand(col2.g, 10);
    col3.g = smoothBand(col2.b, 3);

    col3 = mix(col, col3, 0.6);

    out_color = vec4(col3.rgb, 1.0);
}