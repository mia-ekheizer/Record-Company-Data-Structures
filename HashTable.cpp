#include "HashTable.h"
#include "Costumer.h"

HashTable::Node* HashTable::InitNode(Costumer* costumer) {
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
    if (node->prev) {
        node->prev->next = nullptr;
    }
    if (node->next) {
        node->next->prev = nullptr;
    }
    node->prev = nullptr;
    node->next = nullptr;
    delete node->costumer;
    delete node;
}

HashTable::HashTable() : size(0), capacity(1) {
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
    return c_id % this->size;
}

HashTable::Node* HashTable::Search(const int c_id) {
    int index = HashFunction(c_id);
    Node* curr = table[index];
    while (curr) {
        if (curr->costumer->getID() == c_id) {
            return curr;
        }
        curr = curr->next;
    }
    return nullptr;
}

HashTable::Node* HashTable::Insert(int c_id, int phone) {
    if (Search(c_id)) {
        return nullptr;
    }
    Costumer* costumer = new Costumer(c_id, phone);
    Node* node_to_insert = InitNode(costumer);
    int index = HashFunction(c_id);
    if (table[index] == nullptr) {
        table[index] = node_to_insert;
        size++;
        Rehash();
    }
    else {
        Node* head = table[index];
        head->prev = node_to_insert;
        node_to_insert->next = head;
        table[index] = node_to_insert;
    }
    return node_to_insert;
}

void HashTable::Rehash() {
    if (2*size == capacity) {
        Node** old_table = table;
        int old_size = this->size;
        Node** new_table = new Node*[2*capacity];
        this->table = new_table;
        this->size *= 2;
        for (int i = 0; i < 2*capacity; i++) {
            new_table[i] = nullptr;
        }
        for (int i = 0; i < capacity; i++) {
            Node* curr = old_table[i];
            while (curr) {
                Insert(curr->costumer->getID(), curr->costumer->getPhone());
                curr = curr->next;
            }
        }
        this->capacity *= 2;
        DeleteOldTable(old_table, old_size);
    }
}

void HashTable::DeleteOldTable(Node** table_to_delete, int size) {
    for (int i = 0; i < size; i++) {
        Node* curr = table_to_delete[i];
        while (curr) {
            Node* node_to_delete = curr;
            curr = curr->next;
            DeleteNode(node_to_delete);
        }
    }
    delete[] table;
}
