#include "recordsCompany.h"

#include "UnionFind.h"
#include "Record.h"
#include "HashTable.h"
#include "AVLTree.h"
#include "Costumer.h"
#include "memory"

#define EMPTY 0

RecordsCompany::RecordsCompany() :
        records(nullptr, EMPTY),
        records_arr(nullptr),
        costumers(),
        members(AVLTree<int, Costumer *>()),
        num_costumers(0) {}

RecordsCompany::~RecordsCompany() { //UF is destroyed in its destructor
    delete[] records_arr;
}

StatusType RecordsCompany::newMonth(int *records_stocks, int number_of_records) {
    if (records_stocks == nullptr || number_of_records < 0) {
        return INVALID_INPUT;
    }
    delete[] records_arr;
    try {
        records_arr = new Record[number_of_records];
    } catch (std::bad_alloc &e) {
        delete[] records_arr;
        records_arr = nullptr;
        return ALLOCATION_ERROR;
    }
    for (int i = 0; i < number_of_records; ++i) {
        records_arr[i] = Record(i, records_stocks[i], 0, records_stocks[i], i);
    }
    records = UnionFind(records_stocks, number_of_records);
    costumers.InitMonthlyExpenses();
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
    HashTable::Node *costumer_node = costumers.Search(c_id);
    if (!costumer_node) {
        return Output_t<int>(DOESNT_EXISTS);
    }
    return Output_t<int>(costumer_node->costumer->GetPhone());
}

StatusType RecordsCompany::makeMember(int c_id) {
    if (c_id < 0) {
        return INVALID_INPUT;
    }
    HashTable::Node *costumer_node = costumers.Search(c_id);
    if (!costumer_node) {
        return DOESNT_EXISTS;
    }
    if (costumer_node->costumer->IsMember()) {
        return ALREADY_EXISTS;
    }
    Costumer *new_member = costumer_node->costumer;
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
    HashTable::Node *costumer_node = costumers.Search(c_id);
    if (!costumer_node) {
        return Output_t<bool>(DOESNT_EXISTS);
    }
    return Output_t<bool>(costumer_node->costumer->IsMember());
}

StatusType RecordsCompany::buyRecord(int c_id, int r_id) {
    if (r_id < 0 || c_id < 0) {
        return INVALID_INPUT;
    }
    if (r_id >= records.getSize() || !costumers.Search(c_id)) {
        return DOESNT_EXISTS;
    }
    int price = records_arr[r_id].get_price(); //important to do this first because price is changed in the next line
    records_arr[r_id].add_purchase();
    AVLTree<int, Costumer *>::Node *member_node = members.Find(c_id);
    if (member_node) {
        member_node->val->AddToMonthlyExpenses(price);
    }
    return SUCCESS;
}

StatusType RecordsCompany::addPrize(int c_id1, int c_id2, double amount) {

}

Output_t<double> RecordsCompany::getExpenses(int c_id) {

}

StatusType RecordsCompany::putOnTop(int r_id1, int r_id2) {
    if (r_id1 < 0 || r_id2 < 0) {
        return INVALID_INPUT;
    }
    if (r_id1 >= records.getSize() || r_id2 >= records.getSize()) {
        return DOESNT_EXISTS;
    }
    Record *record1 = &records_arr[r_id1];
    Record *record2 = &records_arr[r_id2];
    if (record1 == record2) {
        return FAILURE;
    }
    if (records.getSpecificSize(r_id1) <= records.getSpecificSize(r_id2)) { //remember that 1 goes on top of 2
        record1->set_extra(record1->get_extra() + record2->get_height() - record2->get_extra());
    } else {
        record1->set_extra(record1->get_extra() + record2->get_height());
        record2->set_extra(record2->get_extra() - record1->get_extra()); //important to update record1 first
    }
    record1->set_col(record2->get_col());
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
    *column = records_arr[r_id].get_col();
    records_arr->set_height(r_id);
    *hight =  records_arr[r_id].get_height();
    return SUCCESS;
}