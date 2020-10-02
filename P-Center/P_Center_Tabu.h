//
// Created by Guan on 2020/9/28.
//

#ifndef P_CENTER_P_CENTER_TABU_H
#define P_CENTER_P_CENTER_TABU_H

#include "Graph.h"
#include <unordered_set>
#include <vector>
using std::unordered_set;

class PCenterTabu{
public:
    PCenterTabu(Graph g,int maxIter,int optimum);
    void tabuSearch();
private:
    int Sc;//the length of longest service edge
    vector<int> farmostUserVertices; //store the corresponding user vertices with the longest service edge
    int optimum; //the algorithm will stop when Sc reaches optimum
    int bestSc=INT_MAX;
    Graph g;
    int iter;
    int maxIter;
    struct most_second{
        int most;
        int second;
    };
    struct most_second* F_table;
    struct most_second* D_table;
    int ** N_table; //N_table[w] stores vertices from near to far from vertex w in order
    int * M_table; //M_table[f] stores the distance of the longest service edge generated after deleting the device node f
    int **tabuTenure;
    void addFacility(int vertex);
    void removeFacility(int vertex);
    void findNext(int vertex,int& D,int& F); //Find the second closest service vertex
    struct Move{
        int i;
        int j;
    }nextMove;
    void findMove();
    void makeMove();

    unordered_set<int> sol; //O(1) time complexity to add or delete
};

#endif //P_CENTER_P_CENTER_TABU_H
