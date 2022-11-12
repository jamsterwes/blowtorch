#version 330

in vec2 uv;
uniform sampler2D image;

out vec4 out_color;

const float smoothEP = 0.001;

float wave(float t, float period) {
    float ramp = mod(t * period, 1.0) * 6.28;
    return 1.0 - (cos(ramp) + 1.0) * 0.5;
}

float wave2(float t, float period) {
    float ramp = mod(t * period, 1.0) * 6.28;
    return 1.0 - (pow(cos(ramp), 2) + 1.0) * 0.5;
}

float band(float t, float bands) {
    float large = round(t * bands);
    return large / bands;
}

float smoothBand(float t, float bands) {
    t = clamp(t, 0, 1);
    return (band(t - smoothEP, bands) + band(t, bands) + band(t + smoothEP, bands)) / 3;
}

vec4 lumShuffle(vec4 col) {
    float lum = dot(col.rgb, vec3(0.2126, 0.7152, 0.0722));
    vec4 outCol = col;
    outCol.r = mix(col.r, 1.0 - col.g, pow(lum, 0.1));
    outCol.g = mix(col.g, 1.0 - col.b, pow(lum, 0.1));
    outCol.b = mix(col.b, 1.0 - col.r, pow(lum, 0.1));
    return outCol;
}

void main() {
    vec4 col = texture2D(image, uv);

    vec4 col2 = vec4(pow(col.r, 0.5), pow(col.g, 0.5), pow(col.b, 0.5), 1.0);
    vec4 col0 = col2;

    // col2.r = 1 - wave2(col0.r, 2.5);
    col2.b = wave2(col0.g, 2.5);

    col2.r = wave2(1.0 - smoothBand(col2.b, 4), 1.4);
    col2.g = wave2(col0.r, 2.5);

    col2 = mix(col, col2, 0.5);
    col2 *= col.a;

    out_color = vec4(col2.rgb, 1.0);
}