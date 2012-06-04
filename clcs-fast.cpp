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

forwardPath paths[2000];
int matrix[4001][2000];
int aLen, bLen;
string A, B;
GridPoint new_grid[4000][2000];

forwardPath * copyToPath(GridPoint *gp) {
  forwardPath *fp = new forwardPath;
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

void initializelower(forwardPath* lower, int start) {
  
  forwardPath *head = lower;
  for (int i = 0; i < bLen; i++) {
    head->x = start;
    head->y = i;
    forwardPath *newChild=new forwardPath;
    head->child = newChild;
    head = head->child;
  }    
  for (int j = start+1; j < aLen+start; j++) {
    head->x = j;
    head->y = bLen-1;
    forwardPath *newChild=new forwardPath;
    head->child = newChild;
    head = head->child;
  }
  
}

void initializeUpper(forwardPath *upper, int start) {
  forwardPath *node = upper;
  for (int k = start; k < aLen+start; k++) {
    node->x = k;
    node->y = 0;
    forwardPath *uchild =new forwardPath;
    node->child = uchild;
    node = node->child;
  }    
  for (int j = 1; j < bLen; j++) {
    node->x = bLen - 1;
    node->y = j;
    forwardPath *uchild =new forwardPath;
    node->child = uchild;
    node = node->child;
  }
  
}

/*
 modified dijkstra/BFS algorithm to find shortest path from each gridpoint
 */
forwardPath singleShortestPath(int start, forwardPath* lowerPath, forwardPath* upperPath) {
  
  if (lowerPath->x == -1) {
    initializelower(lowerPath, start);
  }
  if (upperPath->x ==-1) {
    initializeUpper(upperPath, start);
  }

  //implement way to make sure you dont check beyond upper/lower bound path
  //find shortest path from this grid point using a shortest path algorithm for directed acyclic graph
  for (int i = start; i < aLen+start; i++) {
    for (int j = 0; j< bLen; j++) {
      GridPoint vertex;
      vertex.cost = numeric_limits<int>::max();
      vertex.parent = NULL;
      vertex.x = i;
      vertex.y = j;
      new_grid[i][j] = vertex;
    }
  }

  new_grid[start][0].cost = 0;
  
  forwardPath l;
  forwardPath curr = *lowerPath;
  
  //cout << start << endl;
  
  while (true) {
    if (curr.x == start) {
      l = curr;
      break;
    }
    curr = *(curr.child);
  }
  
  forwardPath upper = *upperPath;
  
  forwardPath trav;
  trav.x = upperPath->x;
  trav.y = 0;
  trav.child = upperPath;
  upper = trav;

  
  for (int i = start; i < aLen+start; i++) {
    while (l.x == i) {
      if (l.child !=NULL)
        l = *(l.child);
      else {
        l.y=B.length();
      }
    }
    while (upper.x==i) {
      upper = *(upper.child);
    }
    for (int j=upper.y; j<l.y; j++) {
      cout << "j is " << j << endl;
      if (new_grid[i+1][j].cost > new_grid[i][j].cost + 1) {
        new_grid[i+1][j].cost = new_grid[i][j].cost + 1;
        cout << "one" <<i+1 << j << new_grid[i+1][j].cost << endl;
        new_grid[i+1][j].parent = &new_grid[i][j];
      }
      if (new_grid[i][j+1].cost > new_grid[i][j].cost + 1) {
        new_grid[i][j+1].cost = new_grid[i][j].cost + 1;
        new_grid[i][j+1].parent = &new_grid[i][j];
        cout <<"two"<< i << j+1 << new_grid[i][j+1].cost << endl;
      }
      if ((new_grid[i+1][j+1].cost > new_grid[i][j].cost + 1) && matrix[i+1][j+1] == 1) {
        new_grid[i+1][j+1].cost = new_grid[i][j].cost + 1;
        new_grid[i+1][j+1].parent = &new_grid[i][j];
        cout <<"three"<< i+1 << j+1 << new_grid[i+1][j+1].cost << endl;
      }
      if (j==bLen-1) 
        break;
    }
    if (l.child !=NULL)
      l=*(l.child);
    else 
      l.x=l.x+1;
    
    if (upper.child !=NULL)
      upper=*(upper.child);
   
    cout << "i = " << i;
    
  }
 
  
  GridPoint shortestPath;
  shortestPath = new_grid[start+aLen-1][bLen-1];
  cout << aLen+start << " " << bLen;
  exit(1);
  
  
  cout << endl<<shortestPath.x << shortestPath.y << endl;
  exit(1);
  
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
  for (int i = 0; i < 2000; i++) {
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
  for (int i = 0; i < 2000; i++) {
    forwardPath temp;
    temp.x = -1;
    paths[i] = temp;
  }
  findShortestPaths(0, aLen);
  cout << getShortestPathLength() << endl;
  return 0;
}

