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
int size;

inline void startTimer() {
  start = std::clock();
}

inline bool checkTimer() {
  double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
  return duration > (timeLimit-0.01);
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
  std::vector<point> arr(size);
  for (int n = 0; n < size; n = n+1) {
    arr.at(n) = points.at(n);
  }
  return arr;
}

inline std::vector<point> swappoints(std::vector<point> points, int first, int second) {
  point p = points.at(first);
  points.at(first) = points.at(second);
  points.at(second) = p;
  return points;
}

inline int calculateDistanceBetween(point first, point second) {
  float xDistance = std::abs(first.x - second.x);
  float yDistance = std::abs(first.y - second.y);
  return (int) std::sqrt((xDistance*xDistance) + (yDistance*yDistance));
}

inline int distanceBetween(point first, point second) {
  return distMatrix.at(first.index).at(second.index);
}

inline void calculateDistances(vector<point>& points) {
  for (int i = 0; i<size;i++) {
    vector<int> temp(size);
    for (int j = 0; j<size;j++) {
      temp.at(j) = calculateDistanceBetween(points.at(i), points.at(j));
    }
    distMatrix.push_back(temp);
  }
}

inline int getDistance(std::vector<point>& points) {
  int distance = 0;
  for (int n = 0; n<size;n=n+1) {
    point fromPoint = points.at(n);
    point toPoint = n < size-1 ? points.at(n+1) : points.at(0);
    distance = distance + distanceBetween(fromPoint, toPoint);
  }
  return distance;
}

inline bool acceptNewSolution(double temp, int newDistance, int oldDistance) {
  double acceptProb = exp((oldDistance - newDistance) / temp);
  double random = (double) rand() / (RAND_MAX);
  if (newDistance < oldDistance || acceptProb > random) {
    return true;
  }
  return false;
}

inline vector<point> twoOptSwap(std::vector<point>& tour, int i, int k) {
  if (checkTimer()) { return tour; }
  int size = tour.size();
  vector<point> newTour(size);
  for (int c = 0; c<i; c++) {
    newTour.at(c) = tour.at(c);
  }
  int dec = 0;
  for ( int c = i; c <= k; c++ ) {
    newTour.at(c) = tour.at(k-dec);
    dec++;
  }

  for ( int c = k + 1; c < size; c++ ) {
    newTour.at(c) = tour.at(c);
  }
  return newTour;
}

inline point getNearestNotVisitedNeighbour(vector<point>& points, vector<bool> &used, point p) {
  point best;
  int bestDistance = -1;
  int index = 0;
  for (int j = 0; j < size; j++) {
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

inline vector<point> generateGreedyPath(vector<point>& points, int startindex) {
  vector<bool> used(size, false);
  vector<point> greedyPath(size);
  used.at(startindex) = true;
  greedyPath.at(0) = points.at(startindex);
  if (startindex >= size) { return points; }
  for (int i = 1; i<size; i++) {
    greedyPath.at(i) = getNearestNotVisitedNeighbour(points, used, greedyPath.at(i-1));
  }
  return greedyPath;
}

inline bool twoOptShouldSwap(vector<point> points, int a, int b) {
  int curr = distMatrix.at(points.at(a).index).at(points.at(a+1).index) + distMatrix.at(points.at(b).index).at(points.at(b-1).index);
  int newD = distMatrix.at(points.at(a).index).at(points.at(b-1).index) + distMatrix.at(points.at(b).index).at(points.at(a+1).index);
  return newD > curr;
}

inline std::vector<point> twoOpt(std::vector<point> points) {
  bool madeImprovement = true;
  vector<point> temp = copypoints(points);
  vector<point> best = copypoints(points);
  int bestDist = -1;
  while (madeImprovement) {
    madeImprovement = !madeImprovement;
    for (int i = 0; i<size; i++){
      for (int j = i+1; j<size; j++) {
        if (checkTimer()) { return best; }

          temp = twoOptSwap(best, i, j);
          int dist = getDistance(temp);
          if (bestDist == -1 || dist < bestDist) {
            best = copypoints(temp);
            bestDist = dist;
            madeImprovement = true;
          }

      }
    }
  }
  return best;

  // int improve = 0;
  // double duration;
  // //double temp = 10000;
  // //double coolingRate = 0.0003;
  // vector<point> solution = copypoints(points);
  // int solutionDistance = getDistance(solution);
  // int newDistance;
  // while (improve < 1000) {
  //   start:
  //   std::vector<point> newSolution = copypoints(solution);
  //   for (int i = 0; i < points.size(); i=i+1) {
  //     for (int k = i + 1; k < points.size(); k=k+1) {
  //       if (checkTimer()) { return solution; }
  //       newSolution = twoOptSwap(newSolution, i, k);
  //       newDistance = getDistance(newSolution);
  //       if (newDistance < solutionDistance) {
  //         solution = newSolution;
  //         solutionDistance = newDistance;
  //         improve = 0;
  //         goto start;
  //       }
  //       //temp = temp*(1-coolingRate);
  //     }
  //   }
  //   improve ++;
  // }
  // return solution;
}

inline void printPath(vector<point> path) {
  for(int i = 0; i < path.size(); i=i+1) {
    cout << path.at(i).index << endl;
  }
}

inline vector<point> generateRandomGreedyPath(vector<point> points) {
  vector<point> best;
  vector<point> it;
  int bestDistance = -1;
  for (int i = 0; i < 25; i++) {
    if (checkTimer()) { return best; }
    it = generateGreedyPath(points, rand() % size);
    int dist = getDistance(it);
    if (dist < bestDistance || bestDistance == -1) {
      bestDistance = dist;
      best = copypoints(it);
    }
  }
  return best;
}

inline vector<point> generateRandomPath(vector<point> points) {
  int bestDistance = -1;
  vector<point> it;
  vector<point> best = generateRandomGreedyPath(points);
  auto rng = std::default_random_engine {};
  for (int i; i < 35; i++) {
    if (checkTimer()) { return best; }
    it = copypoints(points);
    std::shuffle(std::begin(it), std::end(it), rng);
    int dist = getDistance(it);
    if (bestDistance == -1 || bestDistance > dist) {
      best = copypoints(it);
      bestDistance = dist;
    }
  }
  return best;
}

vector<point> threeOptSwap2(vector<point> points, int a, int b, int c, int variant) {
  vector<point> temp = copypoints(points);

  for (int i = 0; i<=a; i++) {
    temp.at(i) = points.at(i);
  }
  for (int i = c+1; i<size; i++) {
    temp.at(i) = points.at(i);
  }
  int ba = b-a;
  int cb = c-b;
  if (variant == 0) {
    for (int i = 0; i<ba; i++) {
      temp.at(i+a+1) = points.at(b-i);
    }
    for (int i = 0; i<cb; i++) {
      temp.at(i+ba+1+a) = points.at(c-i);
    }
  } else if (variant == 1) {
    for (int i = 0; i<cb; i++) {
      temp.at(i+1+a) = points.at(b+1+i);
    }
    for (int i = 0; i<ba; i++) {
      temp.at(i+cb+1+a) = points.at(i+a+1);
    }
  } else if (variant == 2) {
    for (int i = 0; i<cb; i++) {
      temp.at(i+1+a) = points.at(b+1+i);
    }
    for (int i = 0; i<ba; i++) {
      temp.at(i+cb+1+a) = points.at(b-i);
    }
  } else if (variant == 3) {
    for (int i = 0; i<cb; i++) {
      temp.at(i+1+a) = points.at(c-i);
    }
    for (int i = 0; i<ba; i++) {
      temp.at(i+cb+1+a) = points.at(i+a+1);
    }
  } else if (variant == 4) {
    temp = twoOptSwap(temp, a, b);
  } else if (variant == 5) {
    temp = twoOptSwap(temp, b, c);
  } else if (variant == 6) {
    temp = twoOptSwap(temp, a, c);
  }
  return temp;
}

inline std::vector<point> threeOpt(std::vector<point> points) {
  bool madeImprovement = true;
  vector<point> temp = copypoints(points);
  vector<point> best = copypoints(points);
  int bestDist = -1;
  while (madeImprovement) {
    madeImprovement = !madeImprovement;
    for (int i = 0; i<size;i++){
      for (int j = i+1; j < size; j++) {
        for (int k = j+1; k < size; k++) {
          for (int var = 0; var <= 6; var++) {
            if (checkTimer()) { return best; }
            temp = threeOptSwap2(best, i, j, k, var);
            int dist = getDistance(temp);
            if (bestDist == -1 || dist < bestDist) {
              best = copypoints(temp);
              bestDist = dist;
              madeImprovement = true;
            }
          }
        }
      }
    }
  }
  return best;
}

std::vector<point> simulatedAnnealing(std::vector<point> points) {
  double temp = 15000;
  double coolingRate = 0.0003;
  double duration;
  vector<point> solution = copypoints(points);
  vector<point> best = copypoints(points);
  vector<point> newSolution;
  while (temp > 1 && !checkTimer()) {
    //int rand1 = rand() % size;
    //newSolution = threeOptSwap2(best, rand1, rand1+1, rand1+2, rand()%6);
    newSolution = swappoints(solution, rand() % size, rand() % size);
    if (acceptNewSolution(temp, getDistance(newSolution), getDistance(solution))) {
      solution = copypoints(newSolution);
    }
    if (getDistance(newSolution) < getDistance(best)) {
      best = copypoints(newSolution);
    }
    temp = temp*(1-coolingRate);
  }

  if (getDistance(solution) > getDistance(best)) {
    return best;
  }
  return solution;
}

inline vector<point> loopUntilTimeOut(vector<point> points) {
  double duration = 0;
  vector<point> best;
  vector<point> temp;
  int bestDistance = -1;
  while (!checkTimer()) {
    //temp = simulatedAnnealing(generateRandomPath(points));
    temp = twoOpt(generateRandomPath(points));
    int dist = getDistance(temp);
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
  size = points.size();
  calculateDistances(points);
  vector<point> greedy = generateRandomGreedyPath(points);
  vector<point> solution = simulatedAnnealing(greedy);
  solution = loopUntilTimeOut(solution);
  //cout << getDistance(solution) << endl;
  printPath(solution);
  return 0;
}
