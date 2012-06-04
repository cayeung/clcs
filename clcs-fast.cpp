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
  int cost, x,y;
};

forwardPath paths[2000];
int matrix[4000][2000];
int aLen, bLen;
string A, B;
GridPoint new_grid[4001][2001];

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
  if (next == NULL) {
    return copyToPath(gp);
  }
  int size=0;
  while (next != NULL) {
    size++;
    gpStack.push(next);
    next = next->parent; 
  }
  GridPoint *firstGP = gpStack.top();
  gpStack.pop();
  forwardPath *pathHead = copyToPath(firstGP);
  forwardPath *fp  = pathHead; 
  while (!gpStack.empty()) {
    GridPoint *next_gp = gpStack.top(); 
    gpStack.pop();
    fp->child = copyToPath(next_gp);
    fp = fp->child;
  }
  fp->child = NULL; //initialize last child to NULL
  pathHead->cost=size;
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

//if the lower path does not exist, build the path along the midline down to the end corner
void initializelower(forwardPath* lower, int start) {
  
  forwardPath *head = lower;
  for (int i = 0; i < bLen+1; i++) {
    head->x = start;
    head->y = i;
    forwardPath *newChild=new forwardPath;
    head->child = newChild;
    head = head->child;
  }    
  for (int j = start+1; j < aLen+start+1; j++) {
    head->x = j;
    head->y = bLen;
    forwardPath *newChild=new forwardPath;
    head->child = newChild;
    head = head->child;
  }
  
}

//if upper path does not exist, builds a path along the left boundary to the bottom
void initializeUpper(forwardPath *upper, int start) {
  forwardPath *node = upper;
  for (int k = start; k < aLen+start+1; k++) {
    node->x = k;
    node->y = 0;
    forwardPath *uchild =new forwardPath;
    node->child = uchild;
    node = node->child;
  }    
  for (int j = 1; j < bLen+1; j++) {
    node->x = aLen + start;
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
  int upperbound;
  int lowerbound;
  int lbound;
  int ubound;
  if (lowerPath->x == -1) {
    lowerbound=0;
  }
  //    initializelower(lowerPath, start);
  //  }
  if (upperPath->x ==-1) {
    upperbound=0;
  }
  //    initializeUpper(upperPath, start);
  //  }
  
  //implement way to make sure you dont check beyond upper/lower bound path
  //find shortest path from this grid point using a shortest path algorithm for directed acyclic graph
  for (int i = start; i < aLen+start+1; i++) {
    for (int j = 0; j< bLen+1; j++) {
      GridPoint vertex;
      vertex.cost = numeric_limits<int>::max();
      vertex.parent = NULL;
      vertex.x = i;
      vertex.y = j;
      new_grid[i][j] = vertex;
    }
  }
  
  new_grid[start][0].cost = 0;
  
  //traverses the linked list of the lower path to find the node in the path with the row start
  forwardPath l;
  forwardPath curr = *lowerPath;
  l=curr;
  if (curr.x!=-1){
    while (true) {
      if (curr.child == NULL || curr.x == start) {  //added first condition
        l = curr;
        break;
      }
      curr = *(curr.child); 
    }
  }
  
  //traverses the upper path and builds a dummy head 
  forwardPath upper = *upperPath;
  
  forwardPath trav;
  if (upper.x!=-1){
    trav.x = upperPath->x;
    trav.y = 0;
    trav.child = upperPath;
    upper = trav;
  }
  
  //iterates through all the vertices from the start to the end between the upper and lower bounds. calculates the cost of the adjacent nodes and if the cost is greater than the cost of the current node + 1, replaces the cost. 
  for (int i = start; i < aLen+start+1; i++) {
    if (l.x != -1){  //if you comment this out and comment in the other while loop, should run but not work
      
      while (l.child !=NULL){  
        if (l.child->x ==i)
          l=*(l.child);
      }
    }
    /*     while (l.x == i) { //infinite loop here with METROMINDANDMEGAMIND / MEGAMINDANDMETROMIND test
     if (l.child !=NULL)
     l = *(l.child);
     else {
     l.y=B.length();
     break; //added to attempt to break infinite loop
     }
     }*/
    if (upper.x!=-1){
      while (upper.x==i) {
        if (upper.child == NULL) break; //added
        upper = *(upper.child);
      }
    }
    if (lowerbound==0)
      lbound = bLen;
    else 
      lbound=l.y;
    if (upperbound==0)
      ubound = 0;
    else
      ubound=upper.y;
    for (int j=ubound; j<=lbound; j++) {
      if (new_grid[i+1][j].cost > new_grid[i][j].cost + 1) {
        new_grid[i+1][j].cost = new_grid[i][j].cost + 1;
        cout << "one" <<i+1 <<" "<< j <<" "<< new_grid[i+1][j].cost << endl;
        new_grid[i+1][j].parent = &new_grid[i][j];
      }
      if (new_grid[i][j+1].cost > new_grid[i][j].cost + 1) {
        new_grid[i][j+1].cost = new_grid[i][j].cost + 1;
        new_grid[i][j+1].parent = &new_grid[i][j];
        cout <<"two"<< i <<" "<<j+1<< " "<< new_grid[i][j+1].cost << endl;
      }
      if ((new_grid[i+1][j+1].cost > new_grid[i][j].cost + 1) && matrix[i][j] == 1) {
        new_grid[i+1][j+1].cost = new_grid[i][j].cost + 1;
        new_grid[i+1][j+1].parent = &new_grid[i][j];
        cout <<"three"<< i+1 <<" "<< j+1 <<" "<< new_grid[i+1][j+1].cost << endl;
      }
      if (j==bLen) 
        break;
    }
    if (l.x!=-1){
      if (l.child !=NULL)
        l=*(l.child);
      else 
        l.x=l.x+1;
    }
    if (upper.x!=-1){
      if (upper.child !=NULL)
        upper=*(upper.child);
    }
    
  }
  
  //finds the shortest path and returns the forward version (from source to destination)
  GridPoint shortestPath;
  shortestPath = new_grid[start+aLen][bLen];
  //cout << shortestPath.x << " " << shortestPath.y << endl;
  
  forwardPath *answer = reverseList(&shortestPath);
  cout << "----------" << endl;
  
  forwardPath *newHead = answer;
  /*
   while (answer->child != NULL) {
   cout << answer->x << " " << answer->y << endl;
   answer = answer->child;
   }
   exit(1); */
  
  return *newHead;
}

/*
 161 Handout Pseudocode translated to c++.
 Fills paths array with shortest paths at each grid point. 
 */
void findShortestPaths(int lower, int upper) {
  if ((upper - lower) <= 1){
    paths[lower]=singleShortestPath(lower, &paths[upper], &paths[lower]);
    return;
  }
  int mid = (lower + upper) / 2;
  paths[mid] = singleShortestPath(mid, &paths[lower], &paths[upper]);
  findShortestPaths(lower, mid);
  findShortestPaths(mid, upper);
}

int getShortestPathLength() {
  int min = numeric_limits<int>::max();
  for (int i = 0; i < aLen; i++) {
    //if (paths[i].x == -1) break; //check &paths[i] == NULL or this? if paths[0] or something in the middle doesnt get assigned, everything stops...
    if (paths[i].x != -1) {
      int length = paths[i].cost;
      if (length < min) {
        min = length;
      }
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

