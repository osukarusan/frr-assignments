#version 120

const int NUMSAMPLES = 16;
const int TEX_DEFERRED = 0;
const int TEX_ROTATION = 1;
const int TEX_RAYDIR   = 2;
const float PI = 3.14159265;
const float EPSILON = 0.005;

uniform sampler2D normalsDepth;
uniform sampler2D rotationPattern;
uniform vec3      samplingPattern[NUMSAMPLES];

uniform mat4      projectionMatrix;
uniform vec2      texelSize;
uniform float     radius;
uniform float     zfar;

void main(void)
{
    vec2  texpos   = gl_TexCoord[TEX_DEFERRED].xy;
    vec4  texdef   = texture2D(normalsDepth, texpos);
    float depth    = texdef.a;
    vec3  normal   = 2.0*texdef.rgb - 1.0;
    vec3  esCenter = depth*gl_TexCoord[TEX_RAYDIR].xyz;

    float angle    = 2.0*PI*texture2D(rotationPattern, gl_TexCoord[TEX_ROTATION].xy).r;
    vec3  vrot     = vec3(cos(angle), sin(angle), 0.0);
    vec3 tangent   = normalize(vrot - normal*dot(vrot, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 tbnMatrix = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;
    if (depth < 1.0) {
        for (int i = 0; i < NUMSAMPLES; i++) {
            // get point from sampling pattern rotated
            vec3 esSample = samplingPattern[i];
            esSample = radius*tbnMatrix*esSample + esCenter;

            // project point to screen space
            vec4 cspos = projectionMatrix * vec4(esSample.xy, -esSample.z, 1.0);
            vec2 sspos = (cspos.xy / cspos.w)*0.5 + 0.5;

            // get depth and compare
            float ssdepth      = zfar*texture2D(normalsDepth, sspos).a;
            float insideRadius = abs(esCenter.z - ssdepth) < radius ? 1.0 : 0.0;

            // consider occluders only inside the radius
            occlusion += insideRadius * (ssdepth < esSample.z - EPSILON*zfar ? 1.0 : 0.0);
        }
        occlusion /= float(NUMSAMPLES);
    }
    else {
        occlusion = 1.0;
    }

    gl_FragColor = vec4(1.0 - vec3(occlusion), 1.0);
}
