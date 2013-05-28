#version 120

const int NUMSAMPLES = 16;
const int TEX_DEFERRED = 0;
const int TEX_ROTATION = 1;
const int TEX_RAYDIR   = 2;
const float PI = 3.14159265;

uniform sampler2D normalsDepth;
uniform sampler2D rotationPattern;
uniform vec3      samplingPattern[NUMSAMPLES];

uniform mat4      projectionMatrix;
uniform vec2      texelSize;
uniform float     radius;


void main(void)
{
    vec2  texpos = gl_TexCoord[TEX_DEFERRED].xy;
    vec4  texdef = texture2D(normalsDepth, texpos);
    float depth  = texdef.a;
    vec3  normal = 2.0*texdef.rgb - 1.0;
    vec3  esPos = depth*gl_TexCoord[TEX_RAYDIR].xyz;

    float angle    = 0.0;//2.0*PI*texture2D(rotationPattern, gl_TexCoord[TEX_ROTATION].xy).r;
    vec3  vrot     = vec3(cos(angle), sin(angle), 0.0);
    vec3 tangent   = normalize(vrot - normal*dot(vrot, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 tbnMatrix = mat3(tangent, bitangent, normal);
    tbnMatrix = mat3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);

    float accum = 0.0;
    if (depth > 0.0) {
        for (int i = 0; i < NUMSAMPLES; i++) {
            // get point from sampling pattern rotated
            vec3 point = samplingPattern[i];
            point = radius*tbnMatrix*point + esPos;

            // project point to screen space
            vec4 offset = vec4(point, 1.0);
            offset = projectionMatrix * offset;
            vec2 sspos = (offset.xy / offset.w)*0.5 + 0.5;

            // get depth and compare
            float sampleDepth  = texture2D(normalsDepth, sspos).a;
            float insideRadius = abs(esPos.z - sampleDepth) < radius ? 1.0 : 1.0;
            accum += insideRadius * (sampleDepth >= point.z ? 1.0 : 1.0);
        }
        accum /= float(NUMSAMPLES);
    }
    else {
        accum = 0.0;
    }

    gl_FragColor = vec4(accum, accum, accum, 1.0);
}
