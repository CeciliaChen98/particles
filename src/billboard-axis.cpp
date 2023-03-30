// Bryn Mawr College, alinen, 2020
//

#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"

using namespace std;
using namespace glm;
using namespace agl;

class Viewer : public Window {
public:
  Viewer() : Window() {
  }

  void setup() {
    setWindowSize(1000, 1000);
    renderer.loadShader("simple-texture",
      "../shaders/simple-texture.vs",
      "../shaders/simple-texture.fs");

    Image img;
    img.load("../textures/tree.png", true);
    renderer.loadTexture("tree", img, 0);
    // Use the width and the height of the image to scale the billboard
    scale = img.height()/(float)img.width();
    renderer.loadTexture("grass", "../textures/grass.png", 0);
    renderer.blendMode(agl::BLEND);
  }


  void mouseMotion(int x, int y, int dx, int dy) {
      if (mouseIsDown(GLFW_MOUSE_BUTTON_LEFT)){
         azimuth-=(dx/2);
         elevation+=(dy/2);
         if(azimuth<-90){
            azimuth+=1800;
         }
         if(elevation<-90){
            elevation = -90;
         }
         if(azimuth>90){
            azimuth-=180;
         }
         if(elevation>90){
            elevation = 90;
         }
         
         updateEyePos();
      }
   }

  void updateEyePos(){
    float pie = 3.1415926;
    float eyeX = 2* sin(azimuth/360.0*2*pie)*cos(elevation/360.0*2*pie);
    float eyeY = 2* sin(elevation/360.0*2*pie);
    float eyeZ = 2* cos(azimuth/360.0*2*pie)*cos(elevation/360.0*2*pie);
    eyePos = vec3(eyeX,eyeY,eyeZ);
  }

  void mouseDown(int button, int mods) {
  }

  void mouseUp(int button, int mods) {
  }

  void scroll(float dx, float dy) {
  }

  void draw() {
    renderer.beginShader("simple-texture");

    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);
    renderer.lookAt(eyePos, lookPos, up);
    
    // draw plane
    renderer.texture("Image", "grass");
    renderer.push();
    renderer.translate(vec3(0.0, -0.5, 0));
    renderer.scale(vec3(2.0f));
    renderer.plane();
    renderer.pop();

    float nx = dot(eyePos,vec3(1,0,0));
    float nz = dot(eyePos,vec3(0,0,1));

    float angle = atan(nx/nz);


    // draw tree
    renderer.texture("Image", "tree");
    renderer.rotate(vec3(0,angle,0));    
    renderer.translate(vec3(-0.5, -0.5, 0));
    renderer.pop();
    renderer.scale(vec3(1,scale,1));

    renderer.quad(); // vertices span from (0,0,0) to (1,1,0)
    //renderer.pop();

    renderer.endShader();
  }

protected:

  vec3 eyePos = vec3(0, 0, 2);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  int azimuth = 0;
  int elevation = 0;
  float scale = 0;
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
