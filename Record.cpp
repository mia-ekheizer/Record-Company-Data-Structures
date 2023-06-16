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

int Record::get_price() const {
    return num_purchases + 100;
}

void Record::add_purchase(int amount) {
    num_purchases += amount;
}

int Record::get_extra() const {
    return extra;
}

void Record::set_extra(int new_extra) {
    this->extra = new_extra;
}

int Record::get_height() const {
    return height;
}

void Record::set_col(int new_col) {
    this->col = new_col;
}

int Record::get_col() const {
    return col;
}

void Record::set_height(int new_height) {
    this->height = new_height;
}

void Record::set_above(Record *new_above) {
    this->above = new_above;
}

Record *Record::get_above() const {
    return above;
}

Record *Record::get_below() const {
    return below;
}

void Record::set_below(Record *new_below) {
    this->below = new_below;
}
