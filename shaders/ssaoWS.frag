#version 120

const int NUMSAMPLES = 16;
const int TEX_DEFERRED = 0;
const int TEX_ROTATION = 1;
const int TEX_RAYDIR   = 2;
const float PI = 3.14159265;
const float EPSILON = 0.01  ;

uniform sampler2D normalsDepth;
uniform sampler2D rotationPattern;
uniform vec3      samplingPattern[NUMSAMPLES];

uniform mat4      projectionMatrix;
uniform vec2      texelSize;
uniform float     radius;
uniform float     fadeoff;
uniform float     zfar;

void main(void)
{
    vec2  texpos   = gl_TexCoord[TEX_DEFERRED].xy;
    vec4  texdef   = texture2D(normalsDepth, texpos);
    float depth    = texdef.a;
    float occlusion = 0.0;

    if (depth < 1.0) {

        vec3  normal    = normalize(2.0*texdef.rgb - 1.0);
        vec3  esCenter  = depth*gl_TexCoord[TEX_RAYDIR].xyz;
        vec3  vrot      = normalize(texture2D(rotationPattern, gl_TexCoord[TEX_ROTATION].xy).rgb);
        vec3  tangent   = normalize(vrot - normal*dot(vrot, normal));
        vec3  bitangent = cross(normal, tangent);
        mat3  tbnMatrix = mat3(tangent, bitangent, normal);

        for (int i = 0; i < NUMSAMPLES; i++) {
            // get point from sampling pattern rotated
            vec3 esSample  = tbnMatrix*samplingPattern[i];
            esSample = radius*tbnMatrix*esSample + esCenter;

            // project point to screen space
            vec4 cspos = projectionMatrix * vec4(esSample.xy, -esSample.z, 1.0);
            vec2 sspos = (cspos.xy / cspos.w)*0.5 + 0.5;

            // get depth and compare, consider occluders only inside the radius and attenuate with distance
            float ssdepth   = zfar*texture2D(normalsDepth, sspos).a;
            float dd = (esSample.z - ssdepth - zfar*EPSILON)/(fadeoff*radius);
            occlusion += (dd > 0.0 ? max(1.0 - 0.01*dd*dd, 0.0) : 0.0);
            //float insideRadius = abs(esCenter.z - ssdepth) < radius + zfar*EPSILON ? 1.0 : 0.0;
            //occlusion += insideRadius * (ssdepth < esSample.z - zfar*EPSILON ? 1.0 : 0.0);
        }
        occlusion /= float(NUMSAMPLES);
    }
    else {
        occlusion = 1.0;
    }

    gl_FragColor = vec4(1.0 - vec3(occlusion), 1.0);

}
