varying vec3 normal;
varying float zdist;

void main(void)
{
    gl_FragColor = vec4(0.5*(normal + 1.0), zdist);
}
