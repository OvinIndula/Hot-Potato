#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream> // For string stream formatting

using namespace std;

// Item data
string potatoTypes[] = {"Small", "Medium", "Large"};
double potatoPrices[] = {5.0, 8.5, 10.25};

string toppings[] = {"Cheese", "Butter", "Garlic Butter", "Beans", "Curry Sauce", "Mushroom", "Onions", "Extra Cheese"};
double toppingPrices[] = {1.0, 0.40, 0.50, 0.75, 1.20, 0.80, 0.40, 1.50};

string extras[] = {"Salad Box", "Hot Dog", "Can of Fizzy Drink", "Bottle of Water", "Tea", "Coffee"};
double extraPrices[] = {1.50, 4.00, 1.20, 0.80, 1.00, 1.20};

// Show menu items
void showMenu(string items[], double prices[], int size) {
    for (int i = 0; i < size; i++) {
        cout << "  " << (i + 1) << ". " << items[i] << " [" << fixed << setprecision(2) << prices[i] << " credits]\n";
    }
}

// Select potato size
int selectPotato(double& total, string& orderDetails, double credits) {
    int choice;
    cout << "\nNEW ORDER – Select Size\n";
    cout << "[Current Balance = " << fixed << setprecision(2) << credits << " credits]\n";
    cout << "---------------------------------\n";
    showMenu(potatoTypes, potatoPrices, 3);
    cout << "  0. Return to Main Menu\n";
    cout << "---------------------------------\n";
    cout << "Enter choice: ";
    cin >> choice;
    if (choice == 0) return -1;
    if (choice < 1 || choice > 3) return -2;

    total += potatoPrices[choice - 1];
    orderDetails += "Potato: " + potatoTypes[choice - 1] + ", ";
    return 1;
}

// Select toppings (at least one)
void selectToppings(double& total, string& orderDetails) {
    bool selected = false;
    int choice;

    do {
        cout << "\nOrder Baked Potato – Select Toppings\n";
        showMenu(toppings, toppingPrices, 8);
        cout << "---------------------------------\n";
        cout << "Enter at least one topping (0 to finish): ";
        cin >> choice;

        if (choice == 0 && !selected) {
            cout << "You must select at least one topping.\n";
        } else if (choice >= 1 && choice <= 8) {
            total += toppingPrices[choice - 1];
            orderDetails += "Topping: " + toppings[choice - 1] + ", ";
            selected = true;
        } else if (choice != 0) {
            cout << "Invalid input. Try again.\n";
        }
    } while (!selected || choice != 0);
}

// Select extras (Can Order more than 1)
void selectExtras(double& total, string& orderDetails) {
    int choice;
    cout << "\nWould you like to order extras? (1 = YES, 0 = NO): ";
    cin >> choice;

    while (choice == 1) {
        cout << "\nExtras Menu\n";
        showMenu(extras, extraPrices, 6);
        cout << "  0. No More Extras\n";
        cout << "---------------------------------\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 0) break;
        if (choice >= 1 && choice <= 6) {
            total += extraPrices[choice - 1];
            orderDetails += "Extra: " + extras[choice - 1] + ", ";
        } else {
            cout << "Invalid choice. Try again.\n";
        }
        cout << "\nWould you like to order another extra? (1 = YES, 0 = NO): ";
        cin >> choice;
    }
}

// Order Summary
string cleanOrderDetails(string details) {
    if (details.length() >= 2 && details.substr(details.length() - 2) == ", ") {
        return details.substr(0, details.length() - 2);
    }
    return details;
}

// Complete order
void placeOrder(double& credits, vector<string>& history) {
    double total = 0.0;
    string orderDetails = "";

    int status = selectPotato(total, orderDetails, credits);
    if (status == -1) return;
    if (status == -2) {
        cout << "Invalid choice. Returning to menu.\n";
        return;
    }

    selectToppings(total, orderDetails);
    selectExtras(total, orderDetails);

    cout << "\n---------------------------------\n";
    cout << "ORDER SUMMARY:\n";
    cout << "Items ordered: " << cleanOrderDetails(orderDetails) << "\n";
    cout << "Total Price: " << fixed << setprecision(2) << total << " credits.\n";
    cout << "---------------------------------\n";

    cout << "\nAvailable Balance: " << fixed << setprecision(2) << credits << " credits\n";
    cout << "---------------------------------\n";

    if (credits >= total) {
        credits -= total;

        // Use ostringstream to format the total (x.xx format) 
        ostringstream formattedTotal;
        formattedTotal << fixed << setprecision(2) << total;
        
        // Add the order to history
        string finalOrder = cleanOrderDetails(orderDetails); 
        finalOrder += " | Total: " + formattedTotal.str();  // Store the formatted total

        history.push_back(finalOrder);
        cout << "Purchase successful!\n";
        cout << "Remaining balance: " << fixed << setprecision(2) << credits << " credits.\n";
    } else {
        cout << "Insufficient credits. Returning to main menu.\n";
    }
}

// Main menu
int main() {
    double credits = 0.0;
    vector<string> recentOrders;
    int option;

    while (true) {
        cout << "\n-------------------------------\n";
        cout << "        HOT POTATO\n";
        cout << "-------------------------------\n";
        cout << "MAIN MENU\n";
        cout << "  1. Add Credits (Current: " << fixed << setprecision(2) << credits << " credits)\n";
        cout << "  2. New Order\n";
        cout << "  3. View Recent Orders\n";
        cout << "  0. Exit\n";
        cout << "\nEnter choice: ";
        cin >> option;

        if (option == 1) {
            double add;
            cout << "Enter credits to add: ";
            cin >> add;
            if (add > 0) {
                credits += add;
                cout << "Credits added. Current balance: " << fixed << setprecision(2) << credits << " credits\n";
            } else {
                cout << "Invalid amount.\n";
            }
        } else if (option == 2) {
            placeOrder(credits, recentOrders);
        } else if (option == 3) {
            cout << "\n-------------------------------\n";
            cout << "RECENT ORDERS:\n";
            if (recentOrders.empty()) {
                cout << "No recent orders.\n";
            } else {
                for (string order : recentOrders) {
                    cout << "---------------------------------\n";
                    cout << order << "\n";
                    cout << "---------------------------------\n";
                }
            }
        } else if (option == 0) {
            cout << "Thank you for using Hot Potato! Goodbye.\n";
            break;
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
 
