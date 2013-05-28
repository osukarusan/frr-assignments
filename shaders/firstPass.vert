uniform float znear;
uniform float zfar;

varying vec3 normal;
varying float zdist;

void main() 
{
    vec4 eyepos = gl_ModelViewMatrix * gl_Vertex;
    zdist = - eyepos.z / zfar;
    gl_Position = gl_ProjectionMatrix * eyepos;
    normal = gl_NormalMatrix * gl_Normal;
}
