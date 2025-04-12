#version 410 core

in vec2 vs_uv;
out vec4 out_color;

uniform float u_aspect_ratio;

uniform float u_zoom;
uniform vec2 u_center;

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
			distance(z, vec2(0.)) < 2. &&
			iter < u_max_iter
			)
	{
		z = mult_compl(z, z) + coords;

		iter++;
	}

	return iter;
}

float mand_der(vec2 coords, float limit)
{
    float dbail = 1.e6;

    vec2 c = coords;
    vec2 dc = u_z;
	vec2 dc_sum = vec2(0.);

	float n = 0.;
	
	while(n < limit)
	{
		c = mult_compl(c, c) + coords;
		dc = 2.* mult_compl(dc, c) + 1.;
        dc_sum += dc;

		n++;

		if(dc_sum.x*dc_sum.x + dc_sum.y*dc_sum.y > dbail)
			return n;
	}
	return 0.;
}

void main()
{
	vec2 uv = centered_zoom(
			vs_uv,
			vec2(u_zoom, u_zoom*u_aspect_ratio)/2.,
			u_center);

	//float iters = mand_iter(uv);
	float iters = mand_der(uv, u_max_iter);
	
	vec3 color;
	if(iters == u_max_iter) color = vec3(0.);
	else
	{
		color = vec3(1.);
		color.r = iters/u_max_iter; 
		color.g = log(iters)/log(u_max_iter);
		color.b = log(iters);
	}
	out_color = vec4(color, 1.);
}
