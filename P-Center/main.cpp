#include <iostream>
#include "Graph.h"
#include "P_Center_Tabu.h"

using std::cout;
using std::endl;

int main() {
    Graph g;
    int readResult=g.createFromPMed("./pmed/pmed16.txt");
    if(readResult!=0){
        cout<<"file read error"<<endl;
        return 1;
    }
    g.floyd();

    PCenterTabu p(g,100000000,47);
    p.tabuSearch();
    return 0;
}