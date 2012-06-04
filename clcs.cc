#include <string>
#include <algorithm>
#include <iostream>
#include <limits>
#include <stack>
#include <cstring>
#include <cstdio>

typedef struct node {
    int length;
    int x;
    int y;
    int lcs;
} node;

class pqueue {
private:
    node **list;
    int max_elems;
    int get_parent(int current);
    int cmp_node(node *fst, node *snd);
    int count;
public:
    pqueue(int max);
    ~pqueue();

    void push(node *new_node);
    node *get_min();
    node *look min();

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

    dg_matrix(string first, string second);
    ~dg_matrix();

    bool has_diagonal(int x, int y);
    int get_width();

};

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

int pqueue::get_parent(int current) { return (current/2) }

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

int dg_matrix::get_width (void) { return lenn; }

int 
dg_matrix::get_location (int x, int y)
{
    return (x * lenn + y);
}

dg_matrix::dg_matrix(string first, string second)
{
    lenm = first.length();
    lenn = second.length();
    //allocate strings
    stringm = (char *)malloc (len1 *2 +1);
    stringn = (char *)malloc (len2 +1);
    //move strings over, double one of them
    sprintf(stringm, "%s%s", first.c_str(), first.c_str());
    stringn = second.c_str();

    lenm *=2;

    //allocate diagonal matrix
    graph_mat = (int *)malloc (2 * len1 * len2 * sizeof(int));

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
