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

    Node* InitNode(const Costumer& costumer);
    void DeleteNode(Node* node);

    HashTable();
    ~HashTable();
    int HashFunction(const int c_id);
    Node* Search(const int c_id);
    Node* Insert(const Costumer& costumer);
    void Rehash();

    private:
    Node** table;
    int size;
    int number_of_elements;
};

#endif // HASH_TABLE_H