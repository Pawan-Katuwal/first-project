#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_NAME 50
#define MAX_DIAGNOSIS 100
#define MAX_DOCTOR 50

typedef struct {
    int id;
    char name[MAX_NAME];
    int age;
    char diagnosis[MAX_DIAGNOSIS];
    char doctor[MAX_DOCTOR];
    char treatment[MAX_DIAGNOSIS];
} patient;

FILE *file;

void addpatient(){
    patient patient;
    file = fopen("patient.dat","ab");
    if (!file){
        printf("Error opening file!\n");
        return;
    }
    
    printf("\nEnter Patient ID: ");
    scanf("%d",&patient.id);
    printf("Enter Patient Name: ");
    getchar(); // To consume newline
    fgets(patient.name, MAX_NAME, stdin);
    patient.name[strcspn(patient.name,"\n")] = '\0'; //Remove newline
    printf("Enter Age: ");
    scanf("%d",&patient.age);
    printf("Enter Diagnosis: ");
    getchar();
    fgets(patient.diagnosis, MAX_DIAGNOSIS, stdin);
    patient.diagnosis[strcspn(patient.diagnosis, "\n")] = '\0';
    printf("Enter Doctor Name: ");
    fgets(patient.doctor, MAX_DOCTOR, stdin);
    patient.doctor[strcspn(patient.doctor, "\n")] = '\0';
    printf("Enter Treatment Details: ");
    fgets(patient.treatment, MAX_DIAGNOSIS, stdin);
    patient.treatment[strcspn(patient.treatment, "\n")] = '\0';
    fwrite(&patient, sizeof(patient),1, file);
    fclose(file);
    printf("patient added successfully!\n");
}

void viewpatients(){
    patient patient;
    file = fopen("patient.dat", "rb");
    if (!file) {
        printf("No records found!\n");
        return;
    }

    printf("\n%-10s%-30s%-10s%-30s%20s%-30s\n", "ID", "Name", "Age", "Diagnosis", "Doctor", "Treatment");
    printf("--------------------------------------------------------------------------------------------\n");
    while (fread(&patient, sizeof(patient), 1, file)) {
        printf("%-10d%-30s%-10d%-30s%-20s%-30s\n", patient.id, patient.name, patient.age, patient.diagnosis, patient.doctor, patient.treatment);
    }
    fclose(file);
}

void searchpatient(){
    int id, found = 0;
    patient patient;

    printf("\nEnter Patient ID to search: ");
    scanf("%d", &id);
    file = fopen("patients.dat", "rb");
    if (!file){
        printf("No records found!\n");
        return;
    }

    while (fread(&patient, sizeof(patient),1, file)) {
        if (patient.id == id) {
            printf("\nPatient Found:\n");
            printf("ID: %d\n", patient.id);
            printf("Name:%s\n", patient.name);
            printf("Age; %d\n", patient.age);
            printf("Diagnosis:%s\n", patient.diagnosis);
            printf("Treatment:%s\n", patient.treatment);
            found = 1;
            break;
        }
    }
    fclose(file);

    if (!found) {
        printf("Patient with ID %d not found!\n",id);
    }
}

void updatepatient(){
    int id , found = 0;
    patient patient;
    FILE *tempFILE;

    printf("\nEnter Patient ID to update: ");
    scanf("%d", &id);

    file = fopen("patients.dat", "rb");
    tempFILE = fopen("temp.dat", "wb");
    if (!file || !tempFILE) {
        printf("Error opening file!\n");
        return;
    }
    while (fread(&patient, sizeof(patient), 1, file)) {
        if (patient.id == id)  {
            found = 1;
            printf("Enter New Diagnosis: ");
            getchar();
            fgets(patient.diagnosis, MAX_DIAGNOSIS, stdin);
            patient.diagnosis[strcspn(patient.diagnosis, "\n")] = '\0';
            printf("Enter New Treatment Details: ");
            fgets(patient.treatment, MAX_DIAGNOSIS, stdin);
            patient.treatment[strcspn(patient.treatment, "\n")] = '\0';
            printf("Patient updated successfully");
        }
        fwrite(&patient, sizeof(patient), 1, tempFILE);
    }

    fclose(file);
    fclose(tempFILE);
    remove("patients.dat");
    rename("temp.dat", "patients.dat");

    if (!found) {
        printf("Patient with Id %d not found!\n", id);
    }
}

void deletepatient() {
    int id, found = 0;
    patient patient;
    FILE *tempFILE;

    printf("\nEnter Patient ID to delete: ");
    scanf("%d", &id);

    file = fopen("patients.dat", "rb");
    tempFILE = fopen("temp.dat", "wb");
    if (!file || !tempFILE) {
        printf("Error opening file!\n");
        return;
    }

    while (fread(&patient, sizeof(patient), 1, file)) {
        if (patient.id == id) {
            found = 1;
            printf("Patient with ID %d delete successfully !\n", id);
        } else {
            fwrite(&patient, sizeof(patient), 1, tempFILE);
        }
    }
    fclose(file);
    fclose(tempFILE);
    remove("patients.dat");
    rename("temp.dat", "patients,dat");

    if (!found) {
        printf("Patient with ID %d not fond!\n", id);
    }
}

void menu(){
    int choice;
    do {
        printf("\n===== Hospital Patient Management system =====\n");
        printf("1. Add Patient\n");
        printf("2. View Patients\n");
        printf("3. Search Patient\n");
        printf("4. Update Patient\n");
        printf("5. Delete Patient\n");
        printf("6. Exit\n");
        printf("=====================================================\n");
        printf("Enter Your Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
            addpatient();
            break;
            case 2:
            viewpatients();
            break;
            case 3:
            searchpatient();
            break;
            case 4:
            updatepatient();
            break;
            case 5:
            deletepatient();
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