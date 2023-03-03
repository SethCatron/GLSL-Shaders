#version 120

attribute vec3 vPositionModel; // in object space - position of each vertex -- defined in init main
attribute vec3 vNormalModel; // in object space - normal of each vertex-- defined in init main

uniform mat4 model, view, projection, inverseTranspose;
uniform vec3 eye;
varying vec3 worldPosition, worldNormal;
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
	worldPosition = vec3(model * vec4(vPositionModel, 1.0));
	worldNormal = normalize(vec3(inverseTranspose * vec4(vNormalModel, 1.0)));
	gl_Position = projection * view * model * vec4(vPositionModel, 1.0);  // add 1.0 to the end of each to take to homogenous coordinate
}