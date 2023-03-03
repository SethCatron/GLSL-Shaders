#version 120

attribute vec3 vPositionModel; // in object space - position of each vertex -- defined in init main
attribute vec3 vNormalModel; // in object space - normal of each vertex-- defined in init main
varying vec3 worldPosition, worldNormal;
uniform mat4 model, view, projection, inverseTranspose;
uniform vec3 eye;

void main()
{
	worldPosition = vec3(model * vec4(vPositionModel, 1.0));
	worldNormal = normalize(vec3(inverseTranspose * vec4(vNormalModel, 1.0)));
	gl_Position = projection * view * model * vec4(vPositionModel, 1.0);  // add 1.0 to the end of each to take to homogenous coordinate
}