// This sketch is for building particle system [Mover]

#define N (300)           // number of particles
#define CLOUD_WIDTH (5.0) // width particles
struct Mover {
  float G;
  float mass;
  float distance;
  Vec3f position, velocity, acceleration;
  double boundary_radius = 15; // a spherical boundary with center at (0,0,0)
  Vec3f boundary_origin;
  float mover_timestep = 0.06;

  Mover() {
    position =
        Vec3f(rnd::uniformS(), rnd::uniformS(), rnd::uniformS()) * CLOUD_WIDTH;
    velocity = Vec3f(0.0f, 0.0f, 0.0f);
    mass = rnd::uniform(10.0f);
    acceleration = Vec3f(0.0f, 0.0f, 0.0f);
    boundary_origin = Vec3f(0.0f, 0.0f, 0.0f);
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
    velocity += acceleration * mover_timestep;
    position += velocity * mover_timestep;
    acceleration = 0.0f * acceleration;
  }

  void checkEdge() {
    Vec3f d = position - boundary_origin;
    if (d > boundary_radius) {
      velocity *= -1;
    }
  }
};

vector<Mover> movers;
Mesh mover_Mesh{Mesh::POINTS};

struct Movers {
  void setup() {
    for (int i = 0; i < N; i++) {
      mover_Mesh.vertex(
          Vec3f(rnd::uniformS(), rnd::uniformS(), rnd::uniformS()) *
          CLOUD_WIDTH);
      Mover m;
      movers.push_back(m);
    }
  }

  void draw(Graphics &g) {
    for (int i = 0; i < movers.size(); i++) {
      for (int j = i + 1; j < movers.size(); j++) {
        if ((movers[i].position - movers[j].position).mag() < 1.0f)
          line(g, movers[i].position, movers[j].position);
      }

      rect(g, movers[i].position, rnd::uniform(0.01, 0.1), 0.01);
      rect(g, movers[i].position, 0.01, rnd::uniform(0.01, 0.1));
    }
  }

  void update() {
    for (int i = 0; i < N; i++) {
      for (int j = 1 + i; j < N; j++) {
        Mover &a = movers[i];
        Mover &b = movers[j];
        Vec3f force1 = a.attract(b);
        movers[i].applyForce(force1);
        Vec3f force2 = -b.attract(a);
        movers[j].applyForce(force2);
      }
      movers[i].update();
      movers[i].checkEdge();
    }
  }
};
