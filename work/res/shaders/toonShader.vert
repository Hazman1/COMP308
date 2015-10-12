#version 120
// Values to pass to the fragment shader

varying vec3 vNormal;
varying vec3 vPosition;
varying vec3 vLightDir;

void main() {
	vNormal = normalize(gl_NormalMatrix * gl_Normal);
	vPosition = vec3(gl_ModelViewMatrix * gl_Vertex);
	vLightDir = normalize(vec3(gl_LightSource[0].position));
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
// or, use the fixed function version to keep the vert in place
//gl_Position = ftransform();
}