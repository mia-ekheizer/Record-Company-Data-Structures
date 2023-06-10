//
// Created by Amit on 10/06/2023.
//

#include "Record.h"

int Record::get_r_id() const {
    return r_id;
}

int Record::get_num_records() const {
    return num_records;
}

int Record::get_num_purchases() const {
    return num_purchases;
}

int Record::getPrice() const {
    return num_purchases + 100;
}

void Record::addPurchase(int amount) {
    num_purchases += amount;
}

int Record::getExtra() const {
    return extra;
}

void Record::setExtra(int new_extra) {
    this->extra = new_extra;
}