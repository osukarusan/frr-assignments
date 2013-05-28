uniform vec2 rotationRepetitions;

void main() 
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_TexCoord[1] = vec4(gl_MultiTexCoord0.xy * rotationRepetitions, 0.0, 0.0);
}
