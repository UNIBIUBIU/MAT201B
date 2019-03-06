// this sketch the photogrammetry point data

string fullPathOrDie(string fileName, string whereToLook = ".") {
  SearchPaths searchPaths;
  searchPaths.addSearchPath(whereToLook);
  string filePath = searchPaths.find(fileName).filepath();
  if (filePath == "") {
    fprintf(stderr, "FAIL\n");
    exit(1);
  }
  return filePath;
}
// load CSV data
void loadCSV(string fileName, vector<vector<float>> &data) {
  ifstream f1(fileName, ios::in);
  //ifstream f1(fullPathOrDie(fileName, ".."), ios::in);
  if (!f1.is_open()) {
    cout << "file not open" << endl;
    exit(1);
  }
  string line1;
  while (getline(f1, line1)) {
    data.push_back(vector<float>());
    stringstream stream(line1);
    float v;
    while (stream >> v) {
      data.back().push_back(v);
      if (stream.peek() == ',')
        stream.ignore();
    }
  }
  f1.close();
}

struct PointCloud {
  float scale = 10;
  float r = 4.0f;
  Mesh pointClouds_Mesh{Mesh::LINES};
  vector<vector<float>> data1;

  PointCloud() { loadCSV("../asset/tree.csv", data1); }

  void setup() {
    // arrange photogrammetry points position
    for (int row = 0; row < data1.size(); row++) {
      for (int column = 0; column < data1[row].size(); column++) {
        Vec3f p(data1[row][0] * scale, data1[row][1] * scale,
                data1[row][2] * scale);
        // triangle 1
        float w = rnd::uniform(0.001 * scale, 0.005 * scale);
        float h = rnd::uniform(0.001 * scale, 0.005 * scale);
        pointClouds_Mesh.vertex(p.x, p.y, p.z);
        pointClouds_Mesh.vertex(p.x, p.y + h * sin(row), p.z);
        pointClouds_Mesh.vertex(p.x + w * cos(row) * cos(row),
                                p.y + h * sin(row), p.z);
        // triangle 2
        pointClouds_Mesh.vertex(p.x, p.y, p.z);
        pointClouds_Mesh.vertex(p.x + w, p.y, p.z);
        pointClouds_Mesh.vertex(p.x + w, p.y + h * cos(row) * cos(row), p.z);
      }
    }
  }

  void draw(Graphics &g) {
    // draw mesh
    g.pushMatrix();
    g.meshColor();
    g.translate(0, 0, -10);
    g.color(RGB((0.9f), (0.9f), (0.9f)));
    g.draw(pointClouds_Mesh);
    g.popMatrix();
    // draw 2nd mesh [moving]
    g.pushMatrix();
    g.translate(0, 0, -20 + r);
    g.draw(pointClouds_Mesh);
    g.popMatrix();
  }

  void update(double &dt) { r += dt / 2; }
};
