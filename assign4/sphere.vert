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


attribute vec4 vertex_position;		// the vertex position (in the local space) from VBO
attribute vec4 vertex_normal;		// the vertex normal (in the local space) from VBO
attribute vec2 texCoord;			// coordiantes of the texture

varying vec3 fragment_position;		// The fragment position output into the fragment shader
varying vec4 fragment_normal;		// The fragment normal output into the fragment shader
varying vec2 texCoord1;
uniform int t;
varying float time;

uniform mat4 modelViewProjMat;


// directional wave along a line
float height(float x, float z)
{

	float height ,d;
	vec2 dir;
	vec2 dir1;
	vec2 p, q;
	float dz, dx;
	float waveLength = 0.1;
	float amplitude = 7.0;
	float frequency = 0.01;
	float waveLength1 = 0.2;
	float amplitude1 = 0.8;
	float frequency1 = 0.08;

	// direction
	dir.x = 1.0;
	dir.y = 0.0;
	dir = normalize(dir);

	dir1.x = 1.0;
	dir1.y = 1.0;
	dir1 = normalize(dir1);

	// reference point
	p.x = 0;
	p.y = 0;

	// create a vector from the points
	q.x = x;
	q.y = z;

	q = q-p;

	d = dot(dir,q);
	height = amplitude*sin(waveLength*d + frequency*t+100);

	d = dot(dir1,q);

	// secondary function
	//	height += amplitude1*sin(waveLength1*d + frequency1*t+100);
	//vin.pos.y = amp * sin(k*d+omega*time + shift);
	//height = z*sin(0.1*x) + x*cos(0.1*z);

	return(height);
}



// circular waves (ripple effect)
float height1(float x, float z)
{

	float height ,d;
	vec2 point;
	float dz, dx;
	float waveLength = 0.15;
	float amplitude = 7;
	float frequency = 0.01;
	float displacement = 50;

	point.x = -2;
	point.y = 0;
	dz = z - point.y;
	dx = x - point.x;
	d = sqrt(dz*dz + dx*dx);
	height = amplitude*sin(waveLength*d+frequency*t+displacement);
	//vin.pos.y = amp * sin(k*d+omega*time + shift);
	//height = z*sin(0.1*x) + x*cos(0.1*z);
	return(height);
}

void main()
{

	vec4 vtx = vertex_position;
	//vtx.y += height(vtx.x, vtx.z);

	fragment_position = vec3(vtx);
	fragment_normal   = vertex_normal;

	gl_Position = modelViewProjMat * vtx;
	fragment_position = vec3(gl_Position);

	fragment_normal   = modelViewProjMat * vertex_normal;
	//gl_Position.y = height(gl_Position.x, gl_Position.z);
	texCoord1 = texCoord;
	time = t;

}
