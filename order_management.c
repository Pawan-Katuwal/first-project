#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_NAME 50
#define MAX_ITEM 50

typedef struct {
    int orderID;
    char customerNAME[MAX_NAME];
    char itemNAME[MAX_ITEM];
    int quantity;
    float totalprice;
    char status[MAX_NAME];
} order;

FILE *file;

void placeorder() {
    order order;
    float priceperItem;

    file = fopen("orders.dat", "ab");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    printf("\nEnter Order ID: ");
    scanf("%d", &order.orderID);
    printf("Enter Customer Name: ");
    getchar(); //To consume newline
    fgets(order.customerNAME, MAX_NAME, stdin);
    order.customerNAME[strcspn(order.customerNAME, "\n")] = '\0'; //Remove newline
    printf("Enter Item Name: ");
    fgets(order.itemNAME, MAX_ITEM, stdin);
    order.itemNAME[strcspn(order.itemNAME, "\n")] = '\0';
    printf("Enter Quantity: ");
    scanf("%d", &order.quantity);
    printf("Enter Price Per Item");
    scanf("%f", &priceperItem);
    order.totalprice = priceperItem * order.quantity;
    strcpy(order.status, "pending");

    fwrite(&order, sizeof(order), 1, file);
    fclose(file);
    printf("Order placed successfully!\n");
}

void vieworder() {
    order order;
    file = fopen("orders.dat", "rb");
    if(!file) {
        printf("No order found!\n");
        return;
    }

    printf("\n%-10s%-30s%-30s%-10s%-15s%-20s\n", "order ID", "Customer Name", "Item Name", "Qty", "Total Price", "Status");
    printf("-----------------------------------------------------------------------------------------------------------\n");
    while (fread(&order, sizeof(order), 1, file)) {
        printf("%10d%-30s%-30s%-10d%-15.2f%-20s\n", order.orderID, order.customerNAME, order.itemNAME, order.quantity, order.totalprice, order.status);
    }
    fclose(file);
}

void searchorder(){
    int id, found = 0;
    order order;

    printf("\nEnter Order ID to search: ");
    scanf("%d", &id);

    file = fopen("order.dat", "rb");
    if (!file) {
        printf("No orders found!\n");
        return;
    }

    while (fread(&order, sizeof(order), 1, file)) {
        if (order.orderID == id) {
            printf("\nOrder Found: \n");
            printf("Order ID: %d\n", order.orderID);
            printf("Customer Name: %s\n", order.customerNAME);
            printf("Item Name: %s\n", order.itemNAME);
            printf("Quantity: %d\n",order.quantity);
            printf("Total Price: %.2f\n", order.totalprice);
            printf("Status: %s\n", order.status);
            found = 1;
            break;
        }
    }
    fclose(file);

    if (!found) {
        printf("Order with ID %d not found!\n", id);
    }
}

void updateorder() {
    int id, found = 0;
    order order;
    FILE *tempFILE;

    printf("\nEnter Order ID to update: ");
    scanf("%d", &id);

    file = fopen("orders.dat", "rb");
    tempFILE = fopen("temp.dat", "wb");
    if (!file || !tempFILE) {
        printf("Error opening file!\n");
        return;
    }
    while (fread(&order, sizeof(order), 1, file)) {
        if (order.orderID == id) {
            found = 1;
            printf("Enter new Quantity: ");
            scanf("%d", &order.quantity);
            printf("Enter new status: ");
            getchar();
            fgets(order.status, MAX_NAME, stdin);
            order.status[strcspn(order.status, "\n")] = '\0';
            order.totalprice = order.totalprice / order.quantity * order.quantity; //Recalculate total price
            printf("Order updated successfully!\n");
        }
        fwrite(&order, sizeof(order), 1, tempFILE);
    }

    fclose(file);
    fclose(tempFILE);
    remove("orders.dat");
    rename("temp.dat", "orders.dat");

    if (!found) {
        printf("Order with ID %d not found!\n", id);
    }
}

void cancelorder(){
    int id, found = 0;
    order order;
    FILE *tempFILE;

    printf("\nEnter order I to cancel: ");
    scanf("%d", &id);

    file = fopen("order.dat", "rb");
    tempFILE = fopen("temp.dat", "wb");
    if (!file || !tempFILE) {
        printf("Error opening file1\n");
        return;
    }

    while (fread(&order, sizeof(order), 1, file)) {
        if (order.orderID == id) {
            found = 1;
            printf("Order with ID %d canceled successfully!\n", id);
        } else {
            fwrite(&order, sizeof(order), 1, tempFILE);
        }
    }

    fclose(file);
    fclose(tempFILE);
    remove("order.dat");
    rename("temp.dat", "orders.dat");

    if (!found) {
        printf("Order with ID %d not found!\n", id);
    }
}

void menu(){
    int choice;
    do {
        printf("\n===== Online order Management System =====\n");
        printf("1. Place Order\n");
        printf("2. View Orders\n");
        printf("3. Search Order\n");
        printf("4. Update Order\n");
        printf("5. Cancel Order\n");
        printf("6. Exit\n");
        printf("================================================\n");
        printf("%d", &choice);

        switch (choice) {
            case 1:
            placeorder();
            break;
            case 2:
            vieworder();
            break;
            case 3:
            searchorder();
            break;
            case 4:
            updateorder();
            break;
            case 5:
            cancelorder();
            break;
            case 6:
            printf("Exiting the program. Goodbye!\n");
            break;
            default:
            printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 6);
}

int main(){
    menu();
    return 0;
}