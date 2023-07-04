#version 410

uniform sampler2DRect base;
uniform sampler2DRect tex0;
uniform vec2 size;
out vec4 outputColor;

void main()
{
    int x = int(gl_FragCoord.x);
    int y = int(gl_FragCoord.y);
    vec2 seg_size = textureSize(tex0);
    ivec2 seg_pos = ivec2(
        int(seg_size.x * gl_FragCoord.x / size.x),
        int(seg_size.y) - int(seg_size.y * gl_FragCoord.y / size.y)
    );
    vec4 segmentation = texelFetch(tex0, seg_pos);
    vec2 coord = gl_FragCoord.xy;
    coord.y = int(size.y) - coord.y;
    vec4 c = texture(base, coord);
	outputColor = c * (1.0 - segmentation.r), 0.5);
//    outputColor = c;
}