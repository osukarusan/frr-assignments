uniform sampler2D texture;

void main(void)
{
    float depth = texture2D(texture, gl_TexCoord[0].xy).a;
    vec3 esPos  = depth * gl_TexCoord[1].xyz;

    gl_FragColor = vec4(esPos, 1.0);
    //gl_FragColor = vec4(texture2D(texture, gl_TexCoord[0].xy).aaa, 1.0);
    //gl_FragColor = vec4(2.0*(texture2D(texture, gl_TexCoord[0].xy).rgb) - 1.0, 1.0);
}
