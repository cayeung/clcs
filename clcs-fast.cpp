#include <string>
#include <algorithm>
#include <iostream>
#include <limits>

using namespace std;

struct GridPoint {
  GridPoint *parent;
  int cost, x,y;
};

struct forwardPath {
  GridPoint *child;
  int x,y;
}

GridPoint paths[2000];
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
GridPoint singleShortestPath(int start, GridPoint lowerPath, GridPoint upperPath) {
  GridPoint new_grid[4000][2000];
  //implement way to make sure you dont check beyond upper/lower bound path
  //find shortest path from this grid point using a shortest path algorithm for directed acyclic graph
  for (int i = start; i < A.length(); i++) {
    for (int j = 0; i<B.length(); j++) {
      GridPoint vertex;
      vertex.cost = numeric_limits<int>::max();
      vertex.parent = NULL;
      vertex.x = i;
      vertex.y = j;
      new_grid[i][j] = vertex;
    }
  }
  
  new_grid[start][0].cost = 0;
  
  GridPoint lower;
  while (curr != NULL) {
    GridPoint curr = lowerPath;
    if (curr.x = start) 
      lower = curr;
  }
  GridPoint upper = upperPath;
  if (upper.x != start) {
    GridPoint head;
    head.x = 0;
    head.next = upperPath;
    upper = head;
  }
  
  for (int i = start; i < ; i++) {
    for (int j = 0; j < lower.y; j++) {
      if (new_grid[i+1][j].cost > new_grid[i][j].cost + matrix[i+1][j]) {
        new_grid[i+1][j].cost = new_grid[i][j].cost + matrix[i+1][j];
        new_grid[i+1][j].parent = &new_grid[i][j];
      }
      if (new_grid[i][j+1].cost > new_grid[i][j].cost + matrix[i][j+1]) {
        new_grid[i][j+1].cost = new_grid[i][j].cost + matrix[i][j+1];
        new_grid[i][j+1].parent = &new_grid[i][j];
      }
      if (new_grid[i+1][j+1].cost > new_grid[i+1][j+1].cost + matrix[i+1][j+1]) {
        new_grid[i+1][j+1].cost = new_grid[i+1][j+1].cost + matrix[i+1][j+1];
        new_grid[i+1][j+1].parent = &new_grid[i][j];
      }
      
    }
    
    while (lower.x = i) {
      lower = lower.child;
    }

  }
  
  GridPoint shortestPath;
  shortestPath.cost = numeric_limits<int>::max();

  for (int j = 0; j < B.length(); j++) {
    
    if (new_grid[A.length()-1][j].cost < shortestPath.cost) 
      shortestPath = new_grid[A.length()-1][j];
    
  }
  
  
  return shortestPath;
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
    //iterate through array and find cheapest path
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
