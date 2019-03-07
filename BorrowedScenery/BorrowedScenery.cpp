#include "Gamma/SamplePlayer.h"  //sound
#include "al/core.hpp"
#include "al/core/app/al_DistributedApp.hpp"
#include "al/util/al_Asset.hpp"
#include "al/util/al_Image.hpp"

#include <algorithm>  // max
#include <cstdint>    // uint8_t
#include <fstream>
#include <sstream>
#include <vector>
#include "shader.h"
using namespace al;
using namespace gam;
using namespace std;

struct SharedState {
  double time{0};
  double angle{0};
};

#include "helper.h"

#include "Animation.h"
#include "Mover.h"
#include "PixelCloud.h"
#include "PointCloud.h"
#include "grid.h"

// struct AlloApp : App {

struct DistributedExampleApp : DistributedApp<SharedState> {
  bool pause = false;
  PixelCloud pixelCloud;
  Movers movers;
  PointCloud pointCloud;
  Animation animation;
  Grid grid;
  Ruler ruler;
  gam::SamplePlayer<float, gam::ipl::Linear, gam::phsInc::Loop> samplePlayer;

  void onCreate() override {
    // load sound
    samplePlayer.load("../asset/test_3.wav");
    // samplePlayer.loop();
    Sync::master().spu(audioIO().fps());
    // load elements
    pointCloud.setup();
    grid.setup();
    movers.setup();
    // animation - schedule
    animation.scheduleAnimation();

    if (role() == ROLE_RENDERER) {
      load_perprojection_configuration();
      cursorHide(true);
    }
  }

  void onDraw(Graphics &g) override {
    double time = state().time;
    double angle = state().angle;

    g.rotate(angle, Vec3f(0, 1, 0));
    // opening
    if (animation.opening(time)) {
      g.clear(0);
      ruler.draw(g);
      if (time > opening_pointcloud) {
        pointCloud.draw(g);
      }
    }

    // transition
    if (animation.transition(time)) {
      pixelCloud.section1_draw(g);
    }

    // section 1
    if (animation.section1(time)) {
      if (time < blink1) {
        g.clear(rnd::uniform(0.0, 1.0));
      } else if (time > blink2) {
        g.clear(rnd::uniform(0.0, 1.0));
      } else {
        g.clear(0);
      }
      pixelCloud.section1_draw(g);
      if (time > grid_start) {
        grid.draw(g);
      }
    }

    // section 2
    if (animation.section2(time)) {
      if (time < spiral) {
        g.clear(0.8);
      }
      g.color(0);
      // connector
      circle(g, Vec3f(0, 0, -20), 10);
      circle(g, Vec3f(0, 0, 40), 10);
      line(g, Vec3f(0, 0, -20), Vec3f(0, 0, 40));

      ruler.draw(g);
      pixelCloud.section2_draw(g);
      g.pushMatrix();
      g.rotate(90, Vec3f(0, 0, 1));
      ruler.draw(g);
      g.popMatrix();
    }

    // Movers
    if (animation.movers(time)) {
      if (time > spiral && time < section2_end) {
        g.color(0);
      } else {
        g.color(RGB(0.8, 0.8, 0.8));
      }
      movers.draw(g);
    }
  }

  virtual void simulate(double dt) override {
    // if (app.isPrimary()) {
    state().angle += 0.01;
    state().time += dt;
  }

  void onAnimate(double dt) override {
    if (pause) {
      return;
    }

    double time = state().time;
    double angle = state().angle;

    // initial nav
    nav().pos(nav_current);

    // opening
    if (animation.opening(time)) {
      ruler.update(dt);
      if (time > opening_pointcloud) {
        pointCloud.update(dt);
      }
    }

    // mover animation
    if (animation.movers(time)) {
      movers.update();
    }

    if (animation.transition(time) || animation.section1(time)) {
      pixelCloud.section1_update(dt);
      // animate nav
      if (nav_current.x < 8) {
        nav_current.x += dt;
      }
      nav().pos(nav_current);
      ruler.reset();
    }

    if (animation.section2(time)) {
      angle = 0.0;
      pixelCloud.section2_update(dt);
      nav().pos(Vec3f(0, 0, 19));
      ruler.update(dt);
    }
  }

  // interaction
  void onKeyDown(const Keyboard &k) override {
    switch (k.key()) {
      case 'j':
        pause = !pause;
        break;
    }
  }

  void onSound(AudioIOData &io) override {
    while (io()) {
      float f = samplePlayer();
      io.out(0) = f;
      io.out(1) = f;
    }
  }
};

int main(int argc, char *const argv[]) {
  DistributedExampleApp app;
  if (role() != ROLE_RENDERER) app.initAudio();
  app.start();
  return 0;
};
