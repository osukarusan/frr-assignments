uniform sampler2D texture;
uniform sampler2D normalsDepth;
uniform vec2      texelSize;
uniform float     radius;

const float PI = 3.14159265;
const float GS = 10.0;

float Gaussian2D(float x, float y) {
    return 1.0/sqrt(2.0*PI*GS)*exp(-(x*x + y*y)/(2.0*GS));
}

float DepthFun(float x) {
    return 1.0 - sqrt(x);
}

void main(void) {

    vec4  color  = vec4(0.0);
    float wsum   = 0.0;
    vec2  center = gl_TexCoord[0].xy;
    float cdepth = texture2D(normalsDepth, center).a;

    float dwsum = 0.0;
    float dwcnt = 0.0;

    for (float i = -radius; i <= radius; i += 1.0) {
        for (float j = -radius; j <= radius; j += 1.0) {
            vec2  psample = gl_TexCoord[0].xy + texelSize*vec2(i, j);
            float gweight = Gaussian2D(i, j);
            float dweight = DepthFun(abs(cdepth - texture2D(normalsDepth, psample).a));
            wsum  += gweight*dweight;
            color += gweight*dweight*texture2D(texture, psample);

            dwsum += dweight;
            dwcnt += 1.0;
        }
    }

    gl_FragColor = color/wsum;

}
