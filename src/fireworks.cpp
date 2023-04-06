//--------------------------------------------------
// Author: Yue Chen
// Date: 04/06/2023
// Description: create fireworks using particle system
//--------------------------------------------------

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
  float rot;
  float size;
  bool isDone;
};

class Viewer : public Window {
public:
  Viewer() : Window() {
  }

  float g = 2.0;
  float size = 0.1;
  void setup() {
    setWindowSize(1000, 1000);
    createConfetti();
    renderer.setDepthTest(false);
    renderer.blendMode(agl::ADD);
  }

  void createConfetti()
  {
    renderer.loadTexture("particle", "../textures/particle.png", 0);
    renderer.loadTexture("bokeh", "../textures/ParticleBokeh.png", 0);
    Particle particle;
    particle.vel = vec3(0,3,0);
    particle.color = vec4(0.95,0.75,0.8,1);
    particle.size = size;
    particle.rot = 0.0;
    particle.pos = vec3(0,-2,0);
    particle.isDone = false;
    mParticles.push_back(particle);
    
  }

   void updateConfetti(float dt)
  {
    bool isUpdated = false;

    for(int i =1;i<mParticles.size();i++){
      Particle particle = mParticles[i];
      if(particle.isDone&&!isUpdated){
        particle.isDone = false;
        particle.vel = vec3(0,0,0);
        particle.color = vec4(0.95,0.75,0.8,1);     
        particle.pos = mParticles[0].pos;
        particle.size = size;
        particle.rot = 0.0;
        isUpdated = true;
      }
      else if(!particle.isDone){
        if(particle.size<=0){
          particle.isDone = true;
        }
        //updating the trails
        else{
          float alpha = particle.color[3];
          particle.color = vec4(vec3(particle.color),alpha-dt/2);
          particle.size -= 0.001;
        }
      }
      mParticles[i]=particle;
    }
    if(!isUpdated){
      Particle particle;
      particle.vel = vec3(0,0,0);
      particle.color = vec4(0.95,0.75,0.8,1);
      particle.size = size;
      particle.rot = 0.0;
      particle.pos = mParticles[0].pos;
      mParticles.push_back(particle);
    }

    mParticles[0].vel = mParticles[0].vel-vec3(0,g*dt,0);
    mParticles[0].pos += dt*mParticles[0].vel;

    if(mParticles[0].vel[1]<=0){
      //update all the particles
      for(int i=0; i<20; i++){
        Particle p;
        p.color = vec4(0.95,0.75,0.8,1);
        p.isDone = false;
        p.pos = mParticles[0].pos;
        p.rot = 0;
        float angle = (rand()%361)/180.0*3.1415;
        p.vel = vec3(cos(angle),sin(angle),0);
        p.size = size;
        fires.push_back(p);
      }
      mParticles[0].vel = vec3(0,3,0);
      mParticles[0].color = vec4(0.95,0.75,0.8,1);
      mParticles[0].pos = vec3((rand()%15-7)/5.0,-2,0);
    }

    if(fires.size()>0&&fires[0].size>=0.25){
      fires.clear();
      mFires.clear();
    }else{
      bool isDone = false;
      for(int i=0;i<fires.size();i++){
        for(int j=0;(i+j)<mFires.size();j+=20){
          Particle particle = mFires[i+j];
          if(particle.isDone&&!isDone){
            particle.isDone = false;
            particle.vel = vec3(0,0,0);
            particle.color = vec4(agl::randomUnitCube(),1);     
            particle.pos = fires[i].pos;
            particle.size = size;
            particle.rot = 0.0;
            isDone = true;
          }
          else if(!particle.isDone){
            if(particle.size<=0){
              particle.isDone = true;
            }
            //updating the trails
            else{
              float alpha = particle.color[3];
              particle.color = vec4(vec3(particle.color),alpha-dt/2);
              particle.size -= 0.001;
            }
          }
          mFires[i+j]=particle;
          
        }
        if(!isDone){
          Particle particle;
          particle.vel = vec3(0,0,0);
          particle.color = vec4(agl::randomUnitCube(),1);
          particle.size = size;
          particle.rot = 0.0;
          particle.pos = fires[i].pos;
          mFires.push_back(particle);
        }

        fires[i].vel = fires[i].vel-vec3(0,g*dt,0);
        fires[i].pos += fires[i].vel*dt; 
        float alpha = fires[i].color[3];
        fires[i].color = vec4(vec3(fires[i].color),alpha-dt);
        fires[i].size += 0.001;
      }
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

    for (int i = 0; i < mFires.size(); i++)
    {
      Particle p = mFires[i];
      renderer.sprite(p.pos, p.color, p.size, p.rot);
    }    
    renderer.texture("image", "bokeh");
    for (int i = 0; i < fires.size(); i++)
    {
      Particle p = fires[i];
      renderer.sprite(p.pos, p.color, p.size, p.rot);
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
  std::vector<Particle> fires;
  std::vector<Particle> mFires;
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
