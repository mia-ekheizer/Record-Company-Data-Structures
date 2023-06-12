#ifndef RECORDS_COMPANY_H
#define RECORDS_COMPANY_H

#include "utilesWet2.h"
#include "UnionFind.h"
#include "HashTable.h"
#include "AVLTree.h"


class Record;
class Costumer;

class RecordsCompany {

private:
    UnionFind records;
    Record *records_arr;
    HashTable costumers;
    AVLTree<int, Costumer*> members;
    int num_costumers;

public:
    RecordsCompany();
    ~RecordsCompany();
    StatusType newMonth(int *records_stocks, int number_of_records);
    StatusType addCostumer(int c_id, int phone);
    Output_t<int> getPhone(int c_id);
    StatusType makeMember(int c_id);
    void SetPrize(AVLTree<int, Costumer*> members, Costumer* new_member);
    Output_t<bool> isMember(int c_id);
    StatusType buyRecord(int c_id, int r_id);
    StatusType addPrize(int c_id1, int c_id2, double amount);
    void addPrizeAux(int c_id, double amount);
    Output_t<double> getExpenses(int c_id);
    StatusType putOnTop(int r_id1, int r_id2);
    StatusType getPlace(int r_id, int *column, int *hight);
};

#endif