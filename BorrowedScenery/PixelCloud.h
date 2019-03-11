// This sketch is for loading image pixels and animating them

struct PixelCloud {
  vector<Vec3f> pos[16]; //
  Mesh mesh1{Mesh::POINTS};
  Mesh mesh2{Mesh::POINTS};
  Mesh mesh3{Mesh::POINTS};
  Mesh mesh4{Mesh::POINTS};
  Mesh mesh5{Mesh::POINTS};
  float timestep = 2;
  float time = 0;
  float time2 = 0;
  float time3 = 0;
  float angle = 0;
  // load images
  PixelCloud() {
    Image image_1;
    const char *fileName1 = "../asset/flower.jpg";
    Image image_2;
    const char *fileName2 = "../asset/characters.png";
    Image image_3;
    const char *fileName3 = "../asset/cactus.jpg";
    Image image_4;
    const char *fileName4 = "../asset/mountains.jpg";
    Image image_5;
    const char *fileName5 = "../asset/section3.png";

    if (!image_1.load(fileName1)) {
      exit(1);
    }
    if (!image_2.load(fileName2)) {
      exit(1);
    }
    if (!image_3.load(fileName3)) {
      exit(1);
    }
    if (!image_4.load(fileName4)) {
      exit(1);
    }
    if (!image_5.load(fileName5)) {
      exit(1);
    }

    // image1 pixel arrangement
    Image::RGBAPix<uint8_t> pixel;
    for (int row = 0; row < image_1.height(); row++) {
      for (int column = 0; column < image_1.width(); column++) {
        image_1.read(pixel, column, row);
        Vec3f o;
        o.x = mapFunction(column, 0, image_1.width(), -1, 1);
        o.y = mapFunction(row, 0, image_1.height(), -1, 1);
        o.z = 0;
        pos[0].push_back(o);

        // HSV
        Vec3f h;
        HSV color(RGB(pixel.r, pixel.g, pixel.b));
        h.x = color.s * sin(M_2PI * color.h);
        h.y = color.s * cos(M_2PI * color.h);
        h.z = color.v / 255;
        pos[1].push_back(h);

        mesh1.vertex(o);
        float gray =
            HSV(RGB(pixel.r / 255.0, pixel.g / 255.0, pixel.b / 255.0)).v;
        gray = mapFunction(gray, 0, 1, 0, 1.2);
        mesh1.color(Color(gray));
      }
    }
    // image2 pixel arrangement
    for (int row = 0; row < image_2.height(); row++) {
      for (int column = 0; column < image_2.width(); column++) {
        image_2.read(pixel, column, row);
        Vec3f o2;
        o2.x = mapFunction(column, 0, image_2.width(), -1, 1);
        o2.y = mapFunction(row, 0, image_2.height(), -1, 1);
        o2.z = 0;
        pos[3].push_back(o2);

        // HSV
        Vec3f h2;
        HSV color(RGB(pixel.r, pixel.g, pixel.b));
        h2.x = color.s * sin(M_2PI * color.h);
        h2.y = color.s * cos(M_2PI * color.h) * cos(M_2PI * color.h);
        h2.z = color.v / 255;
        pos[4].push_back(h2);
        mesh2.vertex(o2);

        float gray =
            HSV(RGB(pixel.r / 255.0, pixel.g / 255.0, pixel.b / 255.0)).v;
        gray = mapFunction(gray, 0, 1, 0, 1.2);
        mesh2.color(Color(gray));
      }
    }

    // image3 pixel arrangement
    for (int row = 0; row < image_3.height(); row += 2) {
      for (int column = 0; column < image_3.width(); column += 2) {
        image_3.read(pixel, column, row);
        Vec3f o3;
        o3.x = mapFunction(column, 0, image_3.width(), -1, 1);
        o3.y = mapFunction(row, 0, image_3.height(), -1, 1);
        o3.z = 0;
        pos[5].push_back(o3);

        // HSV
        Vec3f h3;
        HSV color(RGB(pixel.r, pixel.g, pixel.b));
        h3.x = color.s * sin(M_2PI * color.h);
        h3.y = color.s * cos(M_2PI * color.h);
        h3.z = color.v / 255;
        pos[6].push_back(h3);
        mesh3.vertex(o3);

        float gray =
            HSV(RGB(pixel.r / 255.0, pixel.g / 255.0, pixel.b / 255.0)).v;
        gray = mapFunction(gray, 0, 1, 0, 1.2);
        mesh3.color(Color(gray));
      }
    }

    // image4 pixel arrangement
    for (int row = 0; row < image_4.height(); row += 2) {
      for (int column = 0; column < image_4.width(); column += 2) {
        image_4.read(pixel, column, row);
        Vec3f o4;
        o4.x = mapFunction(column, 0, image_4.width(), -1, 1);
        o4.y = mapFunction(row, 0, image_4.height(), -1, 1);
        o4.z = 0;
        pos[7].push_back(o4);

        // HSV
        Vec3f h4;
        HSV color(RGB(pixel.r, pixel.g, pixel.b));
        h4.x = color.s * sin(M_2PI * color.h);
        h4.y = color.s * cos(M_2PI * color.h);
        h4.z = color.v / 255;
        pos[8].push_back(h4);
        mesh4.vertex(o4);

        float gray =
            HSV(RGB(pixel.r / 255.0, pixel.g / 255.0, pixel.b / 255.0)).v;
        gray = mapFunction(gray, 0, 1, 0, 1.2);
        mesh4.color(Color(gray));
      }
    }

    // image5 pixel arrangement
    for (int row = 0; row < image_5.height(); row += 3) {
      for (int column = 0; column < image_5.width(); column++) {
        image_5.read(pixel, column, row);
        Vec3f o5;
        o5.x = mapFunction(column, 0, image_5.width(), -2, 2);
        o5.y = mapFunction(row, 0, image_5.height(), -1, 1);
        o5.z = 0;
        pos[9].push_back(o5);

        // HSV
        Vec3f h5;
        HSV color(RGB(pixel.r, pixel.g, pixel.b));
        h5.x = color.s * sin(M_2PI * color.h);
        h5.y = color.s * cos(M_2PI * color.h);
        h5.z = color.v / 255;
        pos[10].push_back(h5);
        mesh5.vertex(o5);

        float gray =
            HSV(RGB(pixel.r / 255.0, pixel.g / 255.0, pixel.b / 255.0)).v;
        gray = mapFunction(gray, 0, 1, 0, 1.2);
        mesh5.color(Color(gray));
        //mesh5.color(RGB(pixel.r / 255.0, pixel.g / 255.0, pixel.b / 255.0));
      }
    }
  }

  // image pixel animation
  void section1_update(double dt) {
    auto &m = mesh1.vertices();
    auto &m2 = mesh2.vertices();
    auto &m3 = mesh3.vertices();
    time += dt / timestep;

    for (int i = 0; i < m.size(); i++) {
      if (time < 10) {
        timestep = 2;
        m[i] = pos[0][i] * (10 - time) + pos[1][i] * time;
      } else {
        m[i] = pos[1][i] * (20 - time) + pos[0][i] * (time - 10);
      }
    }
    for (int i = 0; i < m2.size(); i++) {
      if (time < 10) {
        m2[i] = pos[3][i] * (10 - time) + pos[4][i] * time;
      } else {
        timestep = 0.05;
        m2[i] = pos[4][i] * (20 - time) + pos[3][i] * (time - 10);
      }
    }

    for (int i = 0; i < m3.size(); i++) {
      if (time < 10) {
        m3[i] = pos[5][i] * (10 - time) + pos[6][i] * time;
      } else {
        timestep = 0.05;
        m3[i] = pos[6][i] * (20 - time) + pos[5][i] * (time - 10);
      }
    }
  }

  void section2_update(double &dt) {
    // image 4
    auto &m4 = mesh4.vertices();
    time2 += dt / timestep;
    for (int i = 0; i < m4.size(); i++) {
      if (time2 < 10) {
        timestep = 1;
        m4[i] = pos[7][i] * (10 - time2) + pos[8][i] * time2;
      } else {
        timestep = 0.05;
        m4[i] = pos[8][i] * (20 - time2) + pos[7][i] * (time2 - 10);
      }
    }
  }

  void section3_update(double &dt) {
    angle += dt;
    // image 5
    auto &m5 = mesh5.vertices();
    time3 += dt / timestep;
    for (int i = 0; i < m5.size(); i++) {
      if (time3 < 15) {
        timestep = 5;
        m5[i] = pos[9][i] * (10 - time3) + pos[10][i] * time3;
      } else {
        timestep = 0.05;
        m5[i] = pos[10][i] * (30 - time3) + pos[9][i] * (time3 - 15);
      }
    }
  }

  void section1_draw(Graphics &g) {
    // draw images
    g.meshColor();
    g.pointSize(2);
    g.pushMatrix();
    g.translate(0, 0, -25);
    // g.rotate(40, Vec3f(1, 0, 1));
    g.draw(mesh1); // img1
    g.popMatrix();

    g.pushMatrix();
    for (int i = 0; i < 2; i++) {
      g.rotate(45 * i, Vec3f(0, 1, 0));
      g.translate(0, 0, 25);
      g.draw(mesh2); // img2
    }
    g.popMatrix();

    g.pushMatrix();
    g.translate(0, 0, 35);
    g.draw(mesh3); // img2
    g.color(RGB((1.0f), (1.0f), (1.0f)));

    //-------------------c-o-n-n-e-c-t-o-r-s----------------------------
    auto &m2 = mesh2.vertices();
    auto &m3 = mesh3.vertices();
    auto &m = mesh1.vertices();

    // connecting image 1 with image 2
    for (int i = 0; i < m2.size(); i += 50000) {
      for (int j = 0; j < m.size(); j += 80000) {
        if ((m2[i] - m[j]).mag() < 14.0f) {
          dashedLine(g, m2[i], Vec3f(m[j].x, m[j].y, m[j].z - 60));
          cross(g, Vec3f(m[j].x, m[j].y, m[j].z - 60), 0.4, 0.1);
          cross(g, m2[i], 0.2, 0.06);
        }
      }
    }
    // connecting image 3 with image 1
    for (int i = 0; i < m3.size(); i += 90000) {
      for (int j = 0; j < m.size(); j += 80000) {
        if ((m3[i] - m[j]).mag() < 14.0f) {
          dashedLine(g, m3[i], Vec3f(m[j].x, m[j].y, m[j].z - 60));
          cross(g, Vec3f(m[j].x, m[j].y, m[j].z - 60), 0.4, 0.1);
          cross(g, m3[i], 0.2, 0.06);
        }
      }
    }
    g.popMatrix();
  }

  void section2_draw(Graphics &g) {
    g.meshColor();
    g.pushMatrix();
    g.pointSize(2.5);
    g.translate(0, 0, -20);
    g.draw(mesh4); // img4
    g.popMatrix();
  }

  void section3_draw(Graphics &g) {
    g.pushMatrix();
    g.rotate(angle, Vec3f(1, 0, 1));

    g.meshColor();
    g.pointSize(2);
    g.pushMatrix();
    g.translate(0, 0, -20);
    g.draw(mesh5); // img5
    g.popMatrix();
    g.pushMatrix();
    g.translate(0, 0, 40);
    g.draw(mesh5);
    g.popMatrix();

    // draw cross
    auto &m5 = mesh5.vertices();
    for (int i = 0; i < m5.size(); i += 30000) {
      g.color(1);
      dashedLine(g, Vec3f(m5[i].x, m5[i].y, m5[i].z + 40),
                 Vec3f(m5[i].x, m5[i].y, m5[i].z - 20));
      g.color(RGB(1, 0, 0));
      g.rotate(45, Vec3f(0, 1, 0));
      cross(g, Vec3f(m5[i].x, m5[i].y, m5[i].z - 20), 0.35, 0.05);
      cross(g, Vec3f(m5[i].x, m5[i].y, m5[i].z + 40), 0.35, 0.05);
    }
    g.popMatrix();
  }

  // reset
  void reset(double &dt) { time = 0; }
};
