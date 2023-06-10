#include "Costumer.h"

int Costumer::getID() const {
    return c_id;
}

int Costumer::getPhone() const {
    return phone;
}

bool Costumer::isMember() const {
    return is_member;
}

int Costumer::getMonthlyExpenses() const {
    return monthly_expenses;
}