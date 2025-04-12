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

	vec2 z_prime = vec2(0.);
	vec2 z_prime_last = vec2(0.);
	vec2 z_last = coords; 

	vec2 z_prime_sum = vec2(0.);

	float n = 0.;
	
	while(n < limit)
	{
		z_prime = 2.* z_prime_last*z_last + 1.;
		z_prime_last = z_prime;
		z_last = mult_compl(z_last, z_last) + coords;

		z_prime_sum += z_prime;

		if(z_prime_sum.x*z_prime_sum.x + z_prime_sum.y*z_prime_sum.y > dbail)
			return n;
		n++;
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
	//if(iters == u_max_iter) color = vec3(0.);
	//else
	{
		color = vec3(1.);
		color.r = log(iters)/log2(u_max_iter); 
		color.g = cos(iters/u_zoom)/cos(log(u_max_iter)); 
		color.b = log(2.+cos(iters/5.));
	}
	out_color = vec4(color, 1.);
}
