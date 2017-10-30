#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>

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
};

vector<point> readInput() {
  int a;
  cin >> a;
  std::string line;
  vector<point> points;
  for (int n = 0; n < a; n = n+1) {
    getline (cin, line);
    std::vector<std::string> arr;
    arr = split(line, " ");
    if (arr.size() > 0) {
      point p;
      p.x = atof(arr.at(0).c_str());
      p.y = atof(arr.at(1).c_str());
      points.push_back(p);
    }
  }
  return points;
}

int main() {
  vector<point> points = readInput();
  return 0;
}
