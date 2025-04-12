#version 410 core

in vec2 vs_uv;
out vec4 out_color;

uniform float u_aspect_ratio;

uniform float u_zoom;
uniform vec2 u_center;

uniform float u_bailout_radius;
uniform float u_max_iter;
uniform vec2 u_z;

vec2 centered_zoom(vec2 uv, vec2 zoom, vec2 center)
{
	vec2 new_uv = uv - 0.5;
	
	new_uv /= zoom;
	new_uv += center;

	return new_uv;
}

vec2 mult_compl(vec2 a, vec2 b)
{
	return vec2(
			a.x*b.x - a.y*b.y,
			a.x*b.y + a.y*b.x
			);
}

float mand_iter(vec2 coords)
{
	float iter = 0.;
	vec2 z = u_z;

	while(
			distance(z, vec2(0.)) < u_bailout_radius &&
			iter < u_max_iter
			)
	{
		z = mult_compl(z, z) + coords;

		iter++;
	}

	iter += 1. +
		log2(log2(u_bailout_radius)) -
		log2(log2(distance(z, vec2(0.))));

	return iter;
}

void main()
{
	vec2 uv = centered_zoom(
			vs_uv,
			vec2(u_zoom, u_zoom*u_aspect_ratio)/2.,
			u_center);

	float iters = mand_iter(uv);

	
	vec3 color;
	if(iters >= u_max_iter) color = vec3(0.);
	else
	{
		/*
		   Gamma correction, reducing color banding where
		   it's especially noticeable with continuous
		   coloring
		*/
		//color = vec3(pow(iters/u_max_iter, 1./2.2));
		color.r = cos(log(iters)/log(u_max_iter) + log2(iters));
		//color.g = log(iters)/u_max_iter;
		color.g = (cos((iters)/8.)+1.)*0.6; 
		color.b = sin((iters/log(iters))/5.)*.5 + 7.; 
		//color.b = sin(log2(iters)/(log(u_max_iter)*3.))*.5 + 1.; 
	}
	out_color = vec4(color, 1.);
}
