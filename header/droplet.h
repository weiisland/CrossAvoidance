//
// Created by 吴贻能 on 2019-04-18.
// cell and droplet presentation
//

#ifndef CROSSAVOIDANCEALGO_DROPLET_H
#define CROSSAVOIDANCEALGO_DROPLET_H

#include <vector>
using namespace std;

struct Cell{
    int x;
    int y;
};

struct Droplet{
    int id; //route id
    int type; // 2-pin net or 2-pin net
    int pre;
    Cell source1;
    Cell source2;
    Cell current1;
    Cell current2;
    Cell target;
    int relative_distance;
    int priority;
    vector<vector<Cell> > route;
};

// 这里需要加个inline，不然编译的时候链接不通过
inline bool cmpPriority(const Droplet &d1, const Droplet &d2){
    return d1.priority < d2.priority;
}

namespace std {
/* implement hash function so we can put GridLocation into an unordered_set */
    template <> struct hash<Cell> {
        typedef Cell argument_type;
        typedef std::size_t result_type;
        std::size_t operator()(const Cell& id) const noexcept {
            return std::hash<int>()(id.x ^ (id.y << 4));
        }
    };
}

inline bool operator == (Cell a, Cell b) {
    return a.x == b.x && a.y == b.y;
}

inline bool operator != (Cell a, Cell b) {
    return !(a == b);
}

inline bool operator < (Cell a, Cell b) {
    return std::tie(a.x, a.y) < std::tie(b.x, b.y);
}
#endif //CROSSAVOIDANCEALGO_DROPLET_H
