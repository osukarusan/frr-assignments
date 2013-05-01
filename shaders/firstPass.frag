varying vec3 normal;

void main(void)
{
    gl_FragColor = vec4(normal.x, normal.y, normal.z, gl_FragCoord.z);
}
