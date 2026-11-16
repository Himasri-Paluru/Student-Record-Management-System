#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student {
    int roll;
    char name[50];
    float per;
    struct student *next;
};

struct student *head = NULL;
int roll_count = 1;

// Reassign roll numbers
void reassign_rolls() {
    struct student *temp = head;
    int r = 1;

    while (temp) {
        temp->roll = r++;
        temp = temp->next;
    }
    roll_count = r;
}

// Load from file
void load() {
    FILE *fp = fopen("student.dat", "rb");
    if (!fp) return;

    struct student *temp;
    while (1) {
        temp = (struct student*)malloc(sizeof(struct student));
        if (fread(temp, sizeof(struct student), 1, fp) != 1) {
            free(temp);
            break;
        }
        temp->next = head;
        head = temp;
    }
    fclose(fp);
    reassign_rolls();
}

// Save to file
void save() {
    FILE *fp = fopen("student.dat", "wb");
    struct student *temp = head;

    while (temp) {
        fwrite(temp, sizeof(struct student), 1, fp);
        temp = temp->next;
    }
    fclose(fp);
    printf("Data saved successfully!\n");
}

// Add record
void add() {
    struct student *newnode = (struct student*)malloc(sizeof(struct student));
    newnode->roll = roll_count++;

    printf("Enter Name: ");
    scanf(" %[^\n]", newnode->name);

    printf("Enter Percentage: ");
    scanf("%f", &newnode->per);

    newnode->next = head;
    head = newnode;

    printf("Record Added!\n");
}

// Show records
void show() {
    struct student *temp = head;

    if (!temp) {
        printf("No records found!\n");
        return;
    }

    printf("\nRoll\tName\t\tPercentage\n");
    while (temp) {
        printf("%d\t%s\t\t%.2f\n", temp->roll, temp->name, temp->per);
        temp = temp->next;
    }
}

// Delete record
void delete_record() {
    int choice;
    printf("Delete by: 1.Roll  2.Name : ");
    scanf("%d", &choice);

    struct student *temp = head, *prev = NULL;

    if (choice == 1) {
        int roll;
        printf("Enter Roll: ");
        scanf("%d", &roll);

        while (temp) {
            if (temp->roll == roll) {
                if (prev) prev->next = temp->next;
                else head = temp->next;

                free(temp);
                reassign_rolls();
                printf("Record Deleted!\n");
                return;
            }
            prev = temp;
            temp = temp->next;
        }
        printf("Record not found!\n");
    } else {
        char name[50];
        printf("Enter Name: ");
        scanf(" %[^\n]", name);

        int found = 0;
        while (temp) {
            if (strcmp(temp->name, name) == 0) {
                if (prev) prev->next = temp->next;
                else head = temp->next;

                struct student *del = temp;
                temp = temp->next;
                free(del);
                found = 1;
            } else {
                prev = temp;
                temp = temp->next;
            }
        }

        if (found) {
            reassign_rolls();
            printf("Record(s) Deleted!\n");
        } else {
            printf("Record not found!\n");
        }
    }
}

// Modify record (FINAL UPDATED)
void modify() {
    int search_choice, update_choice;

    printf("Modify by: 1.Roll  2.Name : ");
    scanf("%d", &search_choice);

    struct student *temp = head;
    int found = 0;

    if (search_choice == 1) {
        int roll;
        printf("Enter Roll Number: ");
        scanf("%d", &roll);

        while (temp) {
            if (temp->roll == roll) {

                printf("Update: 1.Name  2.Percentage  3.Both : ");
                scanf("%d", &update_choice);

                if (update_choice == 1 || update_choice == 3) {
                    printf("Enter New Name: ");
                    scanf(" %[^\n]", temp->name);
                }

                if (update_choice == 2 || update_choice == 3) {
                    printf("Enter New Percentage: ");
                    scanf("%f", &temp->per);
                }

                printf("Record Updated!\n");
                return;
            }
            temp = temp->next;
        }
    } else {
        char name[50];
        printf("Enter Name: ");
        scanf(" %[^\n]", name);

        while (temp) {
            if (strcmp(temp->name, name) == 0) {

                printf("Update: 1.Name  2.Percentage  3.Both : ");
                scanf("%d", &update_choice);

                if (update_choice == 1 || update_choice == 3) {
                    printf("Enter New Name: ");
                    scanf(" %[^\n]", temp->name);
                }

                if (update_choice == 2 || update_choice == 3) {
                    printf("Enter New Percentage: ");
                    scanf("%f", &temp->per);
                }

                found = 1;
            }
            temp = temp->next;
        }

        if (found) {
            printf("Record(s) Updated!\n");
            return;
        }
    }

    printf("Record not found!\n");
}

// Sort records
void sort() {
    int choice;
    printf("Sort by: 1.Name (Ascending)  2.Percentage (Descending): ");
    scanf("%d", &choice);

    struct student *i, *j;

    for (i = head; i != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {

            if ((choice == 1 && strcmp(i->name, j->name) > 0) ||
                (choice == 2 && i->per < j->per)) {

                int r = i->roll;
                float p = i->per;
                char n[50];

                strcpy(n, i->name);

                i->roll = j->roll;
                i->per = j->per;
                strcpy(i->name, j->name);

                j->roll = r;
                j->per = p;
                strcpy(j->name, n);
            }
        }
    }

    printf("Sorting done!\n");
}

// Reverse list
void reverse() {
    struct student *prev = NULL, *curr = head, *next;

    while (curr) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    head = prev;
    printf("List Reversed!\n");
}

// Delete all
void delete_all() {
    struct student *temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
    roll_count = 1;
    printf("All records deleted!\n");
}

// Main
int main() {
    char ch;
    load();

    while (1) {
        printf("\n--- Student Record System ---\n");
        printf("A-Add  D-Delete  S-Show  M-Modify\n");
        printf("V-Save T-Sort    R-Reverse L-DeleteAll\n");
        printf("E-Exit\n");
        printf("Enter choice: ");
        scanf(" %c", &ch);

        switch (ch) {
            case 'A': case 'a': add(); break;
            case 'D': case 'd': delete_record(); break;
            case 'S': case 's': show(); break;
            case 'M': case 'm': modify(); break;
            case 'V': case 'v': save(); break;
            case 'T': case 't': sort(); break;
            case 'R': case 'r': reverse(); break;
            case 'L': case 'l': delete_all(); break;
            case 'E': case 'e': save(); exit(0);
            default: printf("Invalid choice!\n");
        }
    }
}