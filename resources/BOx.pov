// csc 473 more interesting box

camera {
  location  <0, 0, 14>
  up        <0,  1,  0>
  right     <1.5, 0,  0>
  look_at   <0, 0, 0>
}

light_source {<10, 10, 3> color rgb <1.5, 1.5, 1.5>}


// Floor
plane {<0, 1, 0>, -4
  pigment {color rgb <0.6, 0.3, 0.6>}
  finish {ambient 0.4 diffuse 1.0}
}

// Background
plane {<0, 0, 1>, -100
  pigment {color rgb <0.17, 0.6, 0.5>}
  finish {ambient 0.2 diffuse 0.4}
}

//reflective 
box {<-2, -5, -4.5>, <2, 5, 4.5>
  pigment { color rgb <1.0, 1.0, 1.0>}
  finish {ambient 0.2 diffuse 0.8 reflection 0.8}
  rotate <0, -45, 0>
  translate <-7, 0, -5>
}


box {<-1, -2, -1>, <1, 2, 1> 
  pigment { color rgb <0.2, 0.6, 0.8>}
  finish {ambient 0.2 diffuse 0.8}
  translate <0,0,0>
  rotate <0,45,0>
}
box {<-.5, -1, -1>, <.5,1, 1>
  pigment { color rgb <0.2, 0.6, 0.8>}
  finish {ambient 0.2 diffuse 0.8}
   rotate <15,-10,90>
  translate <2,0,2>
}
box {<-1, -2, -1>, <1, 2, 1> 
  pigment { color rgb <0.2, 0.6, 0.8>}
  finish {ambient 0.2 diffuse 0.8}
  rotate <0,45,0>
  translate <3,0,2>
}

box {<-1, -2, -1>, <1, 2, 1> 
  pigment { color rgb <0.2, 0.6, 0.8>}
  finish {ambient 0.2 diffuse 0.8}
  rotate <0,45,0>
  translate <6,0,4>
}