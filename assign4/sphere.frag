#version 120

//
//Author: Doron Nussbaum (C) 2014 All Rights Reserved.
//-----------------------------------------------------
//
//
// License
//--------------
//
// Code can be used for instructional and educational purposes.
// Usage of code for other purposes is not allowed without a given permission by the author.
//
//
// Disclaimer
//--------------
//
// The code is provided as is without any warranty

varying vec3 fragment_position;		// The fragment position interpolated from the vertex shader
varying vec4 fragment_normal;		// The fragment normal interpolated from the vertex shader
varying vec2 texCoord1;

varying float time;

 uniform sampler2D tex;
 vec2 tc;

void main()
{


	gl_FragColor = vec4(1.0,1.0,0,0);
	tc = texCoord1;

	// in case one would like to scroll the surface
	//tc.x += time/500;
	//tc.y += time/1000;
	gl_FragColor = texture2D(tex, tc);
 }
