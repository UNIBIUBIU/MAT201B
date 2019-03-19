// This sketch is for functions

float mapFunction(float value, float low, float high, float Low, float High) {
  return Low + (High - Low) * ((value - low) / (high - low));
}

void line(Graphics &g, Vec3f a, Vec3f b) {
  Mesh m{Mesh::LINES};
  for (int i = 0; i < 101; ++i) {
    m.vertex(a);
    m.vertex(b);
  }
  g.draw(m);
}

void circle(Graphics &g, Vec3f center, float radius) {
  Mesh m{Mesh::TRIANGLE_FAN};
  m.vertex(center);
  for (int i = 0; i < 101; ++i)
    m.vertex(center.x + radius * sin(M_2PI * i / 100),
             center.y + radius * cos(M_2PI * i / 100),
             center.z + radius * cos(M_2PI * i / 100));
  // g.color(HSV(rnd::uniform(1.0f), rnd::uniform(1.0f), rnd::uniform(1.0f)).v);
  g.draw(m);
}
void box(Graphics &g, Vec3f pos, float l, float h) {
  Mesh m{Mesh::LINES};
  m.vertex(pos.x - l / 2, pos.y + l / 2, pos.z - h / 2);
  m.vertex(pos.x + l / 2, pos.y + l / 2, pos.z - h / 2);

  m.vertex(pos.x + l / 2, pos.y + l / 2, pos.z - h / 2);
  m.vertex(pos.x + l / 2, pos.y - l / 2, pos.z - h / 2);

  m.vertex(pos.x + l / 2, pos.y - l / 2, pos.z - h / 2);
  m.vertex(pos.x - l / 2, pos.y - l / 2, pos.z - h / 2);

  m.vertex(pos.x - l / 2, pos.y - l / 2, pos.z - h / 2);
  m.vertex(pos.x - l / 2, pos.y + l / 2, pos.z - h / 2);

  m.vertex(pos.x + l / 2, pos.y + l / 2, pos.z - h / 2);
  m.vertex(pos.x + l / 2, pos.y + l / 2, pos.z + h / 2);

  m.vertex(pos.x + l / 2, pos.y - l / 2, pos.z - h / 2);
  m.vertex(pos.x + l / 2, pos.y - l / 2, pos.z + h / 2);

  m.vertex(pos.x - l / 2, pos.y - l / 2, pos.z - h / 2);
  m.vertex(pos.x - l / 2, pos.y - l / 2, pos.z + h / 2);

  m.vertex(pos.x - l / 2, pos.y + l / 2, pos.z - h / 2);
  m.vertex(pos.x - l / 2, pos.y + l / 2, pos.z + h / 2);

  m.vertex(pos.x - l / 2, pos.y + l / 2, pos.z + h / 2);
  m.vertex(pos.x + l / 2, pos.y + l / 2, pos.z + h / 2);

  m.vertex(pos.x + l / 2, pos.y + l / 2, pos.z + h / 2);
  m.vertex(pos.x + l / 2, pos.y - l / 2, pos.z + h / 2);

  m.vertex(pos.x + l / 2, pos.y - l / 2, pos.z + h / 2);
  m.vertex(pos.x - l / 2, pos.y - l / 2, pos.z + h / 2);

  m.vertex(pos.x - l / 2, pos.y - l / 2, pos.z + h / 2);
  m.vertex(pos.x - l / 2, pos.y + l / 2, pos.z + h / 2);
  g.draw(m);
}

void rect(Graphics &g, Vec3f pos, float width, float height) {
  Mesh m{Mesh::TRIANGLES};
  // triangle 1
  m.vertex(pos.x - width / 2, pos.y - height / 2, pos.z);
  m.vertex(pos.x - width / 2, pos.y + height / 2, pos.z);
  m.vertex(pos.x + width / 2, pos.y + height / 2, pos.z);
  // g.color(RGB(1, 1, 1));
  // triangle 2
  m.vertex(pos.x - width / 2, pos.y - height / 2, pos.z);
  m.vertex(pos.x + width / 2, pos.y - height / 2, pos.z);
  m.vertex(pos.x + width / 2, pos.y + height / 2, pos.z);
  // g.color(1);
  g.draw(m);
}

void dashedLine(Graphics &g, Vec3f a, Vec3f b) {
  Mesh verts{Mesh::LINES};
  const int n = 801;
  for (int i = 0; i < n; ++i) {
    float f = float(i) / (n - 1);
    Vec3f p;
    if (i == n - 1) {
      p.y = b.y;
    } else if (i == 0) {
      p.y = a.y;
    }
    if (a.x > b.x) {
      p.x = a.x - (a.x - b.x) * f;
    } else {
      p.x = a.x + (a.x - b.x) * f;
    }
    //
    if (a.y > b.y) {
      p.y = (a.y - (a.y - b.y) * f) * sin(f * M_PI * 2) * cos(f * M_PI) *
            sin(f * M_PI * 2) / 1.5;
    } else {
      p.y = (a.y + (a.y - b.y) * f) * sin(f * M_PI * 2) *
            cos(f * M_PI * 1 / 2) * sin(f * M_PI * 2) / 1.5;
    }
    //
    if (a.z > b.z) {
      p.z = a.z - (a.z - b.z) * f;
    } else {
      p.z = a.z + (a.z - b.z) * f;
    }
    verts.vertex(p);
  }
  g.draw(verts);
}

void cross(Graphics &g, Vec3f pos, float width, float height) {
  Mesh m{Mesh::TRIANGLES};
  // triangle 1
  m.vertex(pos.x - width / 2, pos.y - height / 2, pos.z);
  m.vertex(pos.x - width / 2, pos.y + height / 2, pos.z);
  m.vertex(pos.x + width / 2, pos.y + height / 2, pos.z);
  // g.color(RGB(1, 1, 1));
  // triangle 2
  m.vertex(pos.x - width / 2, pos.y - height / 2, pos.z);
  m.vertex(pos.x + width / 2, pos.y - height / 2, pos.z);
  m.vertex(pos.x + width / 2, pos.y + height / 2, pos.z);
  // triangle 3
  m.vertex(pos.x - height / 2, pos.y - width / 2, pos.z);
  m.vertex(pos.x - height / 2, pos.y + width / 2, pos.z);
  m.vertex(pos.x + height / 2, pos.y + width / 2, pos.z);
  // g.color(RGB(1, 1, 1));
  // triangle 4
  m.vertex(pos.x - height / 2, pos.y - width / 2, pos.z);
  m.vertex(pos.x + height / 2, pos.y - width / 2, pos.z);
  m.vertex(pos.x + height / 2, pos.y + width / 2, pos.z);
  // g.color(1);
  g.draw(m);
}
