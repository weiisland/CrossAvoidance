//
// Created by 吴贻能 on 2019-04-18.
// use graph to present structure of dmfb
//

#ifndef CROSSAVOIDANCEALGO_GRAPH_H
#define CROSSAVOIDANCEALGO_GRAPH_H
#include <queue>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <vector>
#include <unordered_map>
#include <set>
#include "droplet.h"

using namespace std;

class Graph{
private:
    int dmfb_width;
    int dmfb_height;
    map<Cell, set<int> > constraints; //dmfb的每一个Cell都维持一个动态的约束集合，如果一个Cell中约束包含其它液滴的id，那么就无法使用这个Cell
    vector<vector<int> > dmfb;
    vector<vector<int> > dmfbBack;

public:
    // create dmfb
    void createDmfb(int width, int height, std::vector<Cell> &b);

    vector<vector<int> > get_dmfb();

    vector<vector<int> > getDmfbBack();

    void setDmfb( vector<vector<int> > &newDmfb);

    void addBlockArea(std::vector<Cell> &block, int x1, int y1, int x2, int y2);

    void target_to_block(Cell &c);

    void showDmfbStructure();
    // constraints
    void addConstraints(std::vector<Droplet> vec);
    // neighbors
    vector<Cell> get4Neighbors(Cell &cur);

    vector<Cell> getNeighborsCell(Cell &cur);
    // distance
    vector<Cell> getRelativeDistancePath(Cell &start, Cell &end);

    int getRelativeDistance(const vector<Cell> &path);

    void printDmfbCellID(vector<Cell> &cells);
};


#endif //CROSSAVOIDANCEALGO_GRAPH_H
