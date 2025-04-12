#version 410 core

#extension GL_ARB_gpu_shader_fp64 : require

precision highp float;

in vec2 vs_uv;
out vec4 out_color;

uniform bool u_quarter_res;
uniform bool u_double_precision;

uniform float u_time;
uniform float u_aspect_ratio;
uniform float u_zoom;
uniform float u_max_iter;
uniform float u_max_dist;

uniform vec2 u_center_x;
uniform vec2 u_center_y;
uniform vec2 u_center;
uniform vec2 u_z;
uniform vec2 u_resolution;

dvec2 centered_zoom(dvec2 uv, dvec2 zoom, dvec2 center)
{
	dvec2 new_uv = uv - 0.5;
	
	new_uv /= zoom;
	new_uv += center;

	return new_uv;
}

dvec2 mult_compl(dvec2 a, dvec2 b)
{
	return dvec2(
			a.x*b.x - a.y*b.y,
			a.x*b.y + a.y*b.x
			);
}

float mand_iter(dvec2 coords)
{
	float iter = 0.;
	dvec2 z = u_z;

	while(
			distance(z, vec2(0.)) < 2. &&
			iter < u_max_iter
	){
		z = mult_compl(z, z) + coords;

		iter++;
	}

	return iter;
}

void main()
{
	dvec2 center;

	if(u_double_precision)
	{
		center.x = u_center_x.y * pow(10., u_center_x.x);
		center.y = u_center_y.y * pow(10., u_center_y.x);
	}
	else
		center = u_center;

	dvec2 uv = centered_zoom(
			vs_uv,
			dvec2(u_zoom, u_zoom*u_aspect_ratio)/2.,
			center);

	float iters = mand_iter(uv);
	dvec3 color = dvec3(iters/u_max_iter);
	
	out_color = vec4(color, 1.);
}
