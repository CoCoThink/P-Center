//
// Created by Guan on 2020/9/28.
//

#ifndef P_CENTER_GRAPH_H
#define P_CENTER_GRAPH_H

#include <vector>
#include <string>
using std::vector;
using std::string;

class Graph{
public:
    int vertexNum;
    int edgeNum;
    int p;
    int** cost;
    int** distance;
    int** path;
    int createFromPMed(string filePath);
    void floyd();
};

#endif //P_CENTER_GRAPH_H
