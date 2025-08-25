#ifndef UNIONFIND_HPP
#define UNIONFIND_HPP

namespace graph {

class UnionFind {
private:
    int* parent;
    int* rank;

public:
    UnionFind(int n);
    ~UnionFind();
    int find(int x);
    void unite(int x, int y);
};

} 

#endif
