#ifndef COSTUMER_H
#define COSTUMER_H

class Costumer {
  private:
    int c_id;
    int phone;
    bool is_member;
    int monthly_expenses;
  public:
    Costumer(int c_id, int phone) : c_id(c_id), phone(phone), is_member(false), monthly_expenses(0) {};
    ~Costumer() = default;
    int getID() const;
    int getPhone() const;
    bool isMember() const;
    int getMonthlyExpenses() const;
};

#endif // COSTUMER_H