#version 330 core


in vec3 interpColor;

out vec3 fragColor;

vec3 hsv2rgb(vec3 hsv) {
    float H = hsv[0] * 6;
    float C = hsv[2] * hsv[1];
    float X = C * (1 - abs(mod(H, 2) - 1));
    vec3 rgb;
    switch (int(H) % 6) {
        case 0:
            rgb = vec3( C, X, 0);
            // rgb = vec3( 1, 2, 3);
        break;
        case 1:
            rgb = vec3( X, C, 0);
        break;
        case 2:
            rgb = vec3( 0, C, X);
        break;
        case 3:
            rgb = vec3( 0, X, C);
        break;
        case 4:
            rgb = vec3( X, 0, C);
        break;
        case 5:
            rgb = vec3( C, 0, X);
        break;
    }
    vec3 m = vec3(hsv[2]-C, hsv[2]-C, hsv[2]-C);
    rgb = rgb + m;
    return rgb;
}

void main() {
    fragColor = hsv2rgb(interpColor);
}
