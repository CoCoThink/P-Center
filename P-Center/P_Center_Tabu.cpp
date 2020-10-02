//
// Created by Guan on 2020/9/28.
//

#include "P_Center_Tabu.h"
#include <iostream>
#include<algorithm>
#include <cstring>

using std::sort;
using std::cout;
using std::endl;
using std::max;

int* dist; //tmp array
int N_table_cmp(int x,int y){
    return dist[x]<dist[y];
}

PCenterTabu::PCenterTabu(Graph g,int maxIter,int optimum){
    this->g=g;
    this->maxIter=maxIter;
    this->optimum=optimum;
    this->F_table=new struct most_second[this->g.vertexNum];
    this->D_table=new struct most_second[this->g.vertexNum];
    this->tabuTenure=new int*[this->g.vertexNum];
    this->M_table=new int[this->g.vertexNum];
    for(int i=0;i<g.vertexNum;++i){
        tabuTenure[i]=new int[this->g.vertexNum];
        memset(tabuTenure[i],0, sizeof(int)*g.vertexNum);
        F_table[i].most=-1;
        F_table[i].second=-1;
        D_table[i].most=INT_MAX;
        D_table[i].second=INT_MAX;
    }
    this->N_table=new int*[this->g.vertexNum];
    //calculate N Table
    for(int i=0;i<g.vertexNum;++i){
        N_table[i]=new int[g.vertexNum];
        for(int j=0;j<g.vertexNum;++j){
            N_table[i][j]=j;
        }
        dist=g.distance[i];
        sort(N_table[i],N_table[i]+g.vertexNum,N_table_cmp);
    }
    //generate initial solution
    this->addFacility(rand()%g.vertexNum);

    for(int i=1;i<g.p;++i){
        int farmostUserV=farmostUserVertices[rand()%farmostUserVertices.size()]; //choose a longest service edge
        int k;
        //search k to determine N[w][k]
        for(k=0;k<g.vertexNum;++k){
            if(N_table[farmostUserV][k]==F_table[farmostUserV].most){
                break;
            }
        }
        this->addFacility(N_table[farmostUserV][rand()%k]);
    }
    bestSc=Sc;

}

void PCenterTabu::addFacility(int vertex) {
    Sc=0;
    sol.emplace(vertex);

    for(int v=0;v<g.vertexNum;++v){

        if(g.distance[vertex][v]<D_table[v].most){ //what if ==

            D_table[v].second=D_table[v].most;
            F_table[v].second=F_table[v].most;
            D_table[v].most=g.distance[vertex][v];
            F_table[v].most=vertex;
        }else if(g.distance[vertex][v]<D_table[v].second){

            D_table[v].second=g.distance[vertex][v];
            F_table[v].second=vertex;
        }
        if(D_table[v].most>Sc){
            Sc=D_table[v].most;
            farmostUserVertices.clear();
            farmostUserVertices.emplace_back(v);
        }else if(D_table[v].most==Sc){
            farmostUserVertices.emplace_back(v);
        }
    }
//    if(iter==203){
//        printf("F_table[v].most %d F_table[v].sec %d D_table[v].most %d D_table[v].second %d\n",F_table[218].most,F_table[218].second,D_table[218].most,D_table[218].second);
//        if(sol.find(212)==sol.end()){
//            printf("err\n");
//        }
//    }
}

void PCenterTabu::findNext(int vertex,int& D,int& F) { //Find the second nearest service point
    int minD=INT_MAX,secondMinD=INT_MAX;
    int min,secondMin;
    for(auto serV=sol.begin();serV!=sol.end();++serV){
        if(g.distance[vertex][*serV]<minD){
            secondMinD=minD;
            secondMin=min;
            minD=g.distance[vertex][*serV];
            min=*serV;

        }else if(g.distance[vertex][*serV]<secondMinD){
            secondMinD=g.distance[vertex][*serV];
            secondMin=*serV;
        }
    }


    if(secondMin==F){
        //For this situation: min 114 minD 21  secondMin 325 secondMinD 21 F=325
        F=min;
    } else{
        F=secondMin;
    }
    D=secondMinD;
}

void PCenterTabu::removeFacility(int vertex) {
    Sc=0;
    sol.erase(vertex);



    for(int v=0;v<g.vertexNum;++v){


        if (F_table[v].most==vertex){

            D_table[v].most=D_table[v].second;
            F_table[v].most=F_table[v].second;

            findNext(v,D_table[v].second,F_table[v].second);

        }else if(F_table[v].second==vertex){
            findNext(v,D_table[v].second,F_table[v].second);
        }
        if(D_table[v].most>Sc){
            Sc=D_table[v].most;
            farmostUserVertices.clear();
            farmostUserVertices.emplace_back(v);
        }else if(D_table[v].most==Sc){
            farmostUserVertices.emplace_back(v);
        }
    }
}



void PCenterTabu::findMove() {
    //There may be some moves with the same deltaF.
    vector<struct Move> tabuMoveList,nontabuMoveList;
    int tabuMinF=INT_MAX;
    int nontabuMinF=INT_MAX;
    int tmpMinF;

    int farmostUserV=farmostUserVertices[rand()%farmostUserVertices.size()]; //choose a longest service edge


    int k;
    //search k to determine N[w][k]
    for(k=0;k<g.vertexNum;++k){
        if(N_table[farmostUserV][k]==F_table[farmostUserV].most){
            break;
        }
    }

    for(int i=0;i<k;++i){ //traverse N[w][k]

        int vertexToAdd=N_table[farmostUserV][i];

        /* For this situation
         F_table[v].most 236 F_table[v].sec 276 D_table[v].most 25 D_table[v].second 25
            Nwk 119 168 37 251 169 313 250 164 136 267 298 319 361 276 312 236
         because 276 is also in the sol */
        if(sol.find(vertexToAdd)!=sol.end()){
            continue;
        }

        addFacility(vertexToAdd);

        //calculate Mf
        memset(M_table,-1, sizeof(int)*g.vertexNum);
        for(int j=0;j<g.vertexNum;++j){
            if(D_table[j].second > M_table[F_table[j].most]){
                M_table[F_table[j].most] = D_table[j].second;
            }
        }

        for(auto f=sol.begin();f!=sol.end();++f){
            if(*f!=vertexToAdd){
                tmpMinF=max(Sc,M_table[*f]);
                if(tabuTenure[vertexToAdd][*f]<iter){
                    //update the non-tabu best move
                    if(tmpMinF <= nontabuMinF){
                        if(tmpMinF< nontabuMinF){
                            nontabuMoveList.clear();
                            nontabuMinF=tmpMinF;
                        }
                        struct Move tmp={vertexToAdd,*f};
                        nontabuMoveList.emplace_back(tmp);
                    }
                } else{
                    //update the tabu best move
                    if(tmpMinF<=tabuMinF){
                        if(tmpMinF<tabuMinF){
                            tabuMoveList.clear();
                            tabuMinF=tmpMinF;
                        }
                        struct Move tmp={vertexToAdd,*f};
                        tabuMoveList.emplace_back(tmp);
                    }
                }
            }
        }

        removeFacility(vertexToAdd);

    }




//    Aspiration criterion: it holds if both the following two conditions satisfy:
//    1. the best tabu move is better than the previous best known solution;
//    2. the best tabu move is better than the best non-tabu move in the current neighborhood.

    if(tabuMinF<bestSc && tabuMinF < nontabuMinF){
        int randIndex=rand()%tabuMoveList.size();
        nextMove.i=tabuMoveList[randIndex].i;
        nextMove.j=tabuMoveList[randIndex].j;
    }else{
        int randIndex=rand()%nontabuMoveList.size();
        nextMove.i=nontabuMoveList[randIndex].i;
        nextMove.j=nontabuMoveList[randIndex].j;
    }

}

void PCenterTabu::makeMove() {
    //add service vertex

    addFacility(nextMove.i);

    //delete service vertex
    removeFacility(nextMove.j);

//    printf("i %d j %d Sc %d\n",nextMove.i,nextMove.j,Sc);
    if(Sc<bestSc){
        bestSc=Sc;
    }
    tabuTenure[nextMove.i][nextMove.j]=iter+Sc+rand()%10;
}


void PCenterTabu::tabuSearch() {
    iter=0;


    while(iter<maxIter){
        ++iter;
        findMove();
        makeMove();

        if(Sc==optimum){
            cout<<"Optimum Found"<<endl;
            //verify
            int verifySc=0;
            int userV,serV;
            int D0Sum=0;
            for(int i=0;i<g.vertexNum;++i){
                D0Sum+=D_table[i].most;
                int Sci=INT_MAX;
                auto j=sol.begin();
                for(auto j=sol.begin();j!=sol.end();++j){
                    if(g.distance[i][*j]<Sci){
                        Sci=g.distance[i][*j];
                    }
                }
                if(Sci>verifySc){
                    verifySc=Sci;
                    userV=i;
                    serV=*j;
                }
            }
            printf("Sc %d VerifySc %d D0_Sum %d userV %d serV %d\n",Sc,verifySc,D0Sum,userV,serV);

            //output Number of Iterations
            cout<<"Number of Iterations: "<<iter<<endl;
            return;
        }
    }
    cout<<"Not Found"<<endl;
}

