uniform sampler2D texture;

void main(void)
{
    gl_FragColor = vec4(texture2D(texture, gl_TexCoord[0].xy).aaa, 1.0);
}
