#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup()
{
	kinect.init();
	kinect.setVerbose(false);
	kinect.open();
  do_shader = false;

	colorImg.allocate(kinect.width, kinect.height);
	// grayImage.allocate(kinect.width, kinect.height);
	grayBg.allocate(kinect.width, kinect.height);
	grayDiff.allocate(kinect.width, kinect.height);

	calibratedTex.allocate(kinect.width, kinect.height,GL_RGB);
  
  testImage.allocate(kinect.width, kinect.height, GL_RGB);

	ofSetFrameRate(30);
  
  ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(50, 50, 50);
	ofEnableAlphaBlending();
	
	ofSetVerticalSync(false);
  
  ofDisableArbTex();
  ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);
  
  grayImage.loadImage("displacement.png");
  // solid.loadImage("solid.png");
  calibratedTex.loadImage("solid.png");
	
	shader.setup("shaders/shader1.vert", "shaders/shader1.frag"); 
	
  glEnable(GL_DEPTH_TEST);
  
  rotX = 0; rotY = 0;
  current_matrix_index = 0;
  
}

//--------------------------------------------------------------
void testApp::update()
{
	ofBackground(100, 100, 100);
	kinect.update();


  testImage.setFromPixels(kinect.getRGBAPixels(), kinect.width, kinect.height,OF_IMAGE_COLOR_ALPHA,true);
  
  
  // grayImage.setFromPixels(kinect.getRGBAPixels(), kinect.width, kinect.height, GL_RGBA, true);
  //calibratedTex.loadData(kinect.getCalibratedRGBPixels(),640,480,GL_RGB);
  // calibratedTex.setFromPixels(kinect.getCalibratedRGBPixels(),kinect.width,kinect.height,GL_RGB);
  
	//rgbaMixture.allocate(w,h,GL_RGBA);
	// rgbaMixture.loadData(pixels, w,h,GL_RGBA);

  //grayImage.setFromPixels(pixels, kinect.width, kinect.height, GL_RGBA, true);
	
  
  
}

//--------------------------------------------------------------
void testApp::draw()
{


  //  kinect.drawDepth(10, 10, 400, 300);
  //  kinect.drawDepth(360, 10);
  //  grayImage.draw(360,20);
	//  grayBg.draw(20,280, 320, 180);
  //  grayDiff.draw(450, 350, 400, 300);

  

	ofSetColor(0xffffff);
  //	contourFinder.draw(450, 350, 400, 300);
	ofSetColor(0xffffff);
	//  calibratedTex.draw(10,350,400,300);


   

// ******** ********** 
  if (do_shader) {
  
    ofPushMatrix();
    
    shader.begin();
      
      
    float w = grayImage.getWidth();
    float h = grayImage.getHeight();
    
    glTranslatef(ofGetWidth() / 2, ofGetHeight() / 2, 0);
    rotX = ofLerp(mouseX, rotX, 0.5);
    rotY = ofLerp(mouseY, rotY, 0.5);
    glRotatef(rotX, 0, 1, 0);
    glRotatef(-rotY, 1, 0, 0);
    glTranslatef(-w / 2, -h / 2, -320);
    
    grayImage.getTextureReference().bind();
    shader.setUniformTexture("colourMap", testImage, 0);

    shader.setUniform1f("depthScaling", 320.f * 2);
    
    shader.setUniform3f("lightDir", sin(ofGetElapsedTimef()), cos(ofGetElapsedTimef()), 0);
    
    int step = 1;

    glColor3f(1, 1,1);
    glBegin(GL_POINTS);
    for(int y = 0; y < h; y += step) {
      for(int x = 0; x < w; x += step) {
        ofPoint texCoords;
        
        texCoords = grayImage.getTextureReference().getCoordFromPoint(x, y);
        glTexCoord2f(texCoords.x, texCoords.y);
        glVertex2f(x, y);
        
        texCoords = grayImage.getTextureReference().getCoordFromPoint(x, y + step);
        glTexCoord2f(texCoords.x, texCoords.y);
        glVertex2f(x, y + step);
        
      }
    }
    glEnd();
    
    glColor3f(0.3, 0.3, 0.3);
    
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2, 0);
    ofRotateX(mouseY);
    ofRotateY(mouseX);
    
    shader.end();
    
    ofPopMatrix();
  } else {
    // non-3d View:

    ofxMatrix4x4 mx = kinect.getRGBDepthMatrix();

    
    
    glColor3f(1, 1, 1);
    char matrixStr[1024];
    sprintf(matrixStr, "current matrix index: %d|%d", current_matrix_index%4, current_matrix_index/4);
    ofDrawBitmapString(matrixStr, 20, 650);
    glColor3f(1, 0, 0);
    ofNoFill();
    ofRect(20+(current_matrix_index%4)*105, 658+(current_matrix_index/4)*20, 105, 15);
    ofFill();
    glColor3f(1, 1, 1);
    for (int i=0;i<4; i++) {
      sprintf(matrixStr, "%12.6f %12.6f %12.6f %12.6f", mx.getPtr()[i*4], mx.getPtr()[i*4+1], mx.getPtr()[i*4+2],mx.getPtr()[i*4+3]);
      ofDrawBitmapString(matrixStr, 20, 670 + i*20);
    }
    
    glColor3f(1, 1, 1);
    testImage.draw(10,10,640,480);
    kinect.drawDepth(10+320+10,10);
  }

	
  glColor3f(1, 1, 1);

  ofSetColor(0xffffff);
	char reportStr[1024];

	sprintf(reportStr, "fps: %f", ofGetFrameRate());
	ofDrawBitmapString(reportStr, 20, 640);
  

}

//--------------------------------------------------------------
void testApp::exit()
{
	kinect.close();
}

//--------------------------------------------------------------
void testApp::keyPressed (int key)
{
  ofxMatrix4x4 mx = kinect.getRGBDepthMatrix();

  
  switch (key)
	{
		case ' ':
			do_shader ^= true;
			break;
    case 'f':
        ofToggleFullscreen();
      break;
		case '+': current_matrix_index = (current_matrix_index+1) % 16;
			break;
		case '-': current_matrix_index = (current_matrix_index + 15) % 16;
			break;

    case '0':
      mx.getPtr()[current_matrix_index]+=.00001f;
      break;
    case '=':
      mx.getPtr()[current_matrix_index]-=.00001f;
      break;
    case '1':
      mx.getPtr()[current_matrix_index]+=.01f;
    break;
    case '!':
      mx.getPtr()[current_matrix_index]-=.01f;
      break;
    case '2':
      mx.getPtr()[current_matrix_index]+=.1f;
      break;
    case '"':
      mx.getPtr()[current_matrix_index]-=.1f;
      break;
    case '3':
      mx.getPtr()[current_matrix_index]+=1.f;
      break;
    case '¤':
      mx.getPtr()[current_matrix_index]-=1.f;
      break;
    case '4':
      mx.getPtr()[current_matrix_index]+=10.f;
      break;
    case '$':
      mx.getPtr()[current_matrix_index]-=10.f;
      break;
      
      
	}
  
  kinect.setRGBDepthMatrix(mx);
  
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{}

