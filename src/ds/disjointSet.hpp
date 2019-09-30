/**
 * @file   disjointSet.hpp
 * @brief  Datastructure - Disjoint set
 * @author Hao Chen
 * @date   09/30/2019
 *
 **/

#ifndef _DS_DISJOINT_SET_HPP_
#define _DS_DISJOINT_SET_HPP_

#include "src/global/global.hpp"

PROJECT_NAMESPACE_START

class DisjointSet {
 public:
  DisjointSet()
    : parent(0), rnk(0), n(0) {}
  DisjointSet(int n) { init(n); }
  ~DisjointSet() {
    delete [] parent;
    delete [] rnk;
  }

  void init(int n) {
    this->n = n;
    parent = new int[n];
    rnk = new int[n];
    for (int i = 0; i < n; ++i) {
      rnk[i] = 0;
      parent[i] = i;
    }
  }
  // Find set
  int find(int u) {
    return (u == parent[u] ? u : parent[u] = find(parent[u]));
  }

  // Union by rank
  void merge(int x, int y) {
    x = find(x), y = find(y);
    if (x == y)
      return;
    if (rnk[x] > rnk[y])
      parent[y] = x;
    else // If rnk[x] <= rnk[y]
      parent[x] = y;
    if (rnk[x] == rnk[y])
      rnk[y]++;
  }

  // Number of disjoint sets
  int nSets() {
    int nSets = 0;
    for (int i = 0; i < n; ++i)
      if (parent[i] == i) ++nSets;
    return nSets;
  }

 private:
  int *parent, *rnk;
  int n;
};

PROJECT_NAMESPACE_END

#endif /// _DS_DISJOINT_SET_HPP_
