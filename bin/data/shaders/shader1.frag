#version 120

//void main (void)  
//{  
//   gl_FragColor = gl_Color;    
//}

varying vec4 color;

void main(void) {
  // if(color.a <= 0.5) discard;
  // gl_FragColor = gl_Color;    
  gl_FragColor = vec4(color.xyz, 1.0);
}