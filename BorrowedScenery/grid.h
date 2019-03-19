// The sketch is to draw grid and Ruler and box
struct Ruler {
  float offset = 0.0f;
  float acel = 0.01f;
  float velocity = 0.0f;
  float time;
  float dis = 1;
  float timestep = 2;
  float count = 1;

  void draw(Graphics &g) {
    g.color(1);
    cross(g, Vec3f(0, 0, -20), 0.05, 0.3);
    for (int x = -200; x < 201; x += dis) {
      Vec3f pos(x + offset, 0, -20);
      rect(g, pos, 0.1, 0.02);
      rect(g, pos, 0.01 + dis / 200, 0.3 + dis / 20);
      for (int i = 1; i < 11; i++) {
        Vec3f pos2(x + i * dis / 10 + offset, 0, -20);
        rect(g, pos2, 0.01 + dis / 200, 0.05 + dis / 40);
      }
    }
  }

  void update(double &dt) {
    offset += velocity;
    velocity += acel;
    // animation for the scale of ruler points
    if (offset > 0) {
      dis += dt / timestep;
    }
    // animation for the ruler
    if (offset > count) {
      time += dt;
      if (time < 0.5) {
        velocity = 0;
      }
      time = 0;
      count = count + 1;
    }
  }

  void reset() {
    dis = 1.0f;
    offset = 0.0f;
  }
};

// Grid
struct Grid {
  Mesh grid;
  ShaderProgram lineShader;
  ShaderProgram pointShader;
  Texture pointTexture;
  Texture lineTexture;

  void setup() {
    //-----shader ---------------------------------------------------
    // use a texture to control the alpha channel of each particle
    pointTexture.create2D(256, 256, Texture::R8, Texture::RED, Texture::SHORT);
    int Nx = pointTexture.width();
    int Ny = pointTexture.height();
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
    pointTexture.submit(&alpha[0]);
    lineTexture.create1D(256, Texture::R8, Texture::RED, Texture::SHORT);
    std::vector<short> beta;
    beta.resize(lineTexture.width());
    for (int i = 0; i < beta.size(); ++i) {
      beta[i] = alpha[128 * beta.size() + i];
    }
    lineTexture.submit(&beta[0]);

    // compile and link the shaders
    pointShader.compile(vertexPoint, fragmentPoint, geometryPoint);
    lineShader.compile(vertexLine, fragmentLine, geometryLine);

    // draw grid_1
    grid.primitive(Mesh::LINES);
    for (int y = -50; y < 101; y += 10) {
      for (int z = -100; z < 101; z += 10) {
        grid.color(Color(1));
        grid.vertex(100, y, z);
        grid.vertex(-100, y, z);
        grid.texCoord(rnd::uniform(0.5, 0.1), 0.0);
      }
    }
  }

  void draw_grid_1(Graphics &g) {
    // draw rectangle

    for (int x = -30; x < 31; x += 4) {
      for (int y = -30; y < 31; y += 4) {
        Vec3f pos(x, y * sin(x), x * sin(x));
        Vec3f pos2(x * cos(x) * cos(x), y * sin(x), x);
        cross(g, pos, rnd::uniform(0.01, 0.1), 0.02);
        rect(g, pos2, 0.02, rnd::uniform(0.1, 0.2));
        g.color(RGB(1, 1, 1));
      }
    }

    // draw grid
    lineTexture.bind();
    // g.shader(lineShader);
    grid.color(RGB(0.5, 0.5, 0.5));
    g.pushMatrix();
    g.draw(grid);
    g.popMatrix();

    g.pushMatrix();
    g.rotate(90, Vec3f(0, 1, 0));
    g.draw(grid);
    g.popMatrix();

    g.pushMatrix();
    g.rotate(90, Vec3f(0, 0, 1));
    g.draw(grid);
    g.popMatrix();
    lineTexture.unbind();
  }

  void draw_grid_2(Graphics &g) {
    // draw rectangle

    for (int x = -30; x < 31; x += 4) {
      for (int y = -30; y < 31; y += 4) {
        Vec3f pos(x, y * sin(x), x * sin(x));
        Vec3f pos2(x * cos(x) * cos(x), y * sin(x), x);
        cross(g, pos, rnd::uniform(0.01, 0.1), 0.02);
        rect(g, pos2, 0.02, rnd::uniform(0.1, 0.2));
        g.color(RGB(1, 1, 1));
      }
    }

    // draw grid
    lineTexture.bind();
    // g.shader(lineShader);
    g.pushMatrix();
    g.draw(grid);
    g.popMatrix();
    g.pushMatrix();
    g.rotate(90, Vec3f(0, 1, 0));
    g.draw(grid);
    g.popMatrix();
    g.pushMatrix();
    g.rotate(90, Vec3f(0, 0, 1));
    g.draw(grid);
    g.popMatrix();
    lineTexture.unbind();
  }
};
