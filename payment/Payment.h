#ifndef PAYMENT_H
#define PAYMENT_H

#include <iostream>
using namespace std;

class Payment {
protected:
    double amount;

public:
    Payment(double amt) {
        amount = amt;
    }

    // Pure virtual — makes Payment abstract
    virtual void processPayment() = 0;

    // Calculate fee — can be overridden
    virtual double getAmount() {
        return amount;
    }

    // Print receipt
    virtual void printReceipt() {
        cout << "\n=============================\n";
        cout << "         RECEIPT             \n";
        cout << "=============================\n";
        cout << "Amount Paid : Rs." << amount  << "\n";
        cout << "=============================\n";
    }

    virtual ~Payment() {}
};

#endif
