#include "HashTable.h"
#include "Costumer.h"

HashTable::Node* HashTable::InitNode(const Costumer& costumer) {
    Node* node = new Node;
    node->prev = nullptr;
    node->next = nullptr;
    node->costumer = costumer;
    return node;
}

void HashTable::DeleteNode(Node* node) {
    if (node == nullptr) {
        return;
    }
    node->prev = nullptr;
    node->next = nullptr;
    node->costumer = nullptr;
    delete node;
}

HashTable::HashTable() : size(0), number_of_elements(0) {
    this->table = new Node*[1];
    this->table[0] = nullptr;
}

HashTable::~HashTable() {
    for (int i = 0; i < size; i++) {
        if (table[i] == nullptr) {
            delete[] table;
            break;
        }
        Node* curr = table[i];
        while (curr) {
            Node* to_delete = curr;
            curr = curr->next;
            DeleteNode(to_delete);
        }
    }
    delete[] table;
}

int HashTable::HashFunction(const int c_id) {
    return this->size
}

HashTable::Node* HashTable::Search(const int c_id) {
    
}

HashTable::Node* HashTable::Insert(const Costumer& costumer) {

}

void HashTable::Rehash() {

}