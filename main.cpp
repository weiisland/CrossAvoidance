#include <iostream>
#include "header/graph.h"
#include "header/routing.h"

void runRouting(Graph &G, Routing &routing, vector<Droplet> &ds){
//    ds[0].route.resize(1);
//    ds[0].route[0] = routing.getOnePath(G, ds[0].source1, ds[0].target, time(NULL));
    for (int i = 0; i < ds.size(); ++i) {
        if (ds[i].type == 2) {
            vector<Cell> path = routing.getOnePath(G, ds[i].source1, ds[i].target, time(NULL));
        }else if (ds[i].type == 3){
            vector<vector<Cell> > path = routing.getOnePathFor3PinNet(G, ds[i]);
        }
    }

}

int main() {

    Graph G;
    // add block
    std::vector<Cell> blocks;
    G.addBlockArea(blocks, 2, 6, 5, 9);
    // create dmfb
    G.createDmfb(16, 16, blocks);
    G.showDmfbStructure();

    // droplet{id, type, pre, source, source2, current1, current2, target, relative distance, priority}
    Droplet d0{0, 2, -1, Cell{9, 10}, Cell{9, 10}, Cell{9, 10}, Cell{9, 10}, Cell{8, 5}, 0, 0};
    Droplet d1{1, 2, -1, Cell{15, 14}, Cell{15, 14}, Cell{15, 14}, Cell{15, 14}, Cell{0, 12}, 0, 0};
    Droplet d2{2, 2, -1, Cell{15, 11}, Cell{15, 11}, Cell{15, 11}, Cell{15, 11}, Cell{15, 8}, 0, 0};

    Droplet tp1{3, 3, -1, Cell{10, 4}, Cell{4, 15}, Cell{10, 4}, Cell{4, 15}, Cell{13, 8}, 0, 0};
    Droplet tp2{4, 3, -1, Cell{10, 1}, Cell{11, 15}, Cell{10, 1}, Cell{11, 15}, Cell{13, 0}, 0, 0};

    // add three-pin net droplets
    vector<Droplet> droplets{d0, d1, d2, tp1, tp2};

    //Cross-Contamination Avoidance for Droplet Routing
    // add priority
    Routing routing;
    routing.getPriority(droplets, 1);

    sort(droplets.begin(), droplets.end(), cmpPriority);
    for (int j = 0; j < droplets.size(); ++j) {
        cout << droplets[j].priority << endl;
    }

    cout << "=========================" << endl;

    vector<Droplet> fails = routing.runRoutingStageOne(G, droplets, 20, 1);

    //droplets[0].route.resize(1);
    //droplets[0].route[0] = routing.getOnePath(G, droplets[0].source1, droplets[0].target, time(NULL));
//    droplets[0].route[0] = vector<Cell>{Cell{1,2}, Cell{2,3}};
   // runRouting(G, routing, droplets);
    // routing.runRouting(G, droplets);
//    for (int i = 0; i < droplets[0].route[0].size(); ++i) {
//        cout << droplets[0].route[0][i].x << endl;
//    }
    routing.printFinalRoute(G, droplets);
    cout << "=========================" << endl;
    G.showDmfbStructure();
//    vector<Cell> cs = G.get4Neighbors(d0.source1);
//    G.printDmfbCellID(cs);
//
//    vector<Cell> ns = G.getNeighborsCell(d0.source1);
//    G.printDmfbCellID(ns);
//
//    vector<Cell> path = G.getRelativeDistancePath(d0.source1, d0.target);
//    G.printDmfbCellID(path);
//    cout << G.getRelativeDistance(path) << endl;
//
//    vector<vector<int> > distance  = routing.modifiedLeeAlgo(G, d1.source1, d1.target);
//    cout << "route 2 minimum distance = " << routing.getMinDistance(distance, d1.target)  << endl;
//    routing.show2DArray(distance);
//
//    cout << "=========================" << endl;
//
//    vector<vector<Cell> > paths = routing.getTwoPinNetPaths(G, d1.source1, d1.target, 4);
//    vector<vector<int> > indexPaths = routing.changeCellsToIndex(G, paths);
//    routing.show2DArray(indexPaths);
//
//    cout << "=========================" << endl;
//
//    vector<Cell> onePath = routing.getOnePath(G, d1.source1, d1.target, time(NULL));
//    routing.printRoute(G, onePath);
//
//    cout << "=========================" << endl;
//
//    vector<vector<Cell> > threePinPath = routing.getOnePathFor3PinNet(G, tp1);
//    routing.show2DArray(routing.changeCellsToIndex(G, threePinPath));

    return 0;
}