#include "HashTable.h"
#include "Costumer.h"
#include "AVLTree.h"

HashTable::HashTable() : table(new AVLTree<int, Costumer*>[1]), size(0), capacity(1) {
}

HashTable::~HashTable() {
    for (int i = 0; i < size; i++) {
        table[i].DeleteFullTree(table[i].GetRoot());
    }
    delete[] table;
}

int HashTable::HashFunction(const int c_id) {
    return c_id % this->size;
}

AVLTree<int, Costumer*>::Node* HashTable::Search(const int c_id) {
    int index = HashFunction(c_id);
    return table[index].Find(c_id);
}

AVLTree<int, Costumer*>::Node* HashTable::Insert(Costumer* costumer) {
    int index = HashFunction(costumer->GetID());
    AVLTree<int, Costumer*>::Node* node;
    if (table[index].IsEmpty()) {
        node = table[index].Insert(costumer->GetID(), costumer);
        size++;
        Rehash();
    }
    else {
        node = table[index].Insert(costumer->GetID(), costumer);
    }
    return node;
}

void HashTable::Rehash() {
    if (2*size == capacity) {
        AVLTree<int, Costumer*>* old_table = table;
        this->capacity *= 2;
        this->table = new AVLTree<int, Costumer*>[this->capacity];
        for (int i = 0; i < capacity; i++) { 
            this->table[i] = AVLTree<int, Costumer*>();
        }
        for (int i = 0; i < size; i++) {
            this->CopyTreeToHash(old_table[i].GetRoot());
        }
        DeleteOldTable(old_table);
    }
}

void HashTable::CopyTreeToHash(AVLTree<int, Costumer*>::Node* node) {
    if (node == nullptr) {
        return;
    }
    CopyTreeToHash(node->left);
    this->Insert(node->val);
    CopyTreeToHash(node->right);
}

void HashTable::DeleteOldTable(AVLTree<int, Costumer*>* table_to_delete) {
    for (int i = 0; i < size; i++) {
        table_to_delete[i].DeleteFullTree(table_to_delete[i].GetRoot());
    }
    delete[] table_to_delete;
}

void HashTable::InitMonthlyExpenses() {
    for (int i = 0; i < size; i++) {
        InitMonthlyExpensesAux(table[i].GetRoot());
    }
}

void HashTable::InitMonthlyExpensesAux(AVLTree<int, Costumer*>::Node* node) {
    if (node == nullptr) {
        return;
    }
    InitMonthlyExpensesAux(node->left);
    node->val->AddToMonthlyExpenses(-node->val->GetMonthlyExpenses());
    node->val->AddToPrize(-node->val->GetPrize()); // is it part of newMonth?
    InitMonthlyExpensesAux(node->right);
}