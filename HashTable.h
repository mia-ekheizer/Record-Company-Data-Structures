#ifndef HASH_TABLE_H
#define HASH_TABLE_H

class Costumer;

class HashTable {
    public:
    typedef struct Node {
        Node* prev;
        Node* next;
        Costumer* costumer;
    } Node;

    Node* InitNode(Costumer* costumer);
    void DeleteNode(Node* node);

    HashTable();
    ~HashTable();
    int HashFunction(const int c_id);
    Node* Search(const int c_id);
    Node* Insert(Costumer* costumer);
    void Rehash();
    void DeleteOldTable(Node** table_to_delete, int size);
    void InitMonthlyExpenses();

    private:
    Node** table;
    int size;
    int capacity;
};

#endif // HASH_TABLE_H