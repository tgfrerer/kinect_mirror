#version 120

uniform sampler2D depthMap;

// uniform float depthScaling;

varying vec4 color;

//const float depthScaling = 255. * 2.5;

void main() {

   vec4 depth;

   gl_TexCoord[0] = gl_MultiTexCoord0;

   vec4 pos = gl_Vertex;

   depth = texture2D(depthMap, gl_MultiTexCoord0.xy);
   color = texture2D(depthMap, gl_MultiTexCoord0.xy);

   pos.z = ((depth.x *256 ) + (depth.y)) * 20;
  
   // if (color.a <= 0.5){ pos.z=0};
   

   
   gl_Position = gl_ModelViewProjectionMatrix * pos;
}
