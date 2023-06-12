//
// Created by Amit on 10/06/2023.
//

#ifndef DATA_STRUCTURES_WET_2_RECORD_H
#define DATA_STRUCTURES_WET_2_RECORD_H


class Record {
private:
    int r_id;
    int num_records;
    int num_purchases;
    int height;
    int extra;
    int col;

public:
    Record(int r_id, int num_records, int num_purchases, int height, int col) :
    r_id(r_id),
    num_records(num_records),
    num_purchases(num_purchases),
    height(height),
    col(col) {}

    Record() : r_id(-1), num_records(-1), num_purchases(-1), height(-1),extra(0), col(-1) {} //to show that it's empty

    int get_r_id() const;
    int get_num_records() const;
    int get_num_purchases() const;
    int get_height() const;
    void set_height(int new_height);
    int get_extra() const;
    void set_extra(int new_extra);
    int get_price() const;
    void add_purchase(int amount = 1);
    int get_col() const;
    void set_col(int new_col);

};


#endif //DATA_STRUCTURES_WET_2_RECORD_H
