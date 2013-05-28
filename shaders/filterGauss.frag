uniform sampler2D texture;
uniform vec2      texelSize;
uniform float     radius;

/*
void main(void)
{
    vec4 color = vec4(0.0);
    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            vec2 offset = radius*vec2(texelSize.x * float(i), texelSize.y*float(j));
            color += texture2D(texture, gl_TexCoord[0].xy + offset);
        }
    }
    gl_FragColor = color/25.0;
}
*/

void main(void) {
    vec4 color = vec4(0.0);
    color += 0.0030*texture2D(texture, gl_TexCoord[0].xy + radius*texelSize*vec2(-2.0, -2.0));
    color += 0.0133*texture2D(texture, gl_TexCoord[0].xy + radius*texelSize*vec2(-2.0, -1.0));
    color += 0.0219*texture2D(texture, gl_TexCoord[0].xy + radius*texelSize*vec2(-2.0,  0.0));
    color += 0.0133*texture2D(texture, gl_TexCoord[0].xy + radius*texelSize*vec2(-2.0,  1.0));
    color += 0.0030*texture2D(texture, gl_TexCoord[0].xy + radius*texelSize*vec2(-2.0,  2.0));
    color += 0.0133*texture2D(texture, gl_TexCoord[0].xy + radius*texelSize*vec2(-1.0, -2.0));
    color += 0.0596*texture2D(texture, gl_TexCoord[0].xy + radius*texelSize*vec2(-1.0, -1.0));
    color += 0.0983*texture2D(texture, gl_TexCoord[0].xy + radius*texelSize*vec2(-1.0,  0.0));
    color += 0.0596*texture2D(texture, gl_TexCoord[0].xy + radius*texelSize*vec2(-1.0,  1.0));
    color += 0.0133*texture2D(texture, gl_TexCoord[0].xy + radius*texelSize*vec2(-1.0,  2.0));
    color += 0.0219*texture2D(texture, gl_TexCoord[0].xy + radius*texelSize*vec2( 0.0, -2.0));
    color += 0.0983*texture2D(texture, gl_TexCoord[0].xy + radius*texelSize*vec2( 0.0, -1.0));
    color += 0.1621*texture2D(texture, gl_TexCoord[0].xy + radius*texelSize*vec2( 0.0,  0.0));
    color += 0.0983*texture2D(texture, gl_TexCoord[0].xy + radius*texelSize*vec2( 0.0,  1.0));
    color += 0.0219*texture2D(texture, gl_TexCoord[0].xy + radius*texelSize*vec2( 0.0,  2.0));
    color += 0.0133*texture2D(texture, gl_TexCoord[0].xy + radius*texelSize*vec2( 1.0, -2.0));
    color += 0.0596*texture2D(texture, gl_TexCoord[0].xy + radius*texelSize*vec2( 1.0, -1.0));
    color += 0.0983*texture2D(texture, gl_TexCoord[0].xy + radius*texelSize*vec2( 1.0,  0.0));
    color += 0.0596*texture2D(texture, gl_TexCoord[0].xy + radius*texelSize*vec2( 1.0,  1.0));
    color += 0.0133*texture2D(texture, gl_TexCoord[0].xy + radius*texelSize*vec2( 1.0,  2.0));
    color += 0.0030*texture2D(texture, gl_TexCoord[0].xy + radius*texelSize*vec2( 2.0, -2.0));
    color += 0.0133*texture2D(texture, gl_TexCoord[0].xy + radius*texelSize*vec2( 2.0, -1.0));
    color += 0.0219*texture2D(texture, gl_TexCoord[0].xy + radius*texelSize*vec2( 2.0,  0.0));
    color += 0.0133*texture2D(texture, gl_TexCoord[0].xy + radius*texelSize*vec2( 2.0,  1.0));
    color += 0.0030*texture2D(texture, gl_TexCoord[0].xy + radius*texelSize*vec2( 2.0,  2.0));
    gl_FragColor = color;
}

