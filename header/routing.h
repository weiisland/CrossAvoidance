//
// Created by 吴贻能 on 2019-04-18.
// Cross-Contamination Avoidance for Droplet Routing
//

#ifndef CROSSAVOIDANCEALGO_ROUTING_H
#define CROSSAVOIDANCEALGO_ROUTING_H

#include <vector>
#include "droplet.h"
#include "graph.h"

struct FluidicConstraint{
    int time;
    int id;

};

using namespace std;
class Routing{
public:
    void getPriority(vector<Droplet> &droplets, int A);

    // two-pin nets
    vector<vector<int> > modifiedLeeAlgo(Graph &G, Cell &start, Cell &end);
    // 返回一条路径
    vector<Cell> getOnePath(Graph &G, Cell &start, Cell &end, int seed);
    // 返回pathNums条路径
    vector<vector<Cell> > getTwoPinNetPaths(Graph &G, Cell &start, Cell &end, int pathNums);

    vector<vector<int> > changeCellsToIndex(Graph &G, vector<vector<Cell> > &paths);

    // 返回3-pin net的一条路径，分三段
    vector<vector<Cell> > getOnePathFor3PinNet(Graph &G, Droplet &d);
    // three-pin nets
    void modifiedLeeAlgoFor3pinNet(Graph &G, Droplet &d, int pathNums, vector<vector<Cell> > &nearPaths,
                                   vector<vector<Cell> > &farPaths);

    void pathToBlocks();

    void solveConstraints();


     // ds is sorted by priority
     // maxT is the maximum time

    vector<Droplet> runRoutingStageOne(Graph &G, vector<Droplet> &ds, int maxT, int pathNum);

    void runRoutingStageTwo(Graph &G, vector<Droplet> &dsFails, int maxT, int pathNum);

    int getMinDistance(vector<vector<int> > &distance, Cell &target);

    void show2DArray(const vector<vector<int> > &Arrays);

    void printRoute(Graph &G, vector<Cell> &route);

    void printFinalRoute(Graph &G, vector<Droplet> &ds);

    void runRouting(Graph &G, vector<Droplet> &ds);
};

#endif //CROSSAVOIDANCEALGO_ROUTING_H
