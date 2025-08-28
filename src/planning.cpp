#include "planning.h"
#include <cmath>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

using namespace std;
Planner::Planner(const vector<vector<bool>> &grid) : grid(grid) {
    rows = grid.size();
    cols = grid[0].size();
}
bool Planner::isvalid(int x, int y) const {
    return (x >= 0 && x < rows && y >= 0 && y < cols && !grid[x][y]);
}
double Planner::heuristic(int x1, int y1, int x2, int y2) const {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

vector<pair<int, int>> Planner::pathplanning(pair<int, int> start,
                                             pair<int, int> goal) {
  )
    vector<pair<int, int>> directions = {{1,0},{-1,0},{0,1},{0,-1}};

    struct Node {
        int x, y;
        double f, g;
        bool operator>(const Node& other) const { return f > other.f; }
    };

    priority_queue<Node, vector<Node>, greater<Node>> openSet;
    unordered_map<int, pair<int,int>> cameFrom; // track parent node
    unordered_map<int, double> gScore;

    auto key = [&](int x, int y) { return x * cols + y; };
    gScore[key(start.first, start.second)] = 0.0;
    openSet.push({start.first, start.second,
                  heuristic(start.first, start.second, goal.first, goal.second),
                  0.0});

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();
        if (current.x == goal.first && current.y == goal.second) {
            vector<pair<int, int>> path;
            int cx = goal.first, cy = goal.second;
            while (!(cx == start.first && cy == start.second)) {
                path.push_back({cx, cy});
                auto p = cameFrom[key(cx, cy)];
                cx = p.first;
                cy = p.second;
            }
            path.push_back(start);
            reverse(path.begin(), path.end());
            return path;
        }
        for (auto [dx, dy] : directions) {
            int nx = current.x + dx;
            int ny = current.y + dy;
            if (!isvalid(nx, ny)) continue;

            double tentative_g = current.g + 1.0; // cost per step = 1
            int nk = key(nx, ny);

            if (!gScore.count(nk) || tentative_g < gScore[nk]) {
                gScore[nk] = tentative_g;
                double fScore = tentative_g + heuristic(nx, ny, goal.first, goal.second);
                openSet.push({nx, ny, fScore, tentative_g});
                cameFrom[nk] = {current.x, current.y};
            }
        }
    }
    return {};
}
//Not able  to compile the code in gcc/g++
