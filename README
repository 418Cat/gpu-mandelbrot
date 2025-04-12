# GPU Mandelbrot
The mandelbrot fractal is really interesting, and also very simple to implement so it's a nice starter project
<br>
![Mandelbrot](https://raw.githubusercontent.com/418Cat/gpu-mandelbrot/refs/heads/main/images/mandelbrot.png)
<br>
___
It's a learning project for dear-imgui, opengl and c++,
with the goal of implementing various mandelbrot algorithms, and julia sets in the future.
<br>
<br>
Robert Munafo's https://www.mrob.com/pub/muency.html blog is full of tips and information on
implementing mandelbrot algorithms and was of great help.
___
## Pictures
### Traditional iterated algorithm:
This algorithm computes the time  needed for a complex number $z$ to escape to infinity with: <br>
$z_0 = 0$ <br>
$z = z^2 + c$ <br>
Here, $c$ is the coordinates of the point in the complex plane, aka the pixel on screen. <br><br>
Any point $z$ with $|z| > 2$ will escape to infinity, so the condition for the main loop looks
something like this: `while(mag(z) <= 2 && iter < max_iter)`  <br>
![Iterated](https://raw.githubusercontent.com/418Cat/gpu-mandelbrot/refs/heads/main/images/iterated.png)


## Derivative bailout
This algorithm is less known, by me too as it was my first time implementing it.
It checks if the sum of the derivatives is  bigger than a given value, in which case it stops.<br>
This algorithm has the benefit of showing some structures inside the bulbs<br>
According to wikipedia, the derivative $z^\prime_n$ at $z_n$, with $n$  the iteration number, is given by: <br>
$z^\prime_n = 2*z^\prime_{n-1}*z_{n-1}+1$ <br><br>
Wikipedia also gave a python sample which I translated in glsl for my first attempt at implementing it in fs_derivative.glsl: <br>
![Derivative 1](https://raw.githubusercontent.com/418Cat/gpu-mandelbrot/refs/heads/main/images/derivative_1.png)
<br><br>


My second attempt in fs_derivative_2.glsl used the definition, and with a few tweaks for the coloring, seems to give satisfying results: <br>
![Derivative 1](https://raw.githubusercontent.com/418Cat/gpu-mandelbrot/refs/heads/main/images/derivative_2.png)
<br><br>


## Continuous coloring
Because the iteration count is an integer, at low iteration counts,
some color bands are very visible and does not natively allow for smooth
transitions between one iteration to the other. <br><br>
From Robert Munafo's website, after computing the final iteration count $n$, a smooth gradient can be obtained by
adding some biais $b \in [0, 1]$ to $n$, with: <br>
$b = 1+log_2(log_2(r))-log_2(log_2(|z_n|))$ <br>
$r$ being the bailout radius, and $z_n$ the final iterated number at time of bailout.
<br><br>
The final results are a nice smooth gradient: <br>
![Continuous](https://raw.githubusercontent.com/418Cat/gpu-mandelbrot/refs/heads/main/images/continuous_coloring.png)

<br><br><br>
___
This code is under the MIT License