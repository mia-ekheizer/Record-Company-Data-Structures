#include "recordsCompany.h"
#include "utilesWet2.h"
#include "UnionFind.h"
#include "Record.h"
#include "HashTable.h"
#include "AVLTree.h"
#include "RankTree.h"
#include "Costumer.h"
#include "memory"

#define EMPTY 0

RecordsCompany::RecordsCompany() :
        records(nullptr, EMPTY, nullptr),
        records_arr(nullptr),
        costumers(HashTable()),
        members(RankTree<int, Costumer*>()),
        num_costumers(0)
        {}

RecordsCompany::~RecordsCompany() { //UF is destroyed in its destructor
    delete[] records_arr;
}

StatusType RecordsCompany::newMonth(int *records_stocks, int number_of_records) {
    if (records_stocks == nullptr || number_of_records < 0) {
        return INVALID_INPUT;
    }
    if (records_arr != nullptr) {
        for (int i = 0; i < number_of_records; ++i) {
            delete records_arr[i];
        }
        delete[] records_arr;
    }
    try {
        records_arr = new Record*[number_of_records];
    } catch (std::bad_alloc &e) {
        delete[] records_arr;
        records_arr = nullptr;
        return ALLOCATION_ERROR;
    }
    for (int i = 0; i < number_of_records; ++i) {
        records_arr[i] = new Record(i, records_stocks[i], 0, records_stocks[i], i);
    }
    records = UnionFind(records_stocks, number_of_records, records_arr);
    costumers.InitMonthlyExpenses();
    members.InitRanks(members.GetRoot());
    return SUCCESS;
}

StatusType RecordsCompany::addCostumer(int c_id, int phone) {
    if (c_id < 0 || phone < 0) {
        return INVALID_INPUT;
    }
    if (costumers.Search(c_id)) {
        return ALREADY_EXISTS;
    }
    Costumer *new_costumer = nullptr;
    try {
        new_costumer = new Costumer(c_id, phone);
    } catch (const std::bad_alloc &e) {
        delete new_costumer;
        return ALLOCATION_ERROR;
    }
    try {
        costumers.Insert(new_costumer);
    } catch (const std::bad_alloc &e) {
        delete new_costumer;
        return ALLOCATION_ERROR;
    }
    num_costumers++;
    return SUCCESS;
}

Output_t<int> RecordsCompany::getPhone(int c_id) {
    if (c_id < 0) {
        return Output_t<int>(INVALID_INPUT);
    }
    AVLTree<int, Costumer*>::Node* costumer_node = costumers.Search(c_id);
    if (!costumer_node) {
        return Output_t<int>(DOESNT_EXISTS);
    }
    return Output_t<int>(costumer_node->val->GetPhone());
}

StatusType RecordsCompany::makeMember(int c_id) {
    if (c_id < 0) {
        return INVALID_INPUT;
    }
    AVLTree<int, Costumer*>::Node* costumer_node = costumers.Search(c_id);
    if (!costumer_node) {
        return DOESNT_EXISTS;
    }
    if (costumer_node->val->IsMember()) {
        return ALREADY_EXISTS;
    }
    Costumer* new_member = costumer_node->val;
    new_member->SetMember();
    try {
        members.Insert(c_id, new_member);
    } catch (const std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

Output_t<bool> RecordsCompany::isMember(int c_id) {
    if (c_id < 0) {
        return Output_t<bool>(INVALID_INPUT);
    }
    AVLTree<int, Costumer*>::Node* costumer_node = costumers.Search(c_id);
    if (!costumer_node) {
        return Output_t<bool>(DOESNT_EXISTS);
    }
    return Output_t<bool>(costumer_node->val->IsMember());
}

StatusType RecordsCompany::buyRecord(int c_id, int r_id) {
    if (r_id < 0 || c_id < 0) {
        return INVALID_INPUT;
    }
    if (r_id >= records.getSize() || !costumers.Search(c_id)) {
        return DOESNT_EXISTS;
    }
    int price = records_arr[r_id]->get_price(); //important to do this first because price is changed in the next line
    records_arr[r_id]->add_purchase();
    RankTree<int, Costumer*>::Node* member_node = members.Find(c_id);
    if(member_node) {
        member_node->val->AddToMonthlyExpenses(price);
    }
    return SUCCESS;
}

StatusType RecordsCompany::addPrize(int c_id1, int c_id2, double amount) {
    if (c_id1 < 0 || c_id2 < c_id1 || amount <= 0) {
        return INVALID_INPUT;
    }
    members.AddToRanks(members.GetClosestFromBelow(c_id2), amount);
    members.AddToRanks(members.GetClosestFromBelow(c_id1), -amount);
    return SUCCESS;
}

Output_t<double> RecordsCompany::getExpenses(int c_id) {
    if (c_id < 0) {
        return Output_t<double>(INVALID_INPUT);
    }
    RankTree<int, Costumer*>::Node* node_of_member = members.Find(c_id);
    if (!node_of_member) {
        return Output_t<double>(DOESNT_EXISTS);
    }
    return Output_t<double>(node_of_member->val->GetMonthlyExpenses() - 
    members.GetSumOfRanks(node_of_member));
}

StatusType RecordsCompany::putOnTop(int r_id1, int r_id2) {
    if (r_id1 < 0 || r_id2 < 0) {
        return INVALID_INPUT;
    }
    if (r_id1 >= records.getSize() || r_id2 >= records.getSize()) {
        return DOESNT_EXISTS;
    }
    Record *record1 = records_arr[r_id1];
    Record *record2 = records_arr[r_id2];
    if (record1 == record2) {
        return SUCCESS;
    }
    if (record1->get_col() == record2->get_col()) {
        return FAILURE;
    }
    if (records.getSpecificSize(r_id1) <= records.getSpecificSize(r_id2)) { //remember that 1 goes on top of 2
        record1->set_extra(record1->get_extra() + record2->get_height() - record2->get_extra());
    } else {
        record1->set_extra(record1->get_extra() + record2->get_height());
        record2->set_extra(record2->get_extra() - record1->get_extra()); //important to update record1 first
    }
    record2->set_above(record1);
    Record* next_record = record1;
    while (next_record != nullptr) {
        next_record->set_col(record2->get_col());
        next_record = next_record->get_above();
    }
    records.unionNodes(r_id1, r_id2); //not sure if this is the right order
    return SUCCESS;
}

StatusType RecordsCompany::getPlace(int r_id, int *column, int *hight) {
    if (r_id < 0 || column == nullptr || hight == nullptr) {
        return INVALID_INPUT;
    }
    if (r_id >= records.getSize()) {
        return DOESNT_EXISTS;
    }
    *column = records_arr[r_id]->get_col();
    records.set_height(r_id);
    *hight =  records_arr[r_id]->get_height();
    return SUCCESS;
}