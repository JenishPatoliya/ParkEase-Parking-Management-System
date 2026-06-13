#ifndef CASHPAYMENT_H
#define CASHPAYMENT_H

#include "Payment.h"

class CashPayment : public Payment {
private:
    double amountGiven;   // how much customer gave

public:
    CashPayment(double amt, double given)
        : Payment(amt) {
        amountGiven = given;
    }

    void processPayment() override {
        cout << "\n[CASH PAYMENT]" << endl;
        cout << "Amount Due  : Rs." << amount      << endl;
        cout << "Amount Given: Rs." << amountGiven << endl;

        if (amountGiven < amount) {
            cout << "Insufficient amount!" << endl;
            return;
        }

        double change = amountGiven - amount;
        cout << "Change      : Rs." << change << endl;
        cout << "Payment Successful!" << endl;

        printReceipt();
    }
};

#endif
