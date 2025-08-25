#include "UnionFind.hpp"

namespace graph {

 /**
 * Constructor: creates a Union-Find (Disjoint Set) structure with n elements.
 * Each element starts in its own set.
 *
 * @param n Number of elements.
 */
UnionFind::UnionFind(int n) {
    parent = new int[n];
    rank = new int[n];
    for (int i = 0; i < n; ++i) {
        parent[i] = i;
        rank[i] = 0;
    }
}

/**
 * Destructor: frees the memory used by the Union-Find structure.
 */
UnionFind::~UnionFind() {
    delete[] parent;
    delete[] rank;
}

 /**
 * Finds the root (representative) of the set that x belongs to.
 * Uses path compression to make future queries faster.
 *
 * @param x The element to find.
 * @return The root of the set that x belongs to.
 */
int UnionFind::find(int x) {
    if (parent[x] != x)
        parent[x] = find(parent[x]); 
    return parent[x];
}

 /**
 * Joins the sets that contain x and y.
 * Uses union by rank to keep the structure balanced.
 *
 * @param x First element.
 * @param y Second element.
 */
void UnionFind::unite(int x, int y) {
    int rx = find(x);
    int ry = find(y);
    if (rx == ry) return;

    if (rank[rx] < rank[ry])
        parent[rx] = ry;
    else if (rank[rx] > rank[ry])
        parent[ry] = rx;
    else {
        parent[ry] = rx;
        rank[rx]++;
    }
}

}
