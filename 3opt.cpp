#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <random>

std::vector<std::vector<int> > distMatrix;
std::clock_t start;
double timeLimit = 2;

inline void startTimer() {
  start = std::clock();
}

inline bool checkTimer() {
  double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  return duration > (1.9);
}

using namespace std;
std::vector<std::string> split(std::string str,std::string sep){
    char* cstr=const_cast<char*>(str.c_str());
    char* current;
    std::vector<std::string> arr;
    current=strtok(cstr,sep.c_str());
    while(current!=NULL){
        arr.push_back(current);
        current=strtok(NULL,sep.c_str());
    }
    return arr;
}

struct point {
  double x;
  double y;
  int index;
  bool operator==(const point& other) {
    return (other.x == x && other.y == y);
  }
};

inline vector<point> readInput() {
  int a;
  cin >> a;
  std::string line;
  vector<point> points;
  getline(cin, line);
  for (int n = 0; n < a; n = n+1) {
    getline (cin, line);
    std::vector<std::string> arr;
    arr = split(line, " ");
    point p;
    p.x = atof(arr.at(0).c_str());
    p.y = atof(arr.at(1).c_str());
    p.index = n;
    points.push_back(p);

  }
  return points;
}

inline vector<point> copypoints(std::vector<point> points) {
  std::vector<point> arr;
  for (int n = 0; n < points.size(); n = n+1) {
    arr.push_back(points.at(n));
  }
  return arr;
}

inline int calculateDistanceBetween(point first, point second) {
  float xDistance = std::abs(first.x - second.x);
  float yDistance = std::abs(first.y - second.y);
  return (int) std::sqrt((xDistance*xDistance) + (yDistance*yDistance));
}

inline int distanceBetween(point first, point second) {
  return distMatrix.at(first.index).at(second.index);
}

inline void calculateDistances(vector<point> points) {
  for (int i = 0; i<points.size();i++) {
    vector<int> temp(0);
    for (int j = 0; j<points.size();j++) {
      temp.push_back(calculateDistanceBetween(points.at(i), points.at(j)));
    }
    distMatrix.push_back(temp);
  }
}

inline int getDistance(std::vector<point> points) {
  int distance = 0;
  for (int n = 0; n<points.size();n=n+1) {
    point fromPoint = points.at(n);
    point toPoint = n < points.size()-1 ? points.at(n+1) : points.at(0);
    distance = distance + distanceBetween(fromPoint, toPoint);
  }
  return distance;
}

inline vector<point> twoOptSwap(std::vector<point> tour, int i, int k) {
  int size = tour.size();
  vector<point> newTour = copypoints(tour);
  if (checkTimer()) { return newTour; }
  for (int c = 0; c<i; ++c) {
    newTour.at(c) = tour.at(c);
  }
  int dec = 0;
  for ( int c = i; c <= k; ++c ) {
    newTour.at(c) = tour.at(k-dec);
    dec++;
  }

  for ( int c = k + 1; c < size; ++c ) {
    newTour.at(c) = tour.at(c);
  }
  return newTour;
}

inline vector<point> threeOptSwap(std::vector<point> tour, int a, int b, int c, int variant) {
  int size = tour.size();
  vector<point> newTour = copypoints(tour);
  if (checkTimer()) { return newTour; }
  if (variant == 1) {
    newTour = twoOptSwap(tour, a, b);
    newTour = twoOptSwap(newTour, b, c);
  } else if (variant == 2) {
    newTour = twoOptSwap(tour, a, c);
    newTour = twoOptSwap(newTour, b, c);
  } else if (variant == 3) {
    newTour = twoOptSwap(tour, b, c);
    newTour = twoOptSwap(newTour, a, c);
  } else if (variant == 4) {
    newTour = twoOptSwap(tour, a, b);
    newTour = twoOptSwap(tour, a, c);
  } else if (variant == 5) {
    newTour = twoOptSwap(tour, a, c);
    newTour = twoOptSwap(tour, a, b);
  } else if (variant == 6) {
    newTour = twoOptSwap(tour, b, c);
    newTour = twoOptSwap(tour, a, b);
  }
  return newTour;
}

inline vector<point> getOptimalThreeOptSwap(std::vector<point> points, int a, int b, int c) {
  if (checkTimer()) { return points; }
  vector<point> temp;
  vector<point> best = copypoints(points);
  int bestDistance = -1;
  int dist;
  for (int i = 1; i<=2; i++) {
    temp = threeOptSwap(points, a, b, c, i);
    dist = getDistance(temp);
    if (checkTimer()) { return best; }
    if (bestDistance == -1 || dist < bestDistance) {
      bestDistance = dist;
      best = copypoints(temp);
    }
  }
  return best;
}

inline point getNearestNotVisitedNeighbour(vector<point> points, vector<bool> &used, point p) {
  point best;
  int bestDistance = -1;
  int index = 0;
  for (int j = 0; j < points.size(); j++) {
    if (j >= used.size()) continue;
    if (points.at(j) == p || used.at(j)) continue;
    int dist = distanceBetween(p, points.at(j));
    if (dist < bestDistance || bestDistance == -1) {
      bestDistance = dist;
      best = points.at(j);
      index = j;
    }
  }
  used.at(index) = true;
  return best;
}

inline vector<point> generateGreedyPath(vector<point> points, int startindex) {
  if (checkTimer()) { return points; }
  int size = points.size();
  vector<bool> used(size, false);
  vector<point> greedyPath;
  used.at(startindex) = true;
  greedyPath.push_back(points.at(startindex));
  point best;
  if (startindex >= size) { return points; }
  for (int i = 1; i<size; i++) {
    best = getNearestNotVisitedNeighbour(points, used, greedyPath.at(i-1));
    greedyPath.push_back(best);
  }
  return greedyPath;
}

inline std::vector<point> threeOpt(std::vector<point> points) {
  if (checkTimer()) { return points; }
  bool madeImprovement = true;
  vector<point> temp = copypoints(points);
  vector<point> best = copypoints(points);
  int bestDist = -1;
  int dist;
  while (madeImprovement) {
    madeImprovement = !madeImprovement;
    for (int a = 0; a<points.size();a++) {
      for (int b = a+1; b<points.size();b++) {
        for (int c = b+1; c<points.size();c++) {
          if (checkTimer()) { return best; }
          temp = getOptimalThreeOptSwap(best, a, b, c);
          dist = getDistance(temp);
          if (bestDist == -1 || dist < bestDist && dist != 0) {
            best = copypoints(temp);
            bestDist = dist;
            madeImprovement = true;
          }
        }
      }
    }
  }
  return best;
}

inline void printPath(vector<point> path) {
  for(int i = 0; i < path.size(); i=i+1) {
    cout << path.at(i).index << endl;
  }
}

inline vector<point> generateRandomGreedyPath(vector<point> points) {
  if (checkTimer()) { return points; }
  vector<point> best;
  vector<point> it;
  int bestDistance = -1;
  int dist;
  for (int i = 0; i < 25; i++) {
    if (checkTimer()) { return best; }
    it = generateGreedyPath(points, rand() % points.size());
    dist = getDistance(it);
    if (dist < bestDistance || bestDistance == -1) {
      bestDistance = dist;
      best = copypoints(it);
    }
  }
  return best;
}

inline vector<point> generateRandomPath(vector<point> points) {
  if (checkTimer()) { return points; }
  int bestDistance = -1;
  int dist;
  vector<point> it;
  vector<point> best = generateRandomGreedyPath(points);
  auto rng = std::default_random_engine {};
  for (int i; i < 25; i++) {
    if (checkTimer()) { return best; }
    it = copypoints(points);
    std::shuffle(std::begin(it), std::end(it), rng);
    dist = getDistance(it);
    if (bestDistance == -1 || bestDistance > dist) {
      best = copypoints(it);
      bestDistance = dist;
    }
  }
  return best;
}

inline vector<point> loopUntilTimeOut(vector<point> points) {
  double duration = 0;
  vector<point> best;
  vector<point> temp;
  int bestDistance = -1;
  int dist;
  while (!checkTimer()) {
    temp = threeOpt(generateRandomPath(points));
    dist = getDistance(temp);
    if (bestDistance == -1 || dist < bestDistance) {
      best = copypoints(temp);
      bestDistance = dist;
    }
  }
  return best;
}

int main() {
  startTimer();
  vector<point> points = readInput();
  calculateDistances(points);
  //vector<point> greedy = generateRandomGreedyPath(points);
  vector<point> solution = loopUntilTimeOut(points);
  //cout << getDistance(solution) << endl;
  printPath(solution);
  return 0;
}
