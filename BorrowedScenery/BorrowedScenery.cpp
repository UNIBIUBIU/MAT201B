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
float timestep = 1.0f;
struct SharedState {
  Pose pose;
  double time{0};
  // double angle{0};
};

#include "helper.h"

#include "Animation.h"
#include "Mover.h"
#include "PixelCloud.h"
#include "PointCloud.h"
#include "grid.h"

// struct AlloApp : App {

struct DistributedExampleApp : DistributedApp<SharedState> {
  DistributedExampleApp() {
    if (hasRole(ROLE_RENDERER)) {
      displayMode(displayMode() | Window::STEREO_BUF);
    }
  }

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
    samplePlayer.load("../asset/soundtrack.wav");
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
    // opening
    if (animation.opening(time)) {
      g.clear(0);
      ruler.draw(g);
      if (time > opening_pointcloud && time < opening_pointcloud + 6) {
        pixelCloud.box_draw(g);
      } else if (time > opening_pointcloud + 6) {
        pointCloud.draw(g);
      }
    }

    // transition
    if (animation.transition(time)) {
      pixelCloud.section1_draw(g);
    }

    // section 1
    if (animation.section1(time)) {
      g.clear(0);
      pixelCloud.section1_draw(g);
      if (time > grid_start) {
        grid.draw_grid_1(g);
      }
    }

    // section 2
    if (animation.section2(time)) {
      if (time < spiral_start) {
        g.clear(0);
      } else if (time > spiral_end) {
        g.clear(0);
        pixelCloud.section1_draw(g);
        grid.draw_grid_1(g);
      }
      g.color(0);
      if (time < spiral_end) {
        // ruler
        ruler.draw(g);
        g.pushMatrix();
        g.rotate(90, Vec3f(0, 0, 1));
        ruler.draw(g);
        g.popMatrix();
        // pixelcloud section 2
        pixelCloud.section2_draw(g);
      }
    }
    // section3
    if (animation.section3(time)) {
      g.clear(0);
      g.color(1);
      grid.draw_grid_2(g);
      if (time < section3_riverend) {
        pixelCloud.section3_draw(g);
      } else {
        pixelCloud.box_draw(g);
      }
    }
    // section4
    if (animation.section4(time)) {
      g.clear(0);
      grid.draw_grid_1(g);
      g.pushMatrix();
      g.translate(0, 0, 10);
      g.rotate(140, Vec3f(0, 1, 0));
      pixelCloud.section1_draw(g);
      g.popMatrix();
    }
    // ending
    if (animation.ending(time)) {
      g.clear(0);
      grid.draw_grid_1(g);
      pointCloud.draw(g);
    }
    if (time > ending_end) {
      g.clear(0);
    }

    // Movers
    if (animation.movers(time)) {
      g.color(1);
      movers.draw(g);
    }
  }

  virtual void simulate(double dt) override {
    state().pose = nav();
    // if (app.isPrimary()) {
    // state().angle += 0.01;
    state().time += dt * timestep;
  }

  void onAnimate(double dt) override {
    if (hasRole(ROLE_RENDERER) && !hasRole(ROLE_DESKTOP)) {
      pose() = state().pose;
    }
    if (pause) {
      return;
    }

    double time = state().time;
    // double angle = state().angle;
    // opening

    if (animation.opening(time)) {
      // pixelCloud.box_update(dt);
      ruler.update(dt);
      if (time > opening_pointcloud && time < opening_pointcloud + 6) {
        pixelCloud.box_update(dt);
      } else if (time > opening_pointcloud + 6) {
        pointCloud.update(dt);
      }
    }

    // mover animation
    if (animation.movers(time)) {
      movers.update();
    }

    if (animation.transition(time) || animation.section1(time)) {
      pixelCloud.section1_update(dt);
      ruler.reset();
    }

    if (animation.section2(time)) {
      pixelCloud.section2_update(dt);
      ruler.update(dt);
      if (time < spiral_start) {
        pixelCloud.reset(dt);
      } else if (time > spiral_end) {
        pixelCloud.section1_update(dt);
      }
    }

    if (animation.section3(time)) {
      if (time < section3_riverend) {
        pixelCloud.section3_update(dt);
        pixelCloud.reset(dt);
      } else {
        pixelCloud.box_update(dt);
      }
    }
    if (animation.section4(time)) {
      pixelCloud.section1_update(dt);
    }
    if (animation.ending(time)) {
      pointCloud.update(dt);
    }
  }

  // interaction
  void onKeyDown(const Keyboard &k) override {
    switch (k.key()) {
      case 'j':
        pause = !pause;
        break;
      case 'k':
        timestep = 10;
        break;
      case 'l':
        timestep = 1;
        break;
    }
  }

  void onSound(AudioIOData &io) override {
    int n = io.channelsOut();
    while (io()) {
      float f = samplePlayer() / n;
      for (int i = 0; i < n; ++i) {
        io.out(i) = f;
      }
    }
  }
};

int main(int argc, char *const argv[]) {
  DistributedExampleApp app;

  // for the sphere...
  app.audioIO().device(AudioDevice("ECHO XS"));

  app.initAudio();
  app.start();
  return 0;
};
