//
// Created by Amit on 12/06/2023.
//

#include "UnionFind.h"

UnionFind::UnionFind(int *record_stocks, int num_records) : size(num_records) {
    arr = new Node*[num_records];
    sizes = new int[num_records];
    for (int i = 0; i < num_records; ++i) {
        arr[i] = new Node;
        arr[i]->index = i;
        arr[i]->daddy = arr[i];
        arr[i]->record = Record(i, record_stocks[i], 0, record_stocks[i], i);
        sizes[i] = record_stocks[i];
    }
}

UnionFind::~UnionFind() {
    for (int i = 0; i < size; ++i) {
        delete arr[i];
    }
    delete[] arr;
    delete[] sizes;
}

UnionFind::Node *UnionFind::find(int index) {
    int temp_index = index;
    int sum_r = 0, to_sub = 0;
    while (arr[index]->daddy != arr[index]) {
        sum_r += arr[index]->record.get_extra();
        index = arr[index]->daddy->index;
    }
    //sum_r -= arr[index]->record.get_extra(); // im pretty sure this is not needed, but maybe it is
    while (temp_index != index) {
        to_sub += arr[temp_index]->record.get_extra();
        arr[temp_index]->record.set_extra(sum_r - to_sub + arr[temp_index]->record.get_extra());
        Node *temp = arr[temp_index]->daddy;
        arr[temp_index]->daddy = arr[index];
        temp_index = temp->index;
    }
    return arr[index];
}


void UnionFind::set_height(int index) {
    int total_height = 0;
    int temp_index = index;
    while (arr[index]->daddy != arr[index]) {
        index = arr[index]->daddy->index;
        total_height += arr[index]->record.get_extra();
    }
    arr[temp_index]->record.set_height(total_height);
}

void UnionFind::unionNodes(int node1, int node2) {
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

bool UnionFind::areConnected(UnionFind::Node *node1, UnionFind::Node *node2) {
    return find(node1->index) == find(node2->index);
}

int UnionFind::getSpecificSize(int index) {
    return sizes[index];
}

int UnionFind::getSize() const {
    return this->size;
}

