#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <cmath>

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
};

vector<point> readInput() {
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

vector<point> copypoints(std::vector<point> points) {
  std::vector<point> arr;
  for (int n = 0; n < points.size(); n = n+1) {
    arr.push_back(points.at(n));
  }
  return arr;
}

std::vector<point> swappoints(std::vector<point> points, int first, int second) {
  point p = points.at(first);
  points.at(first) = points.at(second);
  points.at(second) = p;
  return points;
}

int distanceBetween(point first, point second) {
  float xDistance = std::abs(first.x - second.x);
  float yDistance = std::abs(first.y - second.y);
  return (int) std::sqrt((xDistance*xDistance) + (yDistance*yDistance));
}

int getDistance(std::vector<point> points) {
  int distance = 0;
  for (int n = 0; n<points.size();n=n+1) {
    point fromPoint = points.at(n);
    point toPoint = n < points.size()-1 ? points.at(n+1) : points.at(0);
    distance = distance + distanceBetween(fromPoint, toPoint);
  }
  return distance;
}

bool acceptNewSolution(double temp, std::vector<point> newSolution, std::vector<point> oldSolution) {
  int newDistance = getDistance(newSolution);
  int oldDistance = getDistance(oldSolution);
  double acceptProb = exp((oldDistance - newDistance) / temp);
  double random = (double) rand() / (RAND_MAX);
  if (newDistance < oldDistance || acceptProb > random) {
    return true;
  }

  return false;
}

vector<point> twoOptSwap(std::vector<point> tour, int i, int k) {
  int size = tour.size();
  vector<point> newTour = copypoints(tour);

  // 2. take route[i] to route[k] and add them in reverse order to new_route
  int dec = 0;
  for ( int c = i; c <= k; ++c ) {
    newTour.at(c) = tour.at(k-dec);
    dec++;
  }

  // 3. take route[k+1] to end and add them in order to new_route
  for ( int c = k + 1; c < size; ++c ) {
    newTour.at(c) = tour.at(c);
  }
  return newTour;
}

std::vector<point> simulatedAnnealing(std::vector<point> points) {
  double temp = 15000;
  double coolingRate = 0.0003;
  int size = points.size();
  vector<point> solution = copypoints(points);
  vector<point> best = copypoints(points);
  while (temp > 1) {
    cout << getDistance(solution) << endl;
    std::vector<point> newSolution = copypoints(solution);
    newSolution = swappoints(newSolution, rand() % size, rand() % size);
    for (int i = 0; i < points.size(); i=i+1) {
      for (int k = i + 1; k < points.size(); k=k+1) {
        newSolution = twoOptSwap(newSolution, i, k);
        if (acceptNewSolution(temp, newSolution, solution)) {
          solution = newSolution;
        }
        if (getDistance(newSolution) < getDistance(best)) {
          best = copypoints(newSolution);
        }
      }
    }
    temp = temp*(1-coolingRate);
  }
  if (getDistance(solution) > getDistance(best)) {
    return best;
  }
  return solution;
}

void printPath(vector<point> path) {
  for(int i = 0; i < path.size(); i=i+1) {
    cout << path.at(i).index << endl;
  }
}

int main() {
  vector<point> points = readInput();
  vector<point> solution = simulatedAnnealing(points);
  cout << "häst: " << getDistance(solution) << endl;
  printPath(solution);
  return 0;
}
