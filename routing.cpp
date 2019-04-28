//
// Created by 吴贻能 on 2019-04-18.
//
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include "header/routing.h"


// A is a hyper-parameter
void Routing::getPriority(vector<Droplet> &droplets, int A) {
    for (int i = 0; i < droplets.size(); ++i) {
        Cell source1 = droplets[i].source1;
        Cell source2 = droplets[i].source2;
        Cell target = droplets[i].target;
        int max_x = max(source1.x, max(source2.x, target.x));
        int min_x = min(source1.x, min(source2.x, target.x));
        int max_y = max(source1.y, max(source2.y, target.y));
        int min_y = min(source1.y, min(source2.y, target.y));
        int p = 0;

        for (int k = 0; k < droplets.size(); ++k) {
            if (i != k){
                Cell s1 = droplets[k].source1;
                Cell s2 = droplets[k].source2;
                Cell t = droplets[k].target;
                if ((s1.x > min_x && s1.x < max_x && s1.y > min_y && s1.y < max_y)
                    && (s2.x > min_x && s2.x < max_x && s2.y > min_y && s2.y < max_y)
                    && (t.x > min_x && t.x < max_x && t.y > min_y && t.y < max_y)){
                    p += 1;
                }
            }
        }
        droplets[i].priority = p + A * (max(max_x - min_x, max_y - min_y));
    }
}

// TODO 要判断到达不了目标的情况
vector<vector<int> > Routing::modifiedLeeAlgo(Graph &G, Cell &start, Cell &end) {
    vector<vector<int> > dmfb = G.get_dmfb();
    int dl[] = {-1, 0, 1, 0}; // these arrays will help you travel in the 4 directions more easily
    int dc[] = {0, 1, 0, -1};
    int m = (int)dmfb.size();
    int n = (int)dmfb[0].size();
    vector<vector<int> > visited(m, vector<int>(n, 0));
    vector<vector<int> > distance(m, vector<int>(n, 0));
    queue<Cell> Q;
    Q.push(start);
    visited[start.x][start.y] = 1;
    distance[start.x][start.y] = 0;
    while (!Q.empty()){
        Cell cur = Q.front();
        Q.pop();
        if (cur.x == end.x && cur.y == end.y){
            distance[end.x][end.y] = distance[cur.x][cur.y];
            break;
        }
        for (int i = 0; i < 4; ++i) {
            Cell next{cur.x + dl[i], cur.y + dc[i]};
            if (next.x >= 0 && next.x < m && next.y >=0 && next.y < n && dmfb[next.x][next.y] != -1
                && visited[next.x][next.y] != 1){
                distance[next.x][next.y] = distance[cur.x][cur.y] + 1;
                visited[next.x][next.y] = 1;
                Q.push(next);
            }
        }
    }
    return distance;
}

int Routing::getMinDistance(vector<vector<int> > &distance, Cell &target) {
    return distance[target.x][target.y];
}

void Routing::show2DArray(const vector<vector<int> > &arrays) {
    for (int i = 0; i < arrays.size(); ++i) {
        for (int j = 0; j < arrays[i].size(); ++j) {
            std::cout << std::setw(5) << arrays[i][j] << " ";
        }
        cout << endl;
    }
}

vector<Cell> Routing::getOnePath(Graph &G, Cell &start, Cell &end, int seed) {
    vector<vector<int> > dmfb = G.get_dmfb();
    vector<vector<int> > distance = modifiedLeeAlgo(G, start, end);
    int dl[] = {-1, 0, 1, 0}; // these arrays will help you travel in the 4 directions more easily
    int dc[] = {0, 1, 0, -1};
    int m = (int)dmfb.size();
    int n = (int)dmfb[0].size();
    srand(seed); //不能把这个放在循环里面，不然随机出来的数也都是一样的

    vector <Cell> route;

    // 到达不了end
    if (distance[end.x][end.y] == -1){
        return route;
    }

    queue<Cell> Q;
    Q.push(end);
    route.push_back(end);
    while (!Q.empty()){
        Cell cur = Q.front();
        Q.pop();
        if (cur.x == start.x && cur.y == start.y){
            break;
        }
        vector<Cell> cells;
        for (int i = 0; i < 4; ++i) {
            Cell next{cur.x + dl[i], cur.y + dc[i]};
            if (next.x >= 0 && next.x < m && next.y >=0 && next.y < n && dmfb[next.x][next.y] != -1
                && distance[next.x][next.y] == distance[cur.x][cur.y]-1){
                cells.push_back(next);
            }
        }

        int r = rand() % cells.size();
        route.push_back(cells[r]);
        Q.push(cells[r]);
    }
    reverse(route.begin(), route.end());
    return route;
}

vector<vector<Cell> > Routing::getTwoPinNetPaths(Graph &G, Cell &start, Cell &end, int pathNums) {
    vector<vector<Cell> > paths(pathNums);

    for (int j = 0; j < pathNums; ++j) {
        vector <Cell> route = getOnePath(G, start, end, time(NULL)+j);
        paths[j] = route;
    }
    return paths;
}

vector<vector<int> > Routing::changeCellsToIndex(Graph &G, vector<vector<Cell> > &paths) {
    vector<vector<int> > dmfb = G.get_dmfb();
    int m = (int)paths.size();
//    int n = (int)paths[0].size();
//    vector<vector<int> > indexPaths(m, vector<int>(n, 0));
//    for (int i = 0; i < m; ++i) {
//        for (int j = 0; j < n; ++j) {
//            indexPaths[i][j] = dmfb[paths[i][j].x][paths[i][j].y];
//        }
//    }
    vector<vector<int> > indexPaths(m);
    for (int i = 0; i < paths.size(); ++i) {
        for (int j = 0; j < paths[i].size(); ++j) {
            indexPaths[i].push_back(dmfb[paths[i][j].x][paths[i][j].y]);
        }
    }
    return indexPaths;
}

vector<vector<Cell> > Routing::getOnePathFor3PinNet(Graph &G, Droplet &d) {
    Cell s1 = d.source1;
    Cell s2 = d.source2;
    Cell t = d.target;

    int relative = (abs(s1.x - t.x) + abs(s1.y - t.y)) - (abs(s2.x - t.x) + abs(s2.y - t.y));
    Cell nearest = relative > 0 ? s2 : s1;
    Cell further = relative > 0 ? s1 : s2;

    vector<vector<Cell> > route(3);
    srand(time(NULL));
    vector<Cell> nearestPath = getOnePath(G, nearest, t, time(NULL));
    int r = rand() %  nearestPath.size();
    Cell curTarget = nearestPath[r];

    vector<Cell> nearToCur;
    vector<Cell> furtherToCur;
    vector<Cell> curToTarget;

    for (int i = 0; i <= r; ++i) {
        nearToCur.push_back(nearestPath[i]);
    }

    for (int i = r; i < nearestPath.size(); ++i) {
        curToTarget.push_back(nearestPath[i]);
    }

    // TODO 因为这句话出错了
    furtherToCur = getOnePath(G, further, curTarget, time(NULL));

    route[0] = nearToCur;
    route[1] = furtherToCur;
    route[2] = curToTarget;
    return route;
}


// 该部分需要考虑对3-pin nets提前融合的情况，但提前融合，距离离目标点较短的那个液滴就要停留在融合点等待较远的那个液滴来融合
// 当前想的是先求出距离较短的液滴与目标点的路径，然后随机选择路径中的一点作为融合点，这个融合点就成了较远距离液滴的当前目标点
void Routing::modifiedLeeAlgoFor3pinNet(Graph &G, Droplet &d, int pathNums, vector<vector<Cell> > &nearPaths,
                                        vector<vector<Cell> > &farPaths) {
    Cell s1 = d.source1;
    Cell s2 = d.source2;
    Cell t = d.target;

    int relative = (abs(s1.x - t.x) + abs(s1.y - t.y)) - (abs(s2.x - t.x) + abs(s2.y - t.y));
    Cell nearest = relative > 0 ? s2 : s1;
    Cell farest = relative > 0 ? s1 : s2;


    vector<vector<Cell> > nearestPaths = getTwoPinNetPaths(G, nearest, t, pathNums);
    srand(time(NULL));

    for (int i = 0; i < nearestPaths.size(); ++i) {
        int r = rand() % nearestPaths[i].size();
        Cell curTarget = nearestPaths[i][r];
        vector<vector<Cell> > nearPath = getTwoPinNetPaths(G, nearest, curTarget, 1);
        vector<vector<Cell> > farPath = getTwoPinNetPaths(G, farest, curTarget, 1);

        if (nearPath.size() < farPath.size()){
            for (int j = 0; j < farPath.size() - nearPath.size(); ++j) {
                nearPath[0].push_back(curTarget);
            }
        } else{
            for (int j = 0; j < nearPath.size() - farPath.size(); ++j) {
                farPath[0].push_back(curTarget);
            }
        }
        vector<vector<Cell> > lastPath = getTwoPinNetPaths(G, curTarget, t, 1);

        for (int k = 0; k < lastPath.size(); ++k) {
            nearPath[0].push_back(lastPath[0][k]);
            farPath[0].push_back(lastPath[0][k]);
        }
        nearPaths.push_back(nearPath[0]);
        farPaths.push_back(farPath[0]);
    }

}

void Routing::printRoute(Graph &G, vector<Cell> &route) {
    vector<vector<int> > dmfb = G.get_dmfb();
    for (auto c : route) {
        cout << dmfb[c.x][c.y] << " ";
    }
    cout << endl;
}


vector<Droplet> Routing::runRoutingStageOne(Graph &G, vector<Droplet> &ds, int maxT, int pathNum) {
    vector<vector<int> > dmfb = G.get_dmfb();

    // vector<vector<vector<int> > > fluidicCon(maxT, vector<vector<int> >(ds.size(), vector<int>(9, 0))); // time -> droplet id -> fluidic constraints
    // vector<vector<Droplet> > result(2); // first row stores successful droplets, second row stores failure droplets.
    vector<Droplet> fails;
    for (int i = 0; i < ds.size(); i++){
        if (ds[i].type == 2){ // d is a 2-pin net
            // vector<vector<Cell> > paths = getTwoPinNetPaths(G, d.source1, d.target, pathNum);
//            for (auto p : paths){
//                if (p.size() > maxT){ // don't satisfy time constraint
//                    result[1].push_back(d);
//                    break;
//                }
//
//            }
            cout << "droplets id = " << ds[i].id << endl;
            vector<Cell> path = getOnePath(G, ds[i].source1, ds[i].target, time(NULL));
            ds[i].route.resize(1);
            if (path.size() > maxT){
                fails.push_back(ds[i]);
                cout << path.size() << endl;
                continue;
            }else{
                cout << "smaller than maxT" << endl;

                ds[i].route[0] = path;
                for (Cell c : path){
                    dmfb[c.x][c.y] = -1;
                }
                // TODO 因为更改了dmfb导致了getOnePathFor3PinNet这句话出错呃， 是因为route2的优先级比较大导致了route5没有地方走了
                G.setDmfb(dmfb);
            }

        }else if (ds[i].type == 3){
            cout << "droplets id = " << ds[i].id << endl;
            vector<vector<Cell> > path = getOnePathFor3PinNet(G, ds[i]);

            ds[i].route.resize(3);
            for (int j = 0; j < path.size(); j++){ // sb了， 外循环用了i，内循环就不要了再用i了。。。。
                ds[i].route[j] = path[j];
                for (Cell c : path[j]){
                    dmfb[c.x][c.y] = -1;
                }
            }
            G.setDmfb(dmfb);
        }
    }
    return fails;
}

void Routing::runRoutingStageTwo(Graph &G, vector<Droplet> &dsFails, int maxT, int pathNum) {
    vector<vector<int> > dmfb = G.getDmfbBack();
    for (int i = 0; i < dsFails.size(); ++i) {
        if (dsFails[i].type == 2){
            vector<Cell> path = getOnePath(G, dsFails[i].source1, dsFails[i].target, time(NULL));
            dsFails[i].route.resize(1);
        }
    }
}

void Routing::printFinalRoute(Graph &G, vector<Droplet> &ds) {
    vector<vector<int> > dmfb = G.get_dmfb();
    int m = dmfb.size(), n = dmfb[0].size();
    vector<vector<char> > routes(m, vector<char>(n, '#'));
    for (Droplet d : ds){
        if (d.type == 2){
            vector<Cell> path = d.route[0];
            for (Cell c : path){
                routes[c.x][c.y] = (d.id + '0');
            }
        }else if (d.type == 3){
            for (auto path : d.route){
                for (Cell c : path){
                    routes[c.x][c.y] = (d.id + '0');
                }
            }
        }
    }

    for (int i = 0; i < routes.size(); ++i) {
        for (int j = 0; j < routes[i].size(); ++j) {
            std::cout << std::setw(5) << routes[i][j] << " ";
        }
        cout << endl;
    }
}

//void Routing::runRouting(Graph &G, vector<Droplet> &ds){
//    for (int i = 0; i < ds.size(); i++){
//        ds[i].route.resize(1);
//        ds[i].route[0] = getOnePath(G, ds[i].source1, ds[i].target, time(NULL));
//    }
//
//}