#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxShader.h"


class testApp : public ofBaseApp
{

	public:

		void setup();
		void update();
		void draw();
    void exit();

		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

    GLint render_mode;
    int render_steps;
  
		ofxKinect kinect;
    
    ofxShader shader;

		ofxCvColorImage		colorImg;
  
		//ofxCvGrayscaleImage 	grayImage;
    ofImage grayImage;
  
    ofImage testImage;
		ofxCvGrayscaleImage 	grayBg;
		ofxCvGrayscaleImage 	grayDiff;

    float rotX, rotY;

		ofImage			calibratedTex;
  
    bool do_shader;
  
    int current_matrix_index;
  
};

#endif
