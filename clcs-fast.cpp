#include <string>
#include <algorithm>
#include <iostream>
#include <limits>
#include <stack>

using namespace std;

struct GridPoint {
  GridPoint *parent;
  int cost, x,y;
};

struct forwardPath {
  forwardPath *child;
  int x,y;
};

forwardPath paths[2001];
int matrix[4001][2001];
int aLen, bLen;
string A, B;
GridPoint new_grid[4000][2000];

forwardPath * copyToPath(GridPoint *gp) {
  forwardPath *fp = NULL;
  fp->x = gp->x;
  fp->y = gp->y;
  return fp;
}

/* reverses path of GridPoints and converts it to
 forward path of forwardPaths */
forwardPath * reverseList(GridPoint *gp) {
  stack<GridPoint*> gpStack; 
  gpStack.push(gp);
  GridPoint *next = gp->parent;
  while (next->parent != NULL) {
    gpStack.push(next);
    next = next->parent; 
  }
  GridPoint *firstGP = gpStack.top();
  gpStack.pop();
  forwardPath *pathHead = copyToPath(firstGP);
  for (forwardPath *fp  = pathHead; !gpStack.empty(); fp = fp->child) {
    GridPoint *next_gp = gpStack.top(); 
    gpStack.pop();
    fp->child = copyToPath(next_gp);
  }
  return pathHead;
}

/* 
 initializes matrix and uses binary value to represent whether A and B match
 at specific spot 
 */
void initializeMatrix() {
  for (int i=0; i < (aLen*2); i++) {
    for (int j=0; j < bLen; j++) {
      if (A[i]==B[j]) {
        matrix[i][j]=1;
      } else { 
        matrix[i][j]=0;
      }
    }
  }
}

void initializePaths(forwardPath* lower, forwardPath *upper) {

  forwardPath *head = lower;
  for (int i = 0; i < bLen; i++) {
    head->x = 0;
    head->y = i;
    forwardPath *newChild=new forwardPath;
    head->child = newChild;
    head = head->child;
  }    
  for (int j = 1; j < aLen; j++) {
    head->x = j;
    head->y = bLen-1;
    forwardPath *newChild=new forwardPath;
    head->child = newChild;
    head = head->child;
  }

  
  forwardPath *shead = upper;
  for (int i = 0; i < aLen; i++) {
    shead->x = i;
    shead->y = 0;
    forwardPath *newChild=new forwardPath;
    shead->child = newChild;
    shead = shead->child;
  }    
  for (int j = 1; j < bLen; j++) {
    shead->x = bLen - 1;
    shead->y = j;
    forwardPath *newChild=new forwardPath;
    shead->child = newChild;
    shead = shead->child;
  }
}

/*
 modified dijkstra/BFS algorithm to find shortest path from each gridpoint
 */
forwardPath singleShortestPath(int start, forwardPath* lowerPath, forwardPath* upperPath) {

  if (lowerPath->x == -1) {
    initializePaths(lowerPath, upperPath);
  }
  
  //implement way to make sure you dont check beyond upper/lower bound path
  //find shortest path from this grid point using a shortest path algorithm for directed acyclic graph
  for (int i = start; i < aLen; i++) {
    for (int j = 0; i< bLen; j++) {
      GridPoint vertex;
      vertex.cost = numeric_limits<int>::max();
      vertex.parent = NULL;
      vertex.x = i;
      vertex.y = j;
      new_grid[i][j] = vertex;
    }
  }
  
  new_grid[start][0].cost = 0;

  
  forwardPath lower;
  forwardPath curr = *lowerPath;

  while (true) {
    cout << curr.x;
    if (curr.x == start) {
      cout << start << endl;
      exit(1);
      lower = curr;
      break;
    }
    if (curr.x == 1) exit(1);
    curr = *(curr.child);
  }

  exit(1);
  
  forwardPath upper = *upperPath;
  
  forwardPath head;
  head.x = upperPath->x;
  head.y = 0;
  head.child = upperPath;
  upper = head;
  

  
  for (int i = start; i < aLen; i++) {
    while (lower.x == i) {
      if (lower.child !=NULL)
        lower = *(lower.child);
      else {
        lower.y=B.length();
      }
    }
    while (upper.x==i) {
      upper = *(upper.child);
    }
    for (int j=upper.y; j<lower.y; j++) {
      
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
      if (j==aLen-1) 
        break;
    }
    if (lower.child !=NULL)
      lower=*(lower.child);
    else 
      lower.x=lower.x+1;
    
    if (upper.child !=NULL)
      upper=*(upper.child);
    
    
  }
  
  GridPoint shortestPath;
  shortestPath.cost = numeric_limits<int>::max();
  
  
  for (int j = 0; j < bLen; j++) {
    
    if (new_grid[aLen-1][j].cost < shortestPath.cost) 
      shortestPath = new_grid[aLen-1][j];
    
  }
  
  forwardPath *answer = reverseList(&shortestPath);

  return *answer;
}

/*
 161 Handout Pseudocode translated to c++.
 Fills paths array with shortest paths at each grid point. 
 */
void findShortestPaths(int lower, int upper) {
  if ((upper - 1) <= 1) return;
  int mid = (lower + upper) / 2;
  paths[mid] = singleShortestPath(mid, &paths[lower], &paths[upper]);
  findShortestPaths(lower, mid);
  findShortestPaths(mid, upper);
}

int getShortestPathLength() {
  int min = numeric_limits<int>::max();
  for (int i = 0; i < 2001; i++) {
    if (&paths[i] == NULL) break; 
    int length = 0;
    for (forwardPath *fp = &paths[i]; fp != NULL; fp = fp->child) {
      length++; 
    }
    if (length < min) {
      min = length;
    }
  }
  return min;
}

int main() {
  cin >> A >> B;
  aLen = (int) A.length();
  bLen = (int) B.length(); 
  initializeMatrix();
  for (int i = 0; i < 2001; i++) {
    forwardPath temp;
    temp.x = -1;
    paths[i] = temp;
  }
  findShortestPaths(aLen, bLen);
  cout << getShortestPathLength() << endl;
  return 0;
}

