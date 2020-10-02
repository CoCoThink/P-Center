//
// Created by Guan on 2020/9/28.
//

#include "Graph.h"
#include <iostream>
#include <fstream>
#include <climits>

#define POS_INF 100000000

using std::ifstream;
using std::cout;
using std::endl;
//split string function
vector<string> split(const string &str, const string &pattern)
{
    vector<string> res;
    if(str == "")
        return res;
    string strs = str + pattern;
    size_t pos = strs.find(pattern);

    while(pos != strs.npos)
    {
        string temp = strs.substr(0, pos);
        res.push_back(temp);
        strs = strs.substr(pos+1, strs.size());
        pos = strs.find(pattern);
    }
    return res;
}

int Graph::createFromPMed(string filePath) {
    ifstream input(filePath);
    string line;
    if(input.is_open()){
        getline(input, line);
        vector<string> strs=split(line," ");
        this->vertexNum=stoi(strs[1]);
        this->edgeNum=stoi(strs[2]);
        this->p=stoi(strs[3]);
        //allocate memory
        this->cost=new int*[this->vertexNum];
        this->distance=new int*[this->vertexNum];
        this->path=new int*[this->vertexNum];
        for(int i=0;i<this->vertexNum;++i){
            this->cost[i]=new int[this->vertexNum];
            this->distance[i]=new int[this->vertexNum];
            this->path[i]=new int[this->vertexNum];
            for(int j=0;j<this->vertexNum;++j){
                path[i][j]=-1;
                if(i==j){
                    this->cost[i][j]=0;
                    this->distance[i][j]=0;
                }else{
                    this->cost[i][j]=POS_INF;
                    this->distance[i][j]=POS_INF;
                }
            }
        }
        for(int i=0;i<this->edgeNum;++i){
            getline(input, line);
            vector<string> strs=split(line," ");
            int v1=stoi(strs[1])-1;
            int v2=stoi(strs[2])-1;
            int weight=stoi(strs[3]);
            this->cost[v1][v2]=weight;
            this->cost[v2][v1]=weight;
            this->distance[v1][v2]=weight;
            this->distance[v2][v1]=weight;
        }
        return 0;
    }else{
        return -1;
    }
}

void Graph::floyd() {
    for(int k=0;k<vertexNum;++k){
        for(int i=0;i<vertexNum;++i){
            for(int j=0;j<vertexNum;++j){
                if(distance[i][k]+distance[k][j]<distance[i][j]){
                    distance[i][j]=distance[i][k]+distance[k][j];
                    path[i][j]=k;
                }
            }
        }
    }
}