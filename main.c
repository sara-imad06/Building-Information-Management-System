/*
 * Sara Imad Abdou - ID #1241167 - Section #4
 *
 * This program keeps building records using an AVL tree and a hash table
 * It reads the buildings from info.txt and lets the user add, search, update, list, and delete buildings
 * It also saves the AVL data in hash.txt, builds the hash table from that file, and saves the final hash data again
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

int TABLE_SIZE = 53;         // hash table size

#define HASH2_PRIME 7        // prime used for hash2
#define MAX_LINE 300         // max line size
#define EMPTY 0              // empty hash place
#define ACTIVE 1             // active hash place
#define DELETED 2            // deleted hash place

typedef struct Building Building;
typedef struct AVLnode *AVLNode;
typedef struct HashEntry HashEntry;

// stores one building information
typedef struct Building{
    char name[100];
    int number;
    char address[100];
    int apartments;
    int year;
    char payed[4];
}Building;

// one node in the AVL tree
struct AVLnode{
    Building Element;
    AVLNode Left;
    AVLNode Right;
    int Height;
};

// one place in the hash table
struct HashEntry{
    Building Element;
    int Status;
};

// function prototypes
void RemoveNewLine(char text[]);
void ReadString(char message[], char text[], int size);
void ReadBuilding(Building *buildingData);
void PrintBuilding(Building buildingData);
int  IsNotPaid(char payed[]);
int  ReadInteger(char message[]);
int  ParseLine(char line[], Building *buildingData);

// AVL function prototypes
AVLNode MakeEmpty(AVLNode T);
AVLNode Find(char name[], AVLNode T);
AVLNode FindMin(AVLNode T);
AVLNode SingleRotateWithLeft(AVLNode K2);
AVLNode SingleRotateWithRight(AVLNode K1);
AVLNode DoubleRotateWithLeft(AVLNode K3);
AVLNode DoubleRotateWithRight(AVLNode K1);
AVLNode Balance(AVLNode T);
AVLNode Insert(Building X, AVLNode T);
AVLNode Delete(char name[], AVLNode T);
int     Height(AVLNode P);
int     Max(int Lhs, int Rhs);
void    PrintInOrder(AVLNode T);
void    PrintGreaterApartments(AVLNode T, int apartmentLimit);
void    PrintUnpaidBuildings(AVLNode T);
void    SaveTreeInOrder(AVLNode T, FILE *outputFile);
void    ReadInfoFile(AVLNode *buildingTree);
void    InsertBuildingOption(AVLNode *buildingTree);
void    FindAndUpdateOption(AVLNode buildingTree);
void    DeleteBuildingOption(AVLNode *buildingTree);
void    SaveAVLToHashFile(AVLNode buildingTree);
void    PrintMenu();

// hash function prototypes
void InitializeHashTable(HashEntry hashTable[], int size);
void PrintHashTable(HashEntry hashTable[], int size);
void PrintHashInfo(int size, int hashCount);
void LoadHashFromFile(HashEntry hashTable[], int size, int *hashCount);
void SaveHashToFile(HashEntry hashTable[], int size);
void InsertHashOption(HashEntry hashTable[], int size, int *hashCount);
void SearchHashOption(HashEntry hashTable[], int size);
void DeleteHashOption(HashEntry hashTable[], int size, int *hashCount);
int  KeyValue(char name[]);
int  Hash1(char name[], int size);
int  Hash2(char name[]);
int  SearchHash(HashEntry hashTable[], int size, char name[], int *collisions);
int  InsertHash(HashEntry hashTable[], int size, Building buildingData, int *hashCount);
int  DeleteHash(HashEntry hashTable[], int size, char name[], int *hashCount);

// removes the new line from a string
void RemoveNewLine(char text[]){
    text[strcspn(text, "\n")] = '\0';
}

// checks if the fees are not payed
int IsNotPaid(char payed[]) {
    if(strcasecmp(payed, "no") == 0)
        return 1;
    if(strcasecmp(payed, "n") == 0)
        return 1;
    return 0;
}

// reads an integer from the user
int ReadInteger(char message[]){
    char line[MAX_LINE];
    int value;
    int flag;
    do{
        printf("%s", message);
        if(fgets(line, MAX_LINE, stdin) == NULL)
            return 0;
        flag = sscanf(line, "%d", &value);
        if(flag != 1)
            printf("Please enter a valid integer\n");
    }while(flag != 1);
    return value;
}

// reads a string from the user
void ReadString(char message[], char text[], int size){
    char line[MAX_LINE];
    printf("%s", message);
    if(fgets(line, MAX_LINE, stdin) == NULL){
        text[0] = '\0';
        return;
    }
    RemoveNewLine(line);
    strncpy(text, line, size - 1);
    text[size - 1] = '\0';
}

// reads all building information from the user
void ReadBuilding(Building *buildingData){
    ReadString("Enter building name: ", buildingData->name, 100);
    buildingData->number = ReadInteger("Enter building number: ");
    ReadString("Enter building address: ", buildingData->address, 100);
    buildingData->apartments = ReadInteger("Enter number of apartments: ");
    buildingData->year = ReadInteger("Enter establishment year: ");
    ReadString("Payed fees? yes/no: ", buildingData->payed, 4);
}

// prints one building record
void PrintBuilding(Building buildingData){
    printf("Name: %s | Number: %d | Address: %s | Apartments: %d | Year: %d | Payed fees: %s\n",
           buildingData.name, buildingData.number, buildingData.address,
           buildingData.apartments, buildingData.year, buildingData.payed);
}

// splits one line from the file into building fields
// returns 1 if the line is correct and 0 if not
int ParseLine(char line[], Building *buildingData){
    char *token;
    RemoveNewLine(line);

    // reads each field separated by colon
    token = strtok(line, ":");
    if(token == NULL) return 0;
    strncpy(buildingData->name, token, 99);
    buildingData->name[99] = '\0';

    token = strtok(NULL, ":");
    if(token == NULL) return 0;
    buildingData->number = atoi(token);

    token = strtok(NULL, ":");
    if(token == NULL) return 0;
    strncpy(buildingData->address, token, 99);
    buildingData->address[99] = '\0';

    token = strtok(NULL, ":");
    if(token == NULL) return 0;
    buildingData->apartments = atoi(token);

    token = strtok(NULL, ":");
    if(token == NULL) return 0;
    buildingData->year = atoi(token);

    token = strtok(NULL, ":");
    if(token == NULL) return 0;
    strncpy(buildingData->payed, token, 3);
    buildingData->payed[3] = '\0';

    return 1;
}

// deletes all nodes in the AVL tree
AVLNode MakeEmpty(AVLNode T){
    if(T != NULL){
        MakeEmpty(T->Left);
        MakeEmpty(T->Right);
        free(T);
    }
    return NULL;
}

// searches for a building name in the AVL tree
AVLNode Find(char name[], AVLNode T){
    int cmp;
    if(T == NULL)
        return NULL;
    cmp = strcmp(name, T->Element.name);
    if(cmp < 0)
        return Find(name, T->Left);
    else if(cmp > 0)
        return Find(name, T->Right);
    else
        return T;
}

// finds the smallest node in the AVL tree
AVLNode FindMin(AVLNode T){
    if(T == NULL)
        return NULL;
    else if(T->Left == NULL)
        return T;
    else
        return FindMin(T->Left);
}

// returns the height of a node
int Height(AVLNode P){
    if(P == NULL)
        return -1;
    else
        return P->Height;
}

// returns the bigger number
int Max(int Lhs, int Rhs){
    return Lhs > Rhs ? Lhs : Rhs;
}

// does a single rotation with left child
AVLNode SingleRotateWithLeft(AVLNode K2){
    AVLNode K1;
    K1 = K2->Left;
    K2->Left = K1->Right;
    K1->Right = K2;
    K2->Height = Max(Height(K2->Left), Height(K2->Right)) + 1;
    K1->Height = Max(Height(K1->Left), K2->Height) + 1;
    return K1;
}

// does a single rotation with right child
AVLNode SingleRotateWithRight(AVLNode K1){
    AVLNode K2;
    K2 = K1->Right;
    K1->Right = K2->Left;
    K2->Left = K1;
    K1->Height = Max(Height(K1->Left), Height(K1->Right)) + 1;
    K2->Height = Max(Height(K2->Right), K1->Height) + 1;
    return K2;
}

// does a double rotation with left child
AVLNode DoubleRotateWithLeft(AVLNode K3){
    K3->Left = SingleRotateWithRight(K3->Left);
    return SingleRotateWithLeft(K3);
}

// does a double rotation with right child
AVLNode DoubleRotateWithRight(AVLNode K1){
    K1->Right = SingleRotateWithLeft(K1->Right);
    return SingleRotateWithRight(K1);
}

// balances the AVL node if needed
AVLNode Balance(AVLNode T){
    if(T == NULL)
        return NULL;

    if(Height(T->Left) - Height(T->Right) > 1){
        // left side is taller so rotate if needed
        if(Height(T->Left->Left) >= Height(T->Left->Right))
            T = SingleRotateWithLeft(T);
        else
            T = DoubleRotateWithLeft(T);
    }
    else if(Height(T->Right) - Height(T->Left) > 1){
        // right side is taller so rotate if needed
        if(Height(T->Right->Right) >= Height(T->Right->Left))
            T = SingleRotateWithRight(T);
        else
            T = DoubleRotateWithRight(T);
    }

    // update the height after balancing
    T->Height = Max(Height(T->Left), Height(T->Right)) + 1;
    return T;
}

// inserts a building in the AVL tree
// duplicate names are not inserted
AVLNode Insert(Building X, AVLNode T){
    int cmp;
    if(T == NULL){
        // create a new node
        T = malloc(sizeof(struct AVLnode));
        if(T == NULL){
            printf("Out of space!!!\n");
            return NULL;
        }
        T->Element = X;
        T->Height = 0;
        T->Left = NULL;
        T->Right = NULL;
    }
    else{
        cmp = strcmp(X.name, T->Element.name);
        if(cmp < 0)
            T->Left = Insert(X, T->Left);
        else if(cmp > 0)
            T->Right = Insert(X, T->Right);
        else{
            // same name already exists
            printf("Building already exists Duplicate ignored\n");
            return T;
        }
    }
    return Balance(T);
}

// deletes a building from the AVL tree
// uses the smallest node from the right side if needed
AVLNode Delete(char name[], AVLNode T){
    AVLNode temp;
    int cmp;
    if(T == NULL)
        return NULL;

    cmp = strcmp(name, T->Element.name);
    if(cmp < 0)
        T->Left = Delete(name, T->Left);
    else if(cmp > 0)
        T->Right = Delete(name, T->Right);
    else{
        // node is found
        if(T->Left != NULL && T->Right != NULL){
            // node has two children
            temp = FindMin(T->Right);
            T->Element = temp->Element;
            T->Right = Delete(temp->Element.name, T->Right);
        }
        else{
            // node has zero or one child
            temp = T;
            if(T->Left == NULL)
                T = T->Right;
            else
                T = T->Left;
            free(temp);
        }
    }
    return Balance(T);
}

// prints buildings in alphabetical order
void PrintInOrder(AVLNode T){
    if(T != NULL){
        PrintInOrder(T->Left);
        PrintBuilding(T->Element);
        PrintInOrder(T->Right);
    }
}

// prints buildings with apartments greater than the limit
void PrintGreaterApartments(AVLNode T, int apartmentLimit){
    if(T != NULL){
        PrintGreaterApartments(T->Left, apartmentLimit);
        if(T->Element.apartments > apartmentLimit)
            PrintBuilding(T->Element);
        PrintGreaterApartments(T->Right, apartmentLimit);
    }
}

// prints buildings with unpaid fees
void PrintUnpaidBuildings(AVLNode T){
    if(T != NULL){
        PrintUnpaidBuildings(T->Left);
        if(IsNotPaid(T->Element.payed))
            PrintBuilding(T->Element);
        PrintUnpaidBuildings(T->Right);
    }
}

// saves AVL buildings to the file
void SaveTreeInOrder(AVLNode T, FILE *outputFile){
    if(T != NULL){
        SaveTreeInOrder(T->Left, outputFile);
        fprintf(outputFile, "%s:%d:%s:%d:%d:%s\n",
                T->Element.name, T->Element.number, T->Element.address,
                T->Element.apartments, T->Element.year, T->Element.payed);
        SaveTreeInOrder(T->Right, outputFile);
    }
}

// reads info.txt and builds the AVL tree
void ReadInfoFile(AVLNode *buildingTree){
    FILE *inputFile;
    char line[MAX_LINE];
    Building buildingData;
    int count = 0;

    inputFile = fopen("info.txt", "r");
    if(inputFile == NULL){
        printf("Cannot open info.txt\n");
        return;
    }
    while(fgets(line, MAX_LINE, inputFile) != NULL){
        if(ParseLine(line, &buildingData)){
            *buildingTree = Insert(buildingData, *buildingTree);
            count++;
        }
    }
    fclose(inputFile);
    printf("%d records were read from info.txt\n", count);
}

// adds a new building to the AVL tree
void InsertBuildingOption(AVLNode *buildingTree){
    Building buildingData;
    ReadBuilding(&buildingData);
    *buildingTree = Insert(buildingData, *buildingTree);
}

// finds a building and updates it if the user wants
// the year is not updated because it is fixed information
void FindAndUpdateOption(AVLNode buildingTree){
    char name[100];
    AVLNode foundNode;
    int choice;

    ReadString("Enter building name to find: ", name, 100);
    foundNode = Find(name, buildingTree);
    if(foundNode == NULL){
        printf("Building not found\n");
        return;
    }

    PrintBuilding(foundNode->Element);
    choice = ReadInteger("Do you want to update it? 1 yes, 0 no: ");
    if(choice == 1){
        foundNode->Element.number     = ReadInteger("Enter new building number: ");
        ReadString("Enter new address: ", foundNode->Element.address, 100);
        foundNode->Element.apartments = ReadInteger("Enter new number of apartments: ");
        ReadString("Payed fees? yes/no: ", foundNode->Element.payed, 4);
        printf("Building updated\n");
    }
}

// deletes a building from the AVL tree by name
void DeleteBuildingOption(AVLNode *buildingTree){
    char name[100];
    ReadString("Enter building name to delete: ", name, 100);
    if(Find(name, *buildingTree) == NULL){
        printf("Building not found\n");
        return;
    }
    *buildingTree = Delete(name, *buildingTree);
    printf("Building deleted from AVL tree\n");
}

// saves AVL data to hash.txt
void SaveAVLToHashFile(AVLNode buildingTree){
    FILE *outputFile;
    outputFile = fopen("hash.txt", "w");
    if(outputFile == NULL){
        printf("Cannot open hash.txt\n");
        return;
    }
    SaveTreeInOrder(buildingTree, outputFile);
    fclose(outputFile);
    printf("AVL data saved to hash.txt\n");
}

// makes all hash places empty
void InitializeHashTable(HashEntry hashTable[], int size){
    int count;
    for(count = 0; count < size; count++)
        hashTable[count].Status = EMPTY;
}

// gets a number from the first four letters of the name
int KeyValue(char name[]){
    int count;
    int value = 0;
    for(count = 0; count < 4 && name[count] != '\0'; count++)
        value = value + name[count];
    if(value < 0)
        value = -value;
    return value;
}

// first hash function
int Hash1(char name[], int size){
    return KeyValue(name) % size;
}

// second hash function for the jump step
int Hash2(char name[]){
    return HASH2_PRIME - (KeyValue(name) % HASH2_PRIME);
}

// searches in the hash table using double hashing
// returns the index if found and counts collisions
int SearchHash(HashEntry hashTable[], int size, char name[], int *collisions){
    int count;
    int index;
    int mainHash = Hash1(name, size);
    int stepHash = Hash2(name);
    *collisions = 0;

    for(count = 0; count < size; count++){
        index = (mainHash + count * stepHash) % size;

        if(hashTable[index].Status == EMPTY)
            return -1;  // empty place means the record is not found

        if(hashTable[index].Status == ACTIVE){
            if(strcmp(hashTable[index].Element.name, name) == 0)
                return index;  // found the record
            else
                (*collisions)++;  // active place with another name means collision
        }
        // deleted places are skipped
    }
    return -1;
}

// inserts a building in the hash table
// if the name already exists the old record is replaced
// returns 1 if inserted and 0 if the table is full
int InsertHash(HashEntry hashTable[], int size, Building buildingData, int *hashCount){
    int count;
    int index;
    int deletedSpot = -1;  // first deleted place found
    int mainHash = Hash1(buildingData.name, size);
    int stepHash = Hash2(buildingData.name);

    for(count = 0; count < size; count++){
        index = (mainHash + count * stepHash) % size;

        if(hashTable[index].Status == ACTIVE){
            if(strcmp(hashTable[index].Element.name, buildingData.name) == 0){
                // same name so update it only
                hashTable[index].Element = buildingData;
                return 1;
            }
        }
        else if(hashTable[index].Status == DELETED){
            // save the deleted place to use it later
            if(deletedSpot == -1)
                deletedSpot = index;
        }
        else{
            // empty place found but use deleted place first if found
            if(deletedSpot != -1)
                index = deletedSpot;
            hashTable[index].Element = buildingData;
            hashTable[index].Status  = ACTIVE;
            (*hashCount)++;
            return 1;
        }
    }

    // no empty place but maybe there is a deleted place
    if(deletedSpot != -1){
        hashTable[deletedSpot].Element = buildingData;
        hashTable[deletedSpot].Status  = ACTIVE;
        (*hashCount)++;
        return 1;
    }

    return 0;  // table is full
}

// deletes a building from the hash table
// returns 1 if deleted and 0 if not found
int DeleteHash(HashEntry hashTable[], int size, char name[], int *hashCount){
    int collisions;
    int position = SearchHash(hashTable, size, name, &collisions);
    if(position == -1)
        return 0;
    hashTable[position].Status = DELETED;
    (*hashCount)--;
    return 1;
}

// prints all hash table places
void PrintHashTable(HashEntry hashTable[], int size){
    int count;
    printf("\n");
    printf("+-------+------------------------------------------------------------------+\n");
    printf("| Index | Building Info                                                    |\n");
    printf("+-------+------------------------------------------------------------------+\n");
    for(count = 0; count < size; count++){
        if(hashTable[count].Status == ACTIVE){
            printf("| [%3d] | ", count);
            printf("Name: %-15s Num: %-5d Apts: %-4d Year: %-4d Paid: %-3s |\n",
                   hashTable[count].Element.name,
                   hashTable[count].Element.number,
                   hashTable[count].Element.apartments,
                   hashTable[count].Element.year,
                   hashTable[count].Element.payed);
        }
        else if(hashTable[count].Status == DELETED){
            printf("| [%3d] | %-64s |\n", count, "-- Deleted --");
        }
        else{
            printf("| [%3d] | %-64s |\n", count, "-- Empty --");
        }
    }
    printf("+-------+------------------------------------------------------------------+\n");
}

// prints hash size and load factor
void PrintHashInfo(int size, int hashCount){
    double loadFactor = (double)hashCount / size;
    printf("\n");
    printf("+-------------------------------+\n");
    printf("|      Hash Table Info          |\n");
    printf("+-------------------------------+\n");
    printf("| Table size    : %-12d |\n", size);
    printf("| Records       : %-12d |\n", hashCount);
    printf("| Load factor   : %-12.2f |\n", loadFactor);
    printf("+-------------------------------+\n");
}

// reads hash.txt and creates the hash table
void LoadHashFromFile(HashEntry hashTable[], int size, int *hashCount){
    FILE *inputFile;
    char line[MAX_LINE];
    Building buildingData;
    int count = 0;

    InitializeHashTable(hashTable, size);
    *hashCount = 0;

    inputFile = fopen("hash.txt", "r");
    if(inputFile == NULL){
        printf("Cannot open hash.txt Save AVL data first\n");
        return;
    }

    while(fgets(line, MAX_LINE, inputFile) != NULL){
        if(ParseLine(line, &buildingData)){
            if(InsertHash(hashTable, size, buildingData, hashCount))
                count++;
        }
    }
    fclose(inputFile);
    printf("Hash table created from hash.txt Records loaded: %d\n", count);
}

// saves active hash records to hash.txt
void SaveHashToFile(HashEntry hashTable[], int size){
    FILE *outputFile;
    int count;

    outputFile = fopen("hash.txt", "w");
    if(outputFile == NULL){
        printf("Cannot open hash.txt\n");
        return;
    }
    for(count = 0; count < size; count++){
        if(hashTable[count].Status == ACTIVE){
            fprintf(outputFile, "%s:%d:%s:%d:%d:%s\n",
                    hashTable[count].Element.name,
                    hashTable[count].Element.number,
                    hashTable[count].Element.address,
                    hashTable[count].Element.apartments,
                    hashTable[count].Element.year,
                    hashTable[count].Element.payed);
        }
    }
    fclose(outputFile);
    printf("Hash table data saved to hash.txt\n");
}

// adds a new building to the hash table
void InsertHashOption(HashEntry hashTable[], int size, int *hashCount){
    Building buildingData;
    if(*hashCount >= size){
        printf("Hash table is full\n");
        return;
    }
    ReadBuilding(&buildingData);
    if(InsertHash(hashTable, size, buildingData, hashCount))
        printf("Record inserted into hash table\n");
    else
        printf("Could not insert Hash table is full\n");
}

// searches for a building in the hash table
void SearchHashOption(HashEntry hashTable[], int size){
    char name[100];
    int collisions;
    int position;

    ReadString("Enter building name to search: ", name, 100);
    position = SearchHash(hashTable, size, name, &collisions);

    if(position == -1){
        printf("Building not found\n");
        printf("Number of collisions: %d\n", collisions);
    }
    else{
        printf("Building found at index %d\n", position);
        printf("Number of collisions to reach it: %d\n", collisions);
        PrintBuilding(hashTable[position].Element);
    }
}

// deletes a building from the hash table by name
void DeleteHashOption(HashEntry hashTable[], int size, int *hashCount){
    char name[100];
    ReadString("Enter building name to delete from hash table: ", name, 100);
    if(DeleteHash(hashTable, size, name, hashCount))
        printf("Record deleted from hash table\n");
    else
        printf("Building not found\n");
}

// prints the menu
void PrintMenu(){
    printf("\n");
    printf("+===========================================+\n");
    printf("|     Building Information Maintainer       |\n");
    printf("+===========================================+\n");
    printf("|            --- AVL Tree ---               |\n");
    printf("|  0  Read file and create tree            |\n");
    printf("|  1  Insert building                      |\n");
    printf("|  2  Delete building                      |\n");
    printf("|  3  Search building and update           |\n");
    printf("|  4  List all buildings                   |\n");
    printf("|  5  List buildings with apartments > x   |\n");
    printf("|  6  List buildings with unpaid fees      |\n");
    printf("|  7  Save AVL to file & create hash table |\n");
    printf("|------------------------------------------ |\n");
    printf("|           --- Hash Table ---              |\n");
    printf("|  8  Insert building                      |\n");
    printf("|  9  Delete building                      |\n");
    printf("|  10 Search building                      |\n");
    printf("|  11 Print hash table                     |\n");
    printf("|  12 Print size and load factor           |\n");
    printf("|  13 Save hash to file                    |\n");
    printf("|------------------------------------------ |\n");
    printf("|  14 Exit & Save                          |\n");
    printf("+===========================================+\n");
    printf("  Enter choice: ");
}

// main function starts the program and controls the menu
// it saves the hash table before exit
int main(){
    AVLNode buildingTree = NULL;
    HashEntry hashTable[53];
    int hashCount = 0;
    int choice;
    int apartmentLimit;

    InitializeHashTable(hashTable, TABLE_SIZE); // make the hash table empty at the start

    do{
        PrintMenu();
        choice = ReadInteger(""); // read the menu choice

        if(choice == 0){
            buildingTree = MakeEmpty(buildingTree); // delete old tree before reading again
            ReadInfoFile(&buildingTree);
        }
        else if(choice == 1)
            InsertBuildingOption(&buildingTree);
        else if(choice == 2)
            DeleteBuildingOption(&buildingTree);
        else if(choice == 3)
            FindAndUpdateOption(buildingTree);
        else if(choice == 4){
            printf("\nAll buildings in alphabetical order:\n");
            PrintInOrder(buildingTree);
        }
        else if(choice == 5){
            apartmentLimit = ReadInteger("Enter number of apartments: ");
            PrintGreaterApartments(buildingTree, apartmentLimit);
        }
        else if(choice == 6){
            printf("\nBuildings with unpaid fees:\n");
            PrintUnpaidBuildings(buildingTree);
        }
        else if(choice == 7){
            SaveAVLToHashFile(buildingTree); // save AVL data to hash.txt
            LoadHashFromFile(hashTable, TABLE_SIZE, &hashCount); // create hash table from hash.txt
        }
        else if(choice == 8)
            InsertHashOption(hashTable, TABLE_SIZE, &hashCount);
        else if(choice == 9)
            DeleteHashOption(hashTable, TABLE_SIZE, &hashCount);
        else if(choice == 10)
            SearchHashOption(hashTable, TABLE_SIZE);
        else if(choice == 11)
            PrintHashTable(hashTable, TABLE_SIZE);
        else if(choice == 12)
            PrintHashInfo(TABLE_SIZE, hashCount);
        else if(choice == 13)
            SaveHashToFile(hashTable, TABLE_SIZE);
        else if(choice == 14){
            printf("Saving hash table data before exit\n");
            SaveHashToFile(hashTable, TABLE_SIZE); // save hash table before exit
            printf("Goodbye!\n");
        }
        else
            printf("Invalid choice Please try again\n");

    }while(choice != 14);

    buildingTree = MakeEmpty(buildingTree); // delete the AVL tree before exit
    return 0;
}
