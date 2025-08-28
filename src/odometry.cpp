#include "odometry.h"
#include <cmath>
#include <ctime>
#include <iterator>
#include <numeric>

using namespace std;

Odometry::Odometry(double wheel_radius, double rpm)
    : radius(wheel_radius), rpm(rpm) {
  
  double rps = rpm / 60.0;
  linear_vel = 2 * M_PI * radius * rps;
}

double Odometry::distance(int x1, int y1, int x2, int y2) {
  return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

double Odometry::angle(int x1, int y1, int x2, int y2) {
  // atan2 returns radians, convert to degrees
  return atan2(y2 - y1, x2 - x1) * 180.0 / M_PI;
}

MotionCommand Odometry::computeCommands(vector<pair<int, int>> &path) {

  MotionCommand res = {0.0, 0.0}; // store total time and angle traversed
if (path.size() < 2) return res; 
    double prev_angle = angle(path[0].first, path[0].second,
                              path[1].first, path[1].second);

    for (size_t i = 1; i < path.size(); i++) {
        int x1 = path[i-1].first, y1 = path[i-1].second;
        int x2 = path[i].first, y2 = path[i].second;
        double d = distance(x1, y1, x2, y2);
        double travel_time = d / linear_vel; // time = distance / velocity
        res.total_time += travel_time;
        double curr_angle = angle(x1, y1, x2, y2);
        double turn = curr_angle - prev_angle;
        while (turn > 180) turn -= 360;
        while (turn < -180) turn += 360;

        res.total_angle += fabs(turn); // accumulate absolute turns
        prev_angle = curr_angle;
    }

    

  return res;
}
//could not compile the code in gcc/g++ compiler
