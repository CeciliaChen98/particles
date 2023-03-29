// Bryn Mawr College, alinen, 2020
//

#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"

using namespace std;
using namespace glm;
using namespace agl;

struct Particle {
  glm::vec3 pos;
  glm::vec3 vel;
  glm::vec4 color;
  float time;
  int frame;
  float rot;
  float size;
  bool isDone;
};

class Viewer : public Window {
public:
  Viewer() : Window() {
  }

  void setup() {
    setWindowSize(1000, 1000);
    createConfetti();
    renderer.setDepthTest(false);
    renderer.blendMode(agl::ADD);
  }

  void createConfetti()
  {
    renderer.loadTexture("particle", "../textures/star4.png", 0);
    Particle particle;
    particle.vel = vec3(0,1,0);
    particle.color = vec4(1,1,1,1);
    particle.size = 0.25;
    particle.rot = 0.0;
    particle.pos = vec3(1,0,0);
    particle.isDone = false;
    particle.time = 0;
    particle.frame = 0;
    mParticles.push_back(particle);
    
  }

   void updateConfetti(float dt)
  {
    bool isUpdated = (((int)(mParticles[0].frame)%4)!=0);
    mParticles[0].time +=dt;
    mParticles[0].frame ++;
    mParticles[0].vel = vec3(-sin(mParticles[0].time),cos(mParticles[0].time),0);
    mParticles[0].pos += dt*mParticles[0].vel;
    int t = mParticles[0].frame % 3;

    for(int i =1;i<mParticles.size();i++){
      Particle particle = mParticles[i];
      if(particle.isDone&&!isUpdated){
        particle.isDone = false;
        particle.vel = vec3(-sin(mParticles[0].time),cos(mParticles[0].time)+0.3*(t-1),0);
        particle.color = vec4(agl::randomUnitCube(),1);     
        particle.pos = mParticles[0].pos;
        particle.size = 0.25;
        particle.rot = 0.0;
        isUpdated = true;
      }
      else if(!particle.isDone){
        float alpha = particle.color[3];
        if(alpha-dt/2<=0){
          particle.isDone = true;
        }else{
          particle.pos -= particle.vel *(3*dt);
          particle.color = vec4(vec3(particle.color),alpha-dt/2);
          particle.size += dt/5;
          particle.rot += dt ;
        }
      }
      mParticles[i]=particle;
    }
    if(!isUpdated){
      Particle particle;
      particle.vel = vec3(-sin(mParticles[0].time),cos(mParticles[0].time)+0.3*(t-1),0);
      particle.color = vec4(agl::randomUnitCube(),1);
      particle.size = 0.25;
      particle.rot = 0.0;
      particle.pos = mParticles[0].pos;
      particle.time = 0;
      mParticles.push_back(particle);
    }
  }

  void drawConfetti()
  {
    renderer.texture("image", "particle");
    for (int i = 0; i < mParticles.size(); i++)
    {
      Particle particle = mParticles[i];
      renderer.sprite(particle.pos, particle.color, particle.size, particle.rot);
    }
  }

  void mouseMotion(int x, int y, int dx, int dy) {
  }

  void mouseDown(int button, int mods) {
  }

  void mouseUp(int button, int mods) {
  }

  void scroll(float dx, float dy) {
    eyePos.z += dy;
  }

  void keyUp(int key, int mods) {
  }

  void draw() {
    renderer.beginShader("sprite");

    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);

    renderer.lookAt(eyePos, lookPos, up);
    //renderer.sprite(position, vec4(1.0f), 0.25f);
    updateConfetti(dt()/2);
    drawConfetti();
    renderer.endShader();
  }

protected:

  vec3 eyePos = vec3(0, 0, 3);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  vec3 position = vec3(1, 0, 0);

  std::vector<Particle> mParticles;
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
