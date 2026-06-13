#ifndef CARDPAYMENT_H
#define CARDPAYMENT_H

#include "Payment.h"

class CardPayment : public Payment {
private:
    string cardNumber;   // last 4 digits only

public:
    CardPayment(double amt, string card)
        : Payment(amt) {
        cardNumber = card;
    }

    void processPayment() override {
        cout << "\n[CARD PAYMENT]" << endl;
        cout << "Card        : XXXX-XXXX-XXXX-"
             << cardNumber << endl;
        cout << "Amount      : Rs." << amount << endl;
        cout << "Processing";

        // Simulate processing delay
        for (int i = 0; i < 3; i++) {
            cout << ".";
        }

        cout << "\nPayment Successful!" << endl;
        printReceipt();
    }
};

#endif
