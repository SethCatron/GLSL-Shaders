#version 120
// color is passed from the vertex program to the fragment program
varying vec3 worldPosition, worldNormal;
uniform vec3 eye;

void main()
{
	vec3 newcolor, E;
	float dotcalc, lengthWN, lengthE, calc;
	E = normalize(eye - worldPosition);
	dotcalc = dot(worldNormal, E);
	lengthWN = length(worldNormal);
	lengthE = length(E);
	calc = (dotcalc) / (lengthWN * lengthE);

	// angle calculation for silhouette
	if ((calc > 0.3) || (calc < -0.3)) {
		newcolor = vec3(0.0f, 0.0f, 0.0f);
	} else {
		newcolor = vec3(1.0f, 1.0f, 1.0f);
	}
		
	// gl_FragColor is the predefined output of a fragment program; 4 elements of the vec4 are red, green, blue, and alpha(opaqueness)
	gl_FragColor = vec4(newcolor, 1.0f);

}
