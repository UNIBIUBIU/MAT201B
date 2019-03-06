// this project I explored how to arrange pixels of an image in 3d space
// I adjust the color for aesthetic experiment

#include "al/core.hpp"
#include "al/util/al_Image.hpp" // gets us Image
using namespace al;
using namespace std;

// create a map function like processing
float mapFunction(float value, float low, float high, float Low, float High) {
  return Low + (High - Low) * ((value - low) / (high - low));
}

struct MyApp : App {
  Mesh mesh{Mesh::POINTS};
  vector<Vec3f> pos[4];

  void onCreate() override {
    Image image;
    const char *fileName = "../fs.png";
    if (!image.load(fileName)) {
      exit(1);
    }
    nav().pullBack(7); // set up the camera

    Image::RGBAPix<uint8_t> pixel;
    for (int row = 0; row < image.height(); row++) {
      for (int column = 0; column < image.width(); column++) {
        image.read(pixel, column, row);

        // image 2d
        Vec3f o;
        o.x = mapFunction(column, 0, image.width(), -1, 1);
        o.y = mapFunction(row, 0, image.height(), -1, 1);
        o.z = 0;
        pos[0].push_back(o);

        // RGB cube
        Vec3f r;
        r.x = mapFunction(pixel.r, 0, 255.0, -1, 1);
        r.y = mapFunction(pixel.g, 0, 255.0, -1, 1);
        r.z = mapFunction(pixel.b, 0, 255.0, -1, 1);
        pos[1].push_back(r - Vec3f(0.5, 0.5, 0.5));

        // HSV
        Vec3f h;
        HSV color(RGB(pixel.r, pixel.g, pixel.b));
        h.x = color.s * sin(M_2PI * color.h);
        h.y = color.s * cos(M_2PI * color.h);
        h.z = color.v / 255.0;
        pos[2].push_back(h);

        // customized
        Vec3f c;
        c.x = o.x * r.x;
        c.y = r.z;
        c.z = r.y * h.z;
        pos[3].push_back(c);
        mesh.vertex(o);

        float gray =
            HSV(RGB(pixel.r / 255.0, pixel.g / 255.0, pixel.b / 255.0)).v;
        gray = mapFunction(gray, 0, 1, 0, 2);
        mesh.color(Color(gray));
        // mesh.color(pixel.r / 255.0, pixel.g / 255.0, pixel.b / 255.0);
      }
    }
  }

  int target = 0;
  int previousTarget = 0;
  bool simulate = true;
  float t = 0;

  void onAnimate(double dt) override {
    if (!simulate) {
      return;
    }
    // dt is in seconds

    t += dt / 3;
    /*
    if (t > 0 && t < 1) {
      auto &m = mesh.vertices();
      // how to change color ? auto &c = mesh.color();
      for (int i = 0; i < m.size(); ++i)
        // not quite sure about this
        m[i] = pos[previousTarget][i] * (1 - t) + pos[target][i] * t;
    }

    if (t > 1 && t < 2) {
      auto &m = mesh.vertices();
      for (int i = 0; i < m.size(); ++i)
        m[i] = pos[target][i];
    }
    */
    if (t > 0 && t < 1) {
      auto &m = mesh.vertices();
      for (int i = 0; i < m.size(); i++) {
        // m[i] = pos[previousTarget][i].lerp(pos[target][i], t);
        m[i] = lerp(pos[previousTarget][i], pos[target][i], t);
      }
    }
  }

  void onDraw(Graphics &g) override {
    g.clear(0);
    g.meshColor();
    g.draw(mesh);
  }
  void onKeyDown(const Keyboard &k) override {
    if (k.key() == '1') {
      t = 0;
      previousTarget = target;
      target = 0;

    } else if (k.key() == '2') {
      t = 0;
      previousTarget = target;
      target = 1;

    } else if (k.key() == '3') {
      t = 0;
      previousTarget = target;
      target = 2;

    } else if (k.key() == '4') {
      t = 0;
      previousTarget = target;
      target = 3;
    } else if (k.key() == 'p') {
      simulate = !simulate;
    }
  }
};

int main() {
  MyApp app;
  app.start();
}
