//
// Created by Amit on 10/06/2023.
//

#ifndef DATA_STRUCTURES_WET_2_UNIONFIND_H
#define DATA_STRUCTURES_WET_2_UNIONFIND_H

#include "Record.h"


class UnionFind {
public:

    typedef struct Node {
        int index;
        Node *daddy;
        Record *record;
    } Node;

    explicit UnionFind(int* record_stocks, int num_records, Record** record_array);
    UnionFind(const UnionFind& to_copy);
    UnionFind& operator=(const UnionFind& to_copy);
    ~UnionFind();
    Node* find(int index);
    void set_height(int index);
    void unionNodes(int node1, int node2);
    bool areConnected(Node *node1, Node *node2);
    int getSpecificSize(int index);
    int getSize() const;

private:

    int size;
    Node **arr;
    int *sizes;

};



#endif //DATA_STRUCTURES_WET_2_UNIONFIND_H
