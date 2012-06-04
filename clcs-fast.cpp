#include <string>
#include <algorithm>
#include <iostream>
#include <limits>

using namespace std;

struct Gridpoint {
  Gridpoint *parent;
  int cost;
}

string paths[2000];
int matrix[4000][2000];
string A, B;

/*
not sure if we need to initialize this shit as instance variables
string A, B;
GridPoint grid[2000][4000];
string paths[2000];
*/

/* 
initializes matrix and uses binary value to represent whether A and B match
at specific spot 
*/
void initializeMatrix() {
/*
  int a_length = A.length();
  int b_length = B.length();
  int matrix[4000][2000] = new int[a_length][b_length]; */ // cant use new on 2d array
  for (int i=0; i< (A.length()*2); i++) {
    for (int j=0; j< (B.length()); j++) {
      if (A[i]==B[j]) {
	matrix[i][j]=1;
      } else { 
	matrix[i][j]=0;
      }
    }
  }
}

/*
  modified dijkstra/BFS algorithm to find shortest path from each gridpoint
*/
string singleShortestPath(int start, Gridpoint* lower, Gridpoint* upper) {
  GridPoint new_grid[4000][2000];
  //implement way to make sure you dont check beyond upper/lower bound path
  //find shortest path from this grid point using a shortest path algorithm for directed acyclic graph
  for (int i = start; i < A.length(); i++) {
    for (int j = 0; i<B.length(); j++) {
      Gridpoint vertex;
      vertex.cost = numeric_limits<int>::max();
      vertex.parent = NULL;
    }
  }

  new_grid[start][0].cost = 0;

  for (int i = start; i < A.length(); i++) {
    for (int j = 0; j < B.length(); j++) {

      

    }
  }
  

  return "";
}

/*
161 Handout Pseudocode translated to c++.
Fills paths array with shortest paths at each grid point. 
*/
void findShortestPaths(int lower, int upper) {
  if ((upper - 1) <= 1) return;
  int mid = (lower + upper) / 2;
  paths[mid] = singleShortestPath(mid, paths[lower], paths[upper]);
  findShortestPaths(lower, mid);
  findShortestPaths(mid, upper);
}

int getShortestPathLength() {
  int min = numeric_limits<int>::max();
  for (int i = 0; i < 2000; i++) {
    if (paths[i].empty()) break;
    int len = paths[i].length();
    if (len < min) {
      min = len;
    }
  }
  return min;
}

int main() {
  cin >> A >> B;
  initializeMatrix();
  cout << getShortestPathLength() << endl;
  return 0;
}

/* 
potential optimizations:
-if the number of diagonals that youve checked in your shortest path is already more than the number of diagonals left in your edge cases, then you can stop
-split as evenly as possible
-don't clear graph, just initialize a new one with nulls?
*/
