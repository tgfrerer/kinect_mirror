#ifndef GUARD_OFXKINEXTEXTENDED
#define GUARD_OFXKINEXTEXTENDED
/*
 *  ofxKinectExtended.h
 *  ofxKinect
 *
 *  Created by Timothy Gfrerer on 02/12/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxKinect.h"

class ofxKinectExtended : public ofxKinect {

public:
  ofxKinectExtended();
  ~ofxKinectExtended();

  void clear();
  void init();

  unsigned char *			rgbaPixels;
  unsigned char * distancePixelsRGBA;

  // get the rgb pixels with the depth frame as the alpha channel
  unsigned char * getRGBAPixels();
  
  unsigned char* getDistancePixelsRGBA();
  unsigned short getDepthPixelBackAt(int x, int y);

protected:
  // unsigned short *	depthPixelsBack;	// depth back
  ofxMatrix4x4		rgbDepthMatrix;

};





#endif