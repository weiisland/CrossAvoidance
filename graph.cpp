//
// Created by 吴贻能 on 2019-04-18.
//

#include "header/graph.h"

void Graph::createDmfb(int width, int height, vector<Cell> &b) {
    vector<vector<int> > vec(height);

    for (int i = 0; i < vec.size(); ++i) {
        vec[i].resize(width);
    }

    for (auto it = b.begin(); it != b.end(); ++it) {
        vec[it->x][it->y] = -1;
    }

    int index = 0;
    for (int i = 0; i < vec.size(); ++i) {
        for (int j = 0; j < vec[i].size(); ++j) {
            if (vec[i][j] != -1){
                vec[i][j] = index;
                index++;
            }
        }
    }
    dmfb_width = width;
    dmfb_height = height;
    dmfb = vec;
    dmfbBack = vec;
}

vector<vector<int> > Graph::get_dmfb() {
    return dmfb;
}

vector<vector<int> > Graph::getDmfbBack() {
    return dmfbBack;
}

void Graph::setDmfb(vector<vector<int> > &newDmfb) {
    dmfb = newDmfb;
}

void Graph::addBlockArea(std::vector<Cell> &block, int x1, int y1, int x2, int y2) {
    for (int i = x1; i < x2 + 1; ++i) {
        for (int j = y1; j < y2 + 1; ++j) {
            Cell c;
            c.x = i;
            c.y = j;
            block.push_back(c);
        }
    }
}

void Graph::target_to_block(Cell &c) {
    dmfb[c.x][c.y] = -1;
    std::vector<Cell> vec = getNeighborsCell(c);
    for (int i = 0; i < vec.size(); ++i) {
        dmfb[vec[i].x][vec[i].y] = -1;
    }
}

void Graph::showDmfbStructure() {
    cout << "dmfb structure size = (" << dmfb.size() << ", " << dmfb[0].size() << ")" << endl;
    for (int i = 0; i < dmfb.size(); ++i) {
        for (int j = 0; j < dmfb[i].size(); ++j) {
            std::cout << std::setw(5) << dmfb[i][j] << " ";
        }
        cout << endl;
    }
}

void Graph::addConstraints(std::vector<Droplet> vec) {
    for (int i = 0; i < vec.size(); ++i) {
        Droplet d = vec[i];
        vector<Cell> neighbors = getNeighborsCell(d.current1);
        for (int j = 0; j < neighbors.size(); ++j) {
            Cell c = neighbors[j];
            constraints[c].insert(d.id);
        }
        vector<Cell> neighbors2 = getNeighborsCell(d.current2);
        for (int j = 0; j < neighbors2.size(); ++j) {
            Cell c = neighbors[j];
            constraints[c].insert(d.id);
        }
    }
}

vector<Cell> Graph::get4Neighbors(Cell &cur) {
    vector<Cell> vec;

    // left
    if (cur.y -1 >= 0 && dmfb[cur.x][cur.y-1] != -1){
        Cell c;
        c.x = cur.x;
        c.y = cur.y -1;
        vec.push_back(c);
    }

    // up
    if (cur.x -1 >= 0 && dmfb[cur.x-1][cur.y] != -1){
        Cell c;
        c.x = cur.x - 1;
        c.y = cur.y;
        vec.push_back(c);
    }

    // right
    if (cur.y + 1 < dmfb_width && dmfb[cur.x][cur.y+1] != -1){
        Cell c;
        c.x = cur.x;
        c.y = cur.y + 1;
        vec.push_back(c);
    }

    // bottom
    if (cur.x + 1 < dmfb_height && dmfb[cur.x+1][cur.y] != -1){
        Cell c;
        c.x = cur.x + 1;
        c.y = cur.y;
        vec.push_back(c);
    }
    return vec;
}

vector<Cell> Graph::getNeighborsCell(Cell &cur) {
    vector<Cell> vec;

    // left
    if (cur.y -1 >= 0 && dmfb[cur.x][cur.y-1] != -1){
        Cell c;
        c.x = cur.x;
        c.y = cur.y -1;
        vec.push_back(c);
    }

    // up left
    if (cur.y - 1 >= 0 & cur.x -1 >= 0 && dmfb[cur.x-1][cur.y-1] != -1){
        Cell c;
        c.x = cur.x-1;
        c.y = cur.y -1;
        vec.push_back(c);
    }

    // up
    if (cur.x - 1 >= 0 && dmfb[cur.x-1][cur.y] != -1){
        Cell c;
        c.x = cur.x - 1;
        c.y = cur.y;
        vec.push_back(c);
    }

    // up right
    if (cur.x -1 >= 0 && cur.y + 1 < dmfb_width && dmfb[cur.x-1][cur.y+1] != -1){
        Cell c;
        c.x = cur.x - 1;
        c.y = cur.y + 1;
        vec.push_back(c);
    }


    // right
    if (cur.y + 1 < dmfb_width && dmfb[cur.x][cur.y+1] != -1){
        Cell c;
        c.x = cur.x;
        c.y = cur.y + 1;
        vec.push_back(c);
    }

    // bottom right
    if (cur.x + 1 < dmfb_height && cur.y + 1 < dmfb_width && dmfb[cur.x+1][cur.y+1] != -1){
        Cell c;
        c.x = cur.x + 1;
        c.y = cur.y + 1;
        vec.push_back(c);
    }

    // bottom
    if (cur.x + 1 < dmfb_height && dmfb[cur.x+1][cur.y] != -1){
        Cell c;
        c.x = cur.x + 1;
        c.y = cur.y;
        vec.push_back(c);
    }

    // bottom left
    if (cur.x + 1 < dmfb_height && cur.y - 1 >= 0 && dmfb[cur.x+1][cur.y-1] != -1){
        Cell c;
        c.x = cur.x + 1;
        c.y = cur.y - 1;
        vec.push_back(c);
    }

    return vec;
}

vector<Cell> Graph::getRelativeDistancePath(Cell &start, Cell &end) {
    queue<Cell> q;
    q.push(start);
    unordered_map<Cell, Cell> came_from;
    //map<Cell, Cell, CellCompare> came_from;
    came_from[start] = start;

    while (!q.empty()){
        Cell now = q.front();
        q.pop();

        if (now.x == end.x && now.y == end.y){
            break;
        }
        vector<Cell> neighbors = get4Neighbors(now);
        for (Cell c : neighbors) {
            if (came_from.find(c) == came_from.end()){
                q.push(c);
                came_from[c] = now;
            }
        }
    }

    vector<Cell> path;
    Cell c = end;
    while (c != start){
        path.push_back(c);
        c = came_from[c];
    }
    path.push_back(start); // optional
    reverse(path.begin(), path.end());
    return path;
}

int Graph::getRelativeDistance(const vector<Cell> &path) {
    return path.size() - 1;
}

void Graph::printDmfbCellID(vector<Cell> &cells) {
    cout << "cells' ids are ";
    for (auto c : cells){
        cout << dmfb[c.x][c.y] << " ";
    }
    cout << endl;
}