uniform sampler2D texture;
uniform vec2      texelSize;
uniform float     radius;

const float PI = 3.14159265;
const float GS = 10.0;


float Gaussian2D(float x, float y) {
    return 1.0/sqrt(2.0*PI*GS)*exp(-(x*x + y*y)/(2.0*GS));
}

void main(void) {

    vec4 color  = vec4(0.0);
    float wsum  = 0.0;
    vec2 center = gl_TexCoord[0].xy + 0.5*texelSize;

    for (float i = -radius; i <= radius; i += 1.0) {
        for (float j = -radius; j <= radius; j += 1.0) {
            vec2  psample = gl_TexCoord[0].xy + texelSize*vec2(i, j);
            float gweight = Gaussian2D(i, j);
            wsum  += gweight;
            color += gweight*texture2D(texture, psample);
        }
    }

    gl_FragColor = color/wsum;
}
