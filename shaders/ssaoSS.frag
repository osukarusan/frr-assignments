#version 120

const int NUMSAMPLES = 16;
const int TEX_DEFERRED = 0;
const int TEX_ROTATION = 1;
const float PI = 3.14159265;
const float EPSILON = 0.01;

uniform sampler2D normalsDepth;
uniform sampler2D rotationPattern;
uniform vec3      samplingPattern[NUMSAMPLES];

uniform vec2      texelSize;
uniform float     radius;

void main(void)
{
    vec2  texpos = gl_TexCoord[TEX_DEFERRED].xy;
    float depth  = texture2D(normalsDepth, texpos).a;

    float angle     = 2.0*PI*texture2D(rotationPattern, gl_TexCoord[TEX_ROTATION].xy).r;
    float sina      = sin(angle);
    float cosa      = cos(angle);
    mat2x2 rotation = mat2x2(cosa, -sina, cosa, sina);

    float accum = 0.0;
    if (depth > 0.0) {
        for (int i = 0; i < NUMSAMPLES; i++) {
            vec2 offset = samplingPattern[i].xy;
            offset = rotation*offset;
            offset = radius*texelSize*offset;

            float sampleDepth = texture2D(normalsDepth, texpos + offset).a;
            if (sampleDepth >= depth - EPSILON) {
                accum += 1.0;
            }
        }
        accum /= float(NUMSAMPLES);
    }
    else {
        accum = 0.0;
    }

    gl_FragColor = vec4(accum, accum, accum, 1.0);



}
