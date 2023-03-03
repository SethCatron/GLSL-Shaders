#version 120
// color is passed from the vertex program to the fragment program
varying vec3 color;

void main()
{
	
	// gl_FragColor is the predefined output of a fragment program; 4 elements of the vec4 are red, green, blue, and alpha(opaqueness)
	gl_FragColor = vec4(color, 1.0f);
}
