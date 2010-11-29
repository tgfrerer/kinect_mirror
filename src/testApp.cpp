#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup()
{
	kinect.init();
	kinect.setVerbose(false);
	kinect.open();
  do_shader = true;

	colorImg.allocate(kinect.width, kinect.height);
	// videoImage.allocate(kinect.width, kinect.height);
	grayBg.allocate(kinect.width, kinect.height);
	grayDiff.allocate(kinect.width, kinect.height);

	calibratedTex.allocate(kinect.width, kinect.height,GL_RGB);
  
  videoImage.allocate(kinect.width, kinect.height, GL_RGB);
  depthImage.allocate(kinect.width, kinect.height, GL_RGBA);
  
	// ofSetFrameRate(30);
  
  ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(0, 0, 0);
	ofEnableAlphaBlending();
	
	ofSetVerticalSync(false);
  
  ofDisableArbTex();
  ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);
  
  // videoImage.loadImage("displacement.png");
  // solid.loadImage("solid.png");
  calibratedTex.loadImage("solid.png");
	
	shader.setup("shaders/shader1.vert", "shaders/shader1.frag"); 
	
  render_mode = GL_FILL;
  
  glEnable(GL_DEPTH_TEST);
  
  rotX = rotY = 0;
  current_matrix_index = 0;
  render_steps = 20;
  
}

//--------------------------------------------------------------
void testApp::update()
{
	kinect.update();

  videoImage.setFromPixels(kinect.getPixels(), kinect.width, kinect.height,OF_IMAGE_COLOR,true);
  depthImage.setFromPixels(kinect.getDistancePixelsRGBA(),kinect.width, kinect.height,OF_IMAGE_COLOR_ALPHA,true);
  
  // videoImage.setFromPixels(kinect.getRGBAPixels(), kinect.width, kinect.height, GL_RGBA, true);
  //calibratedTex.loadData(kinect.getCalibratedRGBPixels(),640,480,GL_RGB);
  // calibratedTex.setFromPixels(kinect.getCalibratedRGBPixels(),kinect.width,kinect.height,GL_RGB);
  
	//rgbaMixture.allocate(w,h,GL_RGBA);
	// rgbaMixture.loadData(pixels, w,h,GL_RGBA);

  //videoImage.setFromPixels(pixels, kinect.width, kinect.height, GL_RGBA, true);
	
  
  
}

//--------------------------------------------------------------
void testApp::draw()
{

  //  kinect.drawDepth(10, 10, 400, 300);
  //  kinect.drawDepth(360, 10);
  //  videoImage.draw(360,20);
	//  grayBg.draw(20,280, 320, 180);
  //  grayDiff.draw(450, 350, 400, 300);

	ofSetColor(0xffffff);
  //	contourFinder.draw(450, 350, 400, 300);
	ofSetColor(0xffffff);
	//  calibratedTex.draw(10,350,400,300);

  
// ******** ********** 
  if (do_shader) {
  
    ofDisableAlphaBlending();
    ofPushMatrix();
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    shader.begin();

    videoImage.getTextureReference().bind();
    depthImage.getTextureReference().bind();
    
    shader.setUniformTexture("depthMap", depthImage, 0);
//    shader.setUniform1f("depthScaling", 320.f);
    shader.setUniform3f("lightDir", sin(ofGetElapsedTimef()), cos(ofGetElapsedTimef()), 0);
    
    float w = videoImage.getWidth();
    float h = videoImage.getHeight();
    
    glTranslatef(ofGetWidth() / 2, ofGetHeight() / 2, -100);
    rotX = ofLerp(mouseX, rotX, 0.5);
    rotY = ofLerp(mouseY, rotY, 0.5);
    glRotatef(rotX, 0, 1, 0);
    glRotatef(-rotY, 1, 0, 0);
    glTranslatef(-w / 2, -h / 2, -320);
    
    
    int step = render_steps;

    ofPoint texTL = videoImage.getTextureReference().getCoordFromPoint(0, 0);
    ofPoint texBR = videoImage.getTextureReference().getCoordFromPoint(w, h);
    
    GLfloat grid2x2[2][2][3] = {
      {{0.0, 0.0, 0.0}, {w, 0.0, 0.0}},
      {{0.0, h, 0.0}, {w, h, 0.0}}
    };

    GLfloat texpts[2][2][2] = {
      {{0.0,0.0},{texBR.x,0.0}},
      {{0.0,texBR.y},{texBR.x,texBR.y}}
    };    
    
    glEnable(GL_MAP2_TEXTURE_COORD_2);
    glShadeModel(GL_FLAT);
    glEnable(GL_MAP2_VERTEX_3);
    glMap2f(GL_MAP2_VERTEX_3,
            0.0, 1.0,  /* U ranges 0..1 */
            3,         /* U stride, 3 floats per coord */
            2,         /* U is 2nd order, ie. linear */
            0.0, 1.0,  /* V ranges 0..1 */
            2 * 3,     /* V stride, row is 2 coords, 3 floats per coord */
            2,         /* V is 2nd order, ie linear */
            &grid2x2[0][0][0]);  /* control points */

    
    glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 
                                     0, 1, 4, 2, 
            &texpts[0][0][0]);
    
    
    glMapGrid2f(
                w/step, 0.0, 1.0,
                h/step, 0.0, 1.0);
    
    glEvalMesh2(render_mode,
                0, w/step,   /* Starting at 0 mesh w/step steps (rows). */
                0, h/step);  /* Starting at 0 mesh h/step (columns). */
    
    
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
      sprintf(matrixStr, "%12.6f %12.6f %12.6f %12.6f >> %d", 
                  mx.getPtr()[i*4], 
                  mx.getPtr()[i*4+1], 
                  mx.getPtr()[i*4+2],
                  mx.getPtr()[i*4+3], 
                  kinect.getDepthPixelBackAt(100+10*i,100+10*i));
      ofDrawBitmapString(matrixStr, 20, 670 + i*20);
    }
    
    glColor3f(1, 1, 1);
    videoImage.draw(10,10,320,240);
    depthImage.draw(20+320,10, 320, 240);
    
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

  
  GLint render_modes[3]={GL_LINE, GL_FILL, GL_POINT};
  
  
  switch (key)
	{
		case ' ':
			do_shader ^= true;
			break;
    case 'r':
      render_mode = render_modes[(int)ofRandom(0,3)];
      break;

    case 'f':
        ofToggleFullscreen();
      break;
		case '+': render_steps++;
			break;
    case '-': 
      render_steps>1 ? render_steps--: render_steps;
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

