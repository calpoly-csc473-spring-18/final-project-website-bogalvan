camera {
  location  <0, 0, 14>
  up        <0,  1,  0>
  right     <1.33333, 0,  0>
  look_at   <0, 0, 0>
}

light_source {<-70, 100, 100> color rgb <1.5, 1.5, 1.5>}
light_source {<70, 100, 100> color rgb <1.5, 1.5, 1.5>}



sphere { <-5, 0, 0>, 1.5 //red sphere
  pigment { color rgbf <.87, .06 , .13, 0.3>}
  finish {ambient 0.2 diffuse 0.4  refraction 1.0 ior 1.6}
  translate <0, 0, 0>
}

sphere { <5, 0, 0>, 1.5 //green sphere
  pigment { color rgbf <.25, .88 , .06 , 0.3>}
  finish {ambient 0.2 diffuse 0.4  refraction 1.0 ior 1.6}
  translate <0, 0, 0>
}

sphere { <-2, 1.5, -2.5>, 1.5 //orange sphere
  pigment { color rgbf <.88, .47 , .06 , 0.3>}
  finish {ambient 0.2 diffuse 0.4  refraction 1.0 ior 1.6}
  translate <0, 0, 0>
}

sphere { <2, 1.5, -2.5>, 1.5 //yellow sphere
  pigment { color rgbf <.88, .85 , .06 , 0.3>}
  finish {ambient 0.2 diffuse 0.4  refraction 1.0 ior 1.6}
  translate <0, 0, 0>
}

sphere { <2, -1.5, 2.5>, 1.5 //blue sphere
  pigment { color rgbf <.06, .87 , .88 , 0.3>}
  finish {ambient 0.2 diffuse 0.4  refraction 1.0 ior 1.6}
  translate <0, 0, 0>
}

sphere { <-2, -1.5, 2.5>, 1.5 //purple sphere
  pigment { color rgbf <.44, .06 , .88 , 0.3>}
  finish {ambient 0.2 diffuse 0.4  refraction 1.0 ior 1.6}
  translate <0, 0, 0>
}



plane {<0, 1, 0>, -4
      pigment {color rgb <0.2, 0.2, 0.8>}
      finish {ambient 0.4 diffuse 0.8}
}
