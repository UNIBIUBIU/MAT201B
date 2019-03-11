// this sketch is to set up animation process
// opening
float opening_start = 0;
float opening_end = 18;
float opening_pointcloud = 6;

// transition
float transition_start = 16.5;
float transition_end = 21;

// mover particle system
float movers_start = 21;
float movers_end = 250;

// section1
float section1_start = 21;
float section1_end = 40;
float blink1 = section1_start + 0.5;
float blink2 = section1_end - 0.5;
float grid_start = section1_start + 5;

// section2
float section2_start = 40;
float section2_end = 60;
float spiral_start = section2_start + 8;
float spiral_end = section2_start + 10;

// section3
float section3_start = 60;
float section3_end = 130;

// section4
float section4_start = 130;
float section4_end = 150;

// ending
float ending_start = 150;
float ending_end = 180;

// model scale animation
float scalestep = 6;

// nav animation
Vec3f nav_current(0, 0, 19);

// set up keynotes for the Animation
struct AnimationProcess {
  double start{0};
  double stop{1};
  double real_t{0}; // (0, 1)

  bool operator()(double time) { return active(time); }
  bool active(double time) {
    real_t = mapFunction(time, start, stop, 0, 1);
    return time > start && time < stop;
  }

  bool done(double time) { return time > stop; }

  void schedule(double start, double stop) {
    this->start = start;
    this->stop = stop;
    real_t = 0;
  }
};

struct MeshAnimation : AnimationProcess {
  Mesh mesh;
};

struct Animation {
  MeshAnimation opening;
  MeshAnimation transition;
  MeshAnimation movers;
  MeshAnimation section1;
  MeshAnimation section2;
  MeshAnimation section3;
  MeshAnimation section4;
  MeshAnimation ending;

  void scheduleAnimation() {
    opening.schedule(opening_start, opening_end);
    transition.schedule(transition_start, transition_end);
    movers.schedule(movers_start, movers_end);
    section1.schedule(section1_start, section1_end);
    section2.schedule(section2_start, section2_end);
    section3.schedule(section3_start, section3_end);
    section4.schedule(section4_start, section4_end);
    ending.schedule(ending_start,ending_end);
  }
};
