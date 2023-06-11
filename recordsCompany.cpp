#include "recordsCompany.h"
#include "utilesWet2.h"
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
        members(AVLTree<int, Costumer*>()),
        num_costumers(0)
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
    Costumer* new_costumer = nullptr;
    try {
        new_costumer = new Costumer(c_id, phone);
    } catch (const std::bad_alloc& e) {
        delete new_costumer;
        return ALLOCATION_ERROR;
    }
    try {
        costumers.Insert(new_costumer);
    } catch (const std::bad_alloc& e) {
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
    } catch (const std::bad_alloc& e) {
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
    int price = records_arr[r_id].getPrice(); //important to do this first because price is changed in the next line
    records_arr[r_id].addPurchase();
    AVLTree<int, Costumer*>::Node* member_node = members.Find(c_id);
    if(member_node) {
        member_node->val->AddToMonthlyExpenses(price);
    }
    return SUCCESS;
}

StatusType RecordsCompany::addPrize(int c_id1, int c_id2, double amount) {
    if (c_id1 < 0 || c_id2 < c_id1 || amount <= 0) {
        return INVALID_INPUT;
    }
    AVLTree<int, Costumer*>::Node* min_node = members.GetClosestFromAbove(c_id1);
    min_node = members.GetClosestFromAbove(min_node); // stop one before c_id1
    AVLTree<int, Costumer*>::Node* max_node = members.GetClosestFromBelow(c_id2);
    addPrizeAux(max_node->val->GetID(), amount);
    addPrizeAux(min_node->val->GetID(), -amount);
    return SUCCESS;
}

void RecordsCompany::addPrizeAux(int c_id, double amount) {
    if (members.GetRoot() == nullptr) {
        return;
    }
    AVLTree<int, Costumer*>::Node* curr = members.GetRoot();
    bool first_right = false;
    bool first_left = false;
    bool last_right = false;
    while (curr) {
        if (curr->key == c_id) {
            if (!last_right) {
                curr->val->AddToPrize(amount);
            }
            if (curr->right) {
                curr->right->val->AddToPrize(-amount);
            }
            return;
        } else if (curr->key < c_id) {
            if (!first_right) {
                first_right = true;
                curr->val->AddToPrize(amount);
            }
            curr = curr->right;
            last_right = true;
        } else if (curr->key > c_id) {
            if (!first_left) {
                first_left = true;
                curr->val->AddToPrize(-amount);
            }
            curr = curr->left;
            last_right = false;
        }
    }
}

Output_t<double> RecordsCompany::getExpenses(int c_id) {
    if (c_id < 0) {
        return Output_t<double>(INVALID_INPUT);
    }
    AVLTree<int, Costumer*>::Node* node_of_member = members.Find(c_id);
    if (!node_of_member) {
        return Output_t<double>(DOESNT_EXISTS);
    }
    int sum_prizes = 0;
    AVLTree<int, Costumer*>::Node* curr = members.GetRoot();
    while (curr->val->GetID() != c_id) {
        sum_prizes += curr->val->GetPrize();
        if (curr->val->GetID() < c_id) {
            curr = curr->right;
        }
        else {
            curr = curr->left;
        }
    }
    return Output_t<double>(curr->val->GetMonthlyExpenses() - sum_prizes);
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