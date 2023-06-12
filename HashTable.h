#ifndef HASH_TABLE_H
#define HASH_TABLE_H

class Costumer;
#include "AVLTree.h"

class HashTable {
    public:
    HashTable();
    ~HashTable();
    int HashFunction(const int c_id);
    AVLTree<int, Costumer*>::Node* Search(const int c_id);
    AVLTree<int, Costumer*>::Node* Insert(Costumer* costumer);
    void Rehash();
    void CopyTreeToHash(AVLTree<int, Costumer*>::Node* node);
    void DeleteOldTable(AVLTree<int, Costumer*>* table_to_delete);
    void InitMonthlyExpenses();
    void InitMonthlyExpensesAux(AVLTree<int, Costumer*>::Node* node);

    private:
    AVLTree<int, Costumer*>* table;
    int size;
    int capacity;
};

#endif // HASH_TABLE_H