#include <string>
#include <algorithm>
#include <iostream>
#include <limits>
#include <cstring>
#include <cstdio>

typedef struct path {
    path *prev;
    int x;
    int y;
} path;

typedef struct node {
    int length;
    int x;
    int y;
    path *cur_path;
} node;

typedef struct output {
    int *path_bound;
    int shortest;
} output;


class pqueue {
private:
    node **list;
    int max_elems;
    int get_parent(int current);
    int get_left(int current);
    int get_right(int current);
    int cmp_node(node *fst, node *snd);
    int count;
public:
    pqueue(int max);
    ~pqueue();

    void push(node *new_node);
    node *get_min();
    node *look_min();
    int has_items();

};

class dg_matrix {
private:
    char *stringm;
    char *stringn;
    int *graph_mat;
    int lenm;
    int lenn;
    int get_location(int x, int y);

public:

    dg_matrix(const char *first, const char *second);
    ~dg_matrix();

    bool has_diagonal(int x, int y);
    int get_width() { return lenn; }
    int get_height() { return lenm; }
    int num_nodes();

};

bool 
dg_matrix::has_diagonal(int x, int y)
{
    return graph_mat[get_location(x,y)];
}

int dg_matrix::num_nodes() { return lenm*lenn * 2; }

int pqueue::has_items() { return count; }

pqueue::pqueue(int max)
{
    max_elems = max;
    list = (node **)malloc(max * sizeof (node *));
    count = 0;
}

pqueue::~pqueue() 
{
    free(list);
}

// note: backwards on purpose, because we want lowest length first
int
pqueue::cmp_node(node* fst, node *snd)
{
    return snd->length - fst->length;
}

int pqueue::get_parent(int current) { return ((current-1)/2); }
int pqueue::get_left  (int current) { return ((current+1)*2 -1); }
int pqueue::get_right (int current) { return ((current+1)*2); }

void
pqueue::push(node *new_node)
{
    // put node in the list
    int child = count++;
    list[child] = new_node;

    int parent = get_parent(child);
    // bubble up...
    while (cmp_node(list[parent], list[child]) < 0) {
        node *temp = list[child];
        list[child] = list[parent];
        list[parent] = temp;
        child = parent;
        parent = get_parent(child);
    }
}

node 
*pqueue::get_min()
{
    node *result = list[0]; 
    list[0] = list[count--];
    int current = 0;
    int left = get_left(current);
    int right= get_right(current);

    while ((left < count && cmp_node(list[current], list[left])  <0 )||
           (right < count && cmp_node(list[current], list[right])<0) ) {
        int largest = left;
        if (cmp_node(list[left], list[right]) < 0) {
            largest = right;
        }
        node* temp;
        temp = list[current];
        list[current] = list[largest];
        list[largest] = temp;
        current = largest;
        left = get_left(current);
        right= get_right(current);

   }
   return result;


}

node 
*pqueue::look_min()
{
    return list[0];
}


int 
dg_matrix::get_location (int x, int y)
{
    return (x * lenn + y);
}

dg_matrix::dg_matrix(const char* first, const char* second)
{
    lenm = strlen(first);
    lenn = strlen(second);
    //allocate strings
    stringm = (char *)malloc (lenm *2 +1);
    stringn = (char *)malloc (lenn +1);
    //move strings over, double one of them
    sprintf(stringm, "%s%s", first, first);
    strcpy(stringn,second);

    lenm *=2;

    //allocate diagonal matrix
    graph_mat = (int *)malloc (2 * lenm * lenn * sizeof(int));

    // initialize last row/col
    for (int i = 0; i < lenn; ++i) graph_mat[get_location(lenm-1, i)] = 0;
    for (int i = 0; i < lenm; ++i) graph_mat[get_location(i, lenn-1)] = 0;

    for (int i = 0; i < lenm-1; ++i) {
        for (int j = 0; i < lenn-1; ++i) {
            if (stringm[i] == stringn[j]) graph_mat[get_location(i,j)] =1; 
            else graph_mat[get_location(i,j)] =0; 
        }
    }

}

dg_matrix::~dg_matrix() 
{
    free(stringm);
    free(stringn);
    free(graph_mat);
}

output *
construct_path (node *current, int width)
{
    path * cur_path = current->cur_path;

    output *result = (output *)malloc(sizeof(output));
    result->path_bound = (int*)malloc(width*sizeof(int));
    memset(result->path_bound, 0, width*sizeof(int));
    result->shortest = current->length;

    while(cur_path) {
        result->path_bound[cur_path->y] = cur_path->x;
        cur_path = cur_path->prev;
    }

    return result;


}

output *
single_shortest_path(dg_matrix *matrix, int offset, int *u_bound, int *l_bound)
{
    int width = matrix->get_width();
    int height = matrix->get_height();

    node *first = (node *)malloc(sizeof(node));
    first->cur_path = (path *)malloc(sizeof(path));
    first->x = offset;
    first->y = 0;
    first->length= 0;
    first->cur_path->prev = NULL;
    first->cur_path->x = offset;
    first->cur_path->y = 0;

    pqueue* node_list = new pqueue(matrix->num_nodes());

    node_list->push(first);

    while(node_list->has_items()) {
        node *current = node_list->get_min();

        int x = current->x;
        int y = current->y;

        if (current->x == offset+height/2-1 &&
            current->y == width-1) {
            output *s_path = construct_path(current, width);
            return s_path;
        }

        if (x + 1 <= l_bound[y] && x + 1 <= offset - 1 + height/2) {
            node *new_right = (node *)malloc (sizeof(node));
            new_right->cur_path = (path *)malloc(sizeof(path));
            new_right->x = x+1;
            new_right->y = y;
            new_right->cur_path->x = x+1;
            new_right->cur_path->y = y;
            new_right->cur_path->prev = current->cur_path;
            new_right->length = current->length + 1;

            node_list->push(new_right);
        }
        if (y + 1 <= u_bound[y] && y + 1 <= width -1) {
            node *new_right = (node *)malloc (sizeof(node));
            new_right->cur_path = (path *)malloc(sizeof(path));
            new_right->x = x;
            new_right->y = y+1;
            new_right->cur_path->x = x;
            new_right->cur_path->y = y+1;
            new_right->cur_path->prev = current->cur_path;

            node_list->push(new_right);
        }
        if (matrix->has_diagonal(x, y)) {
            node *new_right = (node *)malloc (sizeof(node));
            new_right->cur_path = (path *)malloc(sizeof(path));
            new_right->x = x+1;
            new_right->y = y+1;
            new_right->cur_path->x = x+1;
            new_right->cur_path->y = y+1;
            new_right->cur_path->prev = current->cur_path;

            node_list->push(new_right);
        }   
    }
    //something is wrong
    return NULL;
    
}

int 
main ()
{
    std::string A;
    std::string B;
    std::cin >> A >> B;
    dg_matrix *matrix = new dg_matrix(A.c_str(), B.c_str());
    pqueue *test = new pqueue(24);

    delete matrix;
    delete test;
  
  return 0;
>>>>>>> 30426522385e5080a150ada0c19489350a696efc
}
