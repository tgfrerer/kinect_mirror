#version 120

//void main (void)  
//{  
//   gl_FragColor = gl_Color;    
//}

varying vec4 color;
varying float depthZ;

void main(void) {
  // if(depthZ < 500 || depthZ > 800) discard;
  // gl_FragColor = gl_Color;    
  gl_FragColor = vec4(color.xyz, 1.0);
}