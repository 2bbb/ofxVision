#version 410

uniform sampler2DRect base;
uniform sampler2DRect tex0;
out vec4 outputColor;

void main()
{
    int x = int(gl_FragCoord.x);
    int y = int(gl_FragCoord.y);
    ivec2 size = textureSize(tex0);
    vec4 optical = texelFetch(tex0, ivec2(x, 720 - y));
    /*
    optical = vec4(0);
    for(int i = -3; i <= 3; ++i) {
	    for(int j = -3; j <= 3; ++j) {
	    	optical += texelFetch(tex0, ivec2(x + i, 720 - y + j));
	    }
    }
    optical = optical / 49.0;
    */
    vec2 coord = gl_FragCoord.xy;
    coord.y = 720.0 - coord.y;
    vec4 c = texture(base, coord + optical.rg * 3.0);
    optical.a = 0.5;
    c.a = abs(optical.r * optical.g);
	outputColor = c;
}