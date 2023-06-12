#ifndef COSTUMER_H
#define COSTUMER_H

class Costumer {
  private:
    int c_id;
    int phone;
    bool is_member;
    int monthly_expenses;
    int prize;

  public:
    Costumer(int c_id, int phone) : c_id(c_id), phone(phone), is_member(false),
    monthly_expenses(0), prize(0) {};
    ~Costumer() = default;
    int GetID() const;
    int GetPhone() const;
    bool IsMember() const;
    int GetMonthlyExpenses() const;
    void AddToMonthlyExpenses(int num);
    void SetMember();
    void AddToPrize(int num);
    int GetPrize();
};

#endif // COSTUMER_H