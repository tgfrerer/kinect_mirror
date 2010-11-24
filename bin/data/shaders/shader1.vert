#version 120

uniform sampler2D colourMap;

uniform float depthScaling;
varying vec4 color;

//const float depthScaling = 255. * 2.5;

void main() {

   gl_TexCoord[0] = gl_MultiTexCoord0;

   vec4 pos = gl_Vertex;
   color = texture2D(colourMap, gl_MultiTexCoord0.xy);
   
   pos.z = (color.a) * depthScaling;
   // if (color.a <= 0.5){ pos.z=0};
   
   gl_Position = gl_ModelViewProjectionMatrix * pos;
}