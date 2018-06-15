
# HELLO:)

This is my raytracer with Soft Shadow implemented. 
It has straitfied random sampling using a 3x3 grid and allows the light to be a given radius.

my website html file is in the bgalvanb folder

Flags 
   -softs : turns on soft shadows
   -softs=N : N determines the number of samples to be used. the minimum is 16 no matter what the input. Because I liked how 16 looks
   -radius=N : N is the radius of the lights in the scene 
   
My .pov files
   rainBO.pov
   BOx.pov
   //capitilization matters sorry....
   

Rather than talk about what I did right, I'd rather talk about what I did wrong and how 
I learned from that. When I first started this I'm not sure why but I decided creating my own 
vec3's and methods for them would be alright for this project. But I found out fairly fast using GLM 
would have definitely been the way to go. I plan to rewrite my raytracer over summer (hopefully!) using GLM 
to keep my code shorter and much more legible. I do believe over all my recursive method for getting
the color was well written tho.
