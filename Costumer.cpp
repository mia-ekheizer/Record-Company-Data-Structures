#include "Costumer.h"

int Costumer::GetID() const {
    return c_id;
}

int Costumer::GetPhone() const {
    return phone;
}

bool Costumer::IsMember() const {
    return is_member;
}

int Costumer::GetMonthlyExpenses() const {
    return monthly_expenses;
}

void Costumer::AddToMonthlyExpenses(int num) {
    monthly_expenses += num;
}

void Costumer::SetMember() {
    is_member = true;
}