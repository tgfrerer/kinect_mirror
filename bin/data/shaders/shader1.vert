#version 120

uniform sampler2D depthMap;
uniform sampler2D videoMap;
// uniform float depthScaling;

varying vec4 color;
varying float depthZ;
//const float depthScaling = 255. * 2.5;

void main() {

   vec4 depth;

   gl_TexCoord[0] = gl_MultiTexCoord0;

   vec4 pos = gl_Vertex;

   depth = texture2D(depthMap, gl_MultiTexCoord0.xy);
   
   color = texture2D(depthMap, gl_MultiTexCoord0.xy);
   color.x=255;
   
   // color = texture2D(videoMap, gl_MultiTexCoord0.xy);

   pos.z = ((depth.x *256 ) + (depth.y)) * 20;
   depthZ = pos.z;
  
   // if (color.a <= 0.5){ pos.z=0};
   

   
   gl_Position = gl_ModelViewProjectionMatrix * pos;
}
