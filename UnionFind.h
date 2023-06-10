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
        Record record;
    } Node;

    explicit UnionFind(int* record_stocks, int num_records) : size(num_records) {
        arr = new Node*[num_records];
        sizes = new int[num_records];
        for (int i = 0; i < num_records; ++i) {
            arr[i]->index = i;
            arr[i]->daddy = arr[i];
            arr[i]->record = Record(i, record_stocks[i], 0, record_stocks[i], i);
            sizes[i] = 1;
        }
    }

    ~UnionFind() {
        delete[] arr;
        delete[] sizes;
    }

    // find the root of the tree, and compress the path
    Node* find(int index) {
        if (arr[index]->daddy == arr[index]) {
            return arr[index];
        }
        arr[index]->daddy = find(arr[index]->daddy->index);
        return arr[index]->daddy;
    }

    void unionNodes(int node1, int node2) {
        Node *daddy1 = find(node1);
        Node *daddy2 = find(node2);
        if (daddy1 == daddy2) {
            return;
        }
        if (sizes[daddy1->index] < sizes[daddy2->index]) {
            daddy1->daddy = daddy2;
            sizes[daddy2->index] += sizes[daddy1->index];
        } else {
            daddy2->daddy = daddy1;
            sizes[daddy1->index] += sizes[daddy2->index];
        }
    }

    bool areConnected(Node *node1, Node *node2) {
        return find(node1->index) == find(node2->index);
    }

    int getSpecificSize(int index) {
        return sizes[index];
    }

    int getSize() const {
        return this->size;
    }

private:

    int size;
    Node **arr;
    int *sizes;

};



#endif //DATA_STRUCTURES_WET_2_UNIONFIND_H
