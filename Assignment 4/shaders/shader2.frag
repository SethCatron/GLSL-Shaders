#version 120
// color is passed from the vertex program to the fragment program
varying vec3 vPositionModel; // in object space - position of each vertex -- defined in init main
varying vec3 vNormalModel; // in object space - normal of each vertex-- defined in init main
varying vec3 worldPosition, worldNormal;
uniform mat4 model, view, projection, inverseTranspose;
uniform vec3 eye;
struct lightStruct
{
	vec3 position;
	vec3 color;
};

#define NUM_LIGHTS 2

uniform lightStruct lights[NUM_LIGHTS];

// parameters required to calculate the color of each pixel or vertex based on Phong shading model
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;

void main()
{

	vec3 c1, c2, I, li0, li1, r1, r2, E;
	float l1n, l2n, r1e, r2e;
	vec3 color;

	E = normalize((eye - worldPosition));  // homogenizing E into a vec 3 for calculations below 
	li0 = normalize((lights[0].position - worldPosition));
	li1 = normalize((lights[1].position - worldPosition));
// dot products
	// calculate L • N
	l1n = dot(li0, worldNormal);
	l2n = dot(li1, worldNormal);

	// calculate r - reflection values
	r1 = normalize((2 * l1n * worldNormal) - li0); 
	r2 = normalize((2 * l2n * worldNormal) - li1); 
	r1e = dot(r1, E);
	r2e = dot(r2, E);
// calculate each term in the sum
	c1 = (kd * max(0, l1n)) + (ks * pow(max(0, r1e), s));
	c2 = (kd * max(0, l2n)) + (ks * pow(max(0, r2e), s));
// plug everything into I
	I = ka + ((lights[0].color) * c1) + ((lights[1].color) * c2);
	color = I;
	
	// gl_FragColor is the predefined output of a fragment program; 4 elements of the vec4 are red, green, blue, and alpha(opaqueness)
	gl_FragColor = vec4(color, 1.0f);
}
