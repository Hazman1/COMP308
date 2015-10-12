//---------------------------------------------------------------------------
//
// Copyright (c) 2015 Taehyun Rhee, Joshua Scott, Ben Allen
//
// This software is provided 'as-is' for assignment of COMP308 in ECS,
// Victoria University of Wellington, without any express or implied warranty. 
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
//----------------------------------------------------------------------------

#version 120

// Constant across both shaders
//uniform sampler2D texture0;

// Values passed in from the vertex shader
//varying vec3 vNormal;
//varying vec3 vPosition;
//varying vec2 vTextureCoord0;

//void main() {
	
	
//	gl_FragColor = texture2D(texture0, vTextureCoord0);
//}
uniform sampler2D texture0;
varying vec3 N;
varying vec3 v;   
varying vec2 vTextureCoord0; 
void main (void)  
{  
	vec4 startTexture = texture2D(texture0, vTextureCoord0);

   vec3 L = normalize(gl_LightSource[1].position.xyz - v);   
   vec3 E = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)  
   vec3 R = normalize(-reflect(L,N));  
 
   //calculate Ambient Term:  
   vec4 Iamb = gl_FrontLightProduct[0].ambient;    

   //calculate Diffuse Term:  
   vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);
   Idiff = clamp(Idiff, 0.0, 1.0);     
   
   // calculate Specular Term:
   vec4 Ispec = gl_FrontLightProduct[0].specular 
                * pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);
   Ispec = clamp(Ispec, 0.0, 1.0); 
   // write Total Color:  
   gl_FragColor = startTexture*gl_FrontLightModelProduct.sceneColor + Iamb + Idiff + Ispec;     
}
  