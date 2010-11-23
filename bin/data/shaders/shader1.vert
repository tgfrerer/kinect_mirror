#version 120

uniform sampler2D colourMap;

uniform float depthScaling;
varying vec4 color;

//const float depthScaling = 255. * 2.5;

void main() {
  
   color = texture2D(colourMap, gl_MultiTexCoord0.xy);
   vec4 pos = gl_Vertex;
   pos.z += (color.a) * depthScaling;
   gl_Position = gl_ModelViewProjectionMatrix * pos;
}
