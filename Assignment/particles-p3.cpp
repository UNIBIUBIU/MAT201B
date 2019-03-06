/*
Assignment 3 - Weidi Zhang
implement a unstable particle system
Unbalanced force
*/

#include "al/core.hpp"
using namespace al;

#include <vector>
using namespace std;

#define N (1000) // number of particles
#define CLOUD_WIDTH (10.0)

const char *vertex = R"(
#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec4 vertexColor;

uniform mat4 al_ModelViewMatrix;
uniform mat4 al_ProjectionMatrix;

out Vertex {
  vec4 color;
} vertex;

void main() {
  gl_Position = al_ModelViewMatrix * vec4(vertexPosition, 1.0);
  vertex.color = vertexColor;
}
)";

const char *fragment = R"(
#version 400

in Fragment {
  vec4 color;
  vec2 textureCoordinate;
} fragment;

uniform sampler2D alphaTexture;

layout (location = 0) out vec4 fragmentColor;

void main() {
  // use the first 3 components of the color (xyz is rgb), but take the alpha value from the texture
  //
  fragmentColor = vec4(fragment.color.xyz, texture(alphaTexture, fragment.textureCoordinate));
}
)";

const char *geometry = R"(
#version 400

// take in a point and output a triangle strip with 4 vertices (aka a "quad")
//
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 al_ProjectionMatrix;

// this uniform is *not* passed in automatically by AlloLib; do it manually
//
uniform float halfSize;

in Vertex {
  vec4 color;
} vertex[];

out Fragment {
  vec4 color;
  vec2 textureCoordinate;
} fragment;

void main() {
  mat4 m = al_ProjectionMatrix; // rename to make lines shorter
  vec4 v = gl_in[0].gl_Position; // al_ModelViewMatrix * gl_Position

  gl_Position = m * (v + vec4(-halfSize, -halfSize, 0.0, 0.0));
  fragment.textureCoordinate = vec2(0.0, 0.0);
  fragment.color = vertex[0].color;
  EmitVertex();

  gl_Position = m * (v + vec4(halfSize, -halfSize, 0.0, 0.0));
  fragment.textureCoordinate = vec2(1.0, 0.0);
  fragment.color = vertex[0].color;
  EmitVertex();

  gl_Position = m * (v + vec4(-halfSize, halfSize, 0.0, 0.0));
  fragment.textureCoordinate = vec2(0.0, 1.0);
  fragment.color = vertex[0].color;
  EmitVertex();

  gl_Position = m * (v + vec4(halfSize, halfSize, 0.0, 0.0));
  fragment.textureCoordinate = vec2(1.0, 1.0);
  fragment.color = vertex[0].color;
  EmitVertex();

  EndPrimitive();
}
)";
Mesh pointMesh;
double boundary_radius = 50;    // a spherical boundary with center at (0,0,0)
Vec3f boundary_origin(0, 0, 0); // boundary center as (0,0,0)

float symmetry = 1.0; // 0.999

struct Mover {
  float G;
  float mass;
  float distance;
  Vec3f position, velocity, acceleration;
  float timestep = 0.01;
  // Mesh aSphere;
  Mover() {
    // addSphere(aSphere);
    position =
        Vec3f(rnd::uniformS(), rnd::uniformS(), rnd::uniformS()) * CLOUD_WIDTH;
    velocity = Vec3f(0.0f, 0.0f, 0.0f);
    mass = rnd::uniform(5.0f);
    acceleration = Vec3f(0.0f, 0.0f, 0.0f);
    G = 0.1f;
  }

  void applyForce(Vec3f &f) {
    f = f / mass;
    acceleration += f;
  }

  Vec3f attract(Mover &m) {
    Vec3f force = m.position - position;

    distance = force.mag();
    if (force.mag() > 4000) {
      force.normalize(4000);
    }

    force = force.normalize();
    float strength = (G * mass * m.mass) / (distance * distance);
    force = force * strength;
    return force;
  }

  void update() {
    velocity += acceleration * timestep;
    position += velocity * timestep;
    acceleration = 0.0f * acceleration;
  }

  void boundary_detect() {
    Vec3f d = position - boundary_origin;
    if (d > boundary_radius) {
      velocity *= -1;
    }
  }
};

struct AlloApp : App {
  ShaderProgram shader;
  Texture texture;
  vector<Mover> movers;

  void onCreate() override {
    // use a texture to control the alpha channel of each particle
    texture.create2D(256, 256, Texture::R8, Texture::RED, Texture::SHORT);
    int Nx = texture.width();
    int Ny = texture.height();
    std::vector<short> alpha;
    alpha.resize(Nx * Ny);
    for (int j = 0; j < Ny; ++j) {
      float y = float(j) / (Ny - 1) * 2 - 1;
      for (int i = 0; i < Nx; ++i) {
        float x = float(i) / (Nx - 1) * 2 - 1;
        float m = exp(-13 * (x * x + y * y));
        m *= pow(2, 15) - 1; // scale by the largest positive short int
        alpha[j * Nx + i] = m;
      }
    }
    texture.submit(&alpha[0]);
    // compile and link the three shaders
    shader.compile(vertex, fragment, geometry);
    // create a mesh of points scattered randomly

    pointMesh.primitive(Mesh::POINTS);
    for (int i = 0; i < N; i++) {
      pointMesh.vertex(
          Vec3f(rnd::uniformS(), rnd::uniformS(), rnd::uniformS()) *
          CLOUD_WIDTH);
      Mover m;
      pointMesh.color(RGB(1.0, 1.0, 1.0));
      movers.push_back(m);
    }
  }
  void onAnimate(double dt) override {
    for (int i = 0; i < N; i++) {
      for (int j = 1 + i; j < N; j++) {
        Mover &a = movers[i];
        Mover &b = movers[j];
        //  if (i != j) {
        Vec3f force1 = a.attract(b);
        movers[i].applyForce(force1);
        Vec3f force2 = -b.attract(a);
        movers[j].applyForce(force2 * symmetry);
        //    }
      }
      movers[i].update();
      movers[i].boundary_detect();
    }

    for (int i = 0; i < movers.size(); i++)
      pointMesh.vertices()[i] = movers[i].position; // copy particle position
  }

  void onDraw(Graphics &g) override {
    g.clear(0);
    g.depthTesting(false);
    g.blending(true);
    g.blendModeTrans();

    texture.bind();
    g.shader(shader);
    g.shader().uniform("halfSize", 0.2); // what does halfSize mean?
    g.draw(pointMesh);
    texture.unbind();
  }
};

void reset() {}

int main() { AlloApp().start(); }
