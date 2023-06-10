#include "recordsCompany.h"
#include "memory"
#define EMPTY 0

RecordsCompany::RecordsCompany() :
        records(nullptr, EMPTY),
        records_arr(nullptr)
        {}

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
    return SUCCESS;

    //TODO: add the customers part
}

StatusType RecordsCompany::addCostumer(int c_id, int phone) {

}

Output_t<int> RecordsCompany::getPhone(int c_id) {

}

StatusType RecordsCompany::makeMember(int c_id) {

}

Output_t<bool> RecordsCompany::isMember(int c_id) {

}

StatusType RecordsCompany::buyRecord(int c_id, int r_id) {
    //TODO: add the customers part
    if (r_id < 0 || c_id < 0) {
        return INVALID_INPUT;
    }
    if (r_id >= records.getSize()) {
        return DOESNT_EXISTS;
    }
    int price = records_arr[r_id].getPrice(); //important to do this first because price is changed in the next line
    records_arr[r_id].addPurchase();

    return SUCCESS;
}

StatusType RecordsCompany::addPrize(int c_id1, int c_id2, double  amount) {

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
    UnionFind::Node *node1 = records.find(r_id1);
    UnionFind::Node *node2 = records.find(r_id2);
    if (node1 == node2) {
        return SUCCESS;
    }
    if (records.getSpecificSize(r_id1) > records.getSpecificSize(r_id2)) {

    } else {

    }

    records.unionNodes(r_id1, r_id2);

    return SUCCESS;
}

StatusType RecordsCompany::getPlace(int r_id, int *column, int *hight) {
    if (r_id < 0 || column == nullptr || hight == nullptr) {
        return INVALID_INPUT;
    }
    if (r_id >= records.getSize()) {
        return DOESNT_EXISTS;
    }


    return SUCCESS;
}