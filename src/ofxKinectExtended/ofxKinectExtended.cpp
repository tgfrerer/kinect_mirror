/*
 *  ofxKinectExtended.cpp
 *  ofxKinect
 *
 *  Created by Timothy Gfrerer on 02/12/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxKinectExtended.h"
#include "ofMain.h"

ofxKinectExtended::ofxKinectExtended(){
    // constructor
  rgbaPixels = NULL;
  distancePixelsRGBA  = NULL;

};

void ofxKinectExtended::init(){
  int length = width*height;

  ofxKinect::init();
  rgbaPixels = new unsigned char[length*4];

  
  // depthPixelsBack = new unsigned short[length];
  distancePixelsRGBA = new unsigned char[length *4];
	
  memset(rgbaPixels, 0, length*4*sizeof(unsigned char));
  // memset(depthPixelsBack, 0, length*sizeof(unsigned short));
  rgbDepthMatrix=ofxKinect::getRGBDepthMatrix();

};

void ofxKinectExtended::clear(){
  delete[] rgbaPixels; rgbaPixels = NULL;
  // delete[] depthPixelsBack; depthPixelsBack = NULL;
  
  ofxKinect::clear();
};

ofxKinectExtended::~ofxKinectExtended(){
  // destructor
  close();
  clear();
};


unsigned char * ofxKinectExtended::getRGBAPixels(){
	ofxVec3f texcoord3d;
	unsigned char * rgbaP = rgbaPixels;
	for ( int y = 0; y < height; y++) {
		for ( int x = 0; x < width; x++) {
			texcoord3d.set(x,y,0);
			texcoord3d = rgbDepthMatrix * texcoord3d ;
			texcoord3d.x = ofClamp(texcoord3d.x,0,640);
			texcoord3d.y = ofClamp(texcoord3d.y,0,480);
			int pos = int(texcoord3d.y)*640+int(texcoord3d.x);
			*rgbaP++ = rgbPixels[pos*3];
			*rgbaP++ = rgbPixels[pos*3+1];
			*rgbaP++ = rgbPixels[pos*3+2];
      *rgbaP++ = depthPixels[y*640+x];
		}
	}
	return rgbaPixels;
};

unsigned short ofxKinectExtended::getDepthPixelBackAt(int x, int y) {
	return depthPixelsBack[y * width + x];
}

unsigned char* ofxKinectExtended::getDistancePixelsRGBA() {
  typedef unsigned char u_char;
  u_char * pDepthRGBA = distancePixelsRGBA;
  
  int length = 640*480;
  
  for (int i=0; i<length; i++) {
    int distance = int((2048.f / float(ofxKinect::depthPixelsBack[i])) * 2048.f);
    
    // (tmpDistance == 2047) ? tmpDistance = 0 : tmpDistance -= 2048;
    *pDepthRGBA++ = distance / 256;   
    *pDepthRGBA++ = distance % 256;   
    *pDepthRGBA++ = 0xff;
    *pDepthRGBA++ = 0xff;
  }
  return distancePixelsRGBA;
};


