# Data Structures Project 2 — Building Information Management System

## 1. Project Description and Data Structures Used

### Project Description

This project implements a Building Information Management System that maintains information about buildings using an AVL Tree and a Hash Table.

The program reads building information from a file named `info.txt`. Each building record contains the building name, building number, address, number of apartments, establishment year, and whether the building has paid its fees.

The application provides the following functionalities:

- Read building information from `info.txt`.
- Create an AVL Tree using the building names as keys.
- Insert a new building into the AVL Tree.
- Search for a building and update its information.
- List all buildings in alphabetical order.
- List buildings with more than a specified number of apartments.
- List buildings that have not paid their fees.
- Delete a building from the AVL Tree.
- Save the AVL Tree information to `hash.txt`.
- Create a Hash Table using the data stored in `hash.txt`.
- Insert a new building into the Hash Table.
- Search for a building and display the number of collisions.
- Delete a building from the Hash Table.
- Display the complete Hash Table including empty and deleted positions.
- Display the Hash Table size and load factor.
- Save the final Hash Table data back to `hash.txt`.

### Data Structures Used

The project uses the following data structures:

- **AVL Tree:** Used to store and manage building records while maintaining the tree in balanced form.
- **Binary Search Tree Operations:** Building names are used to organize the AVL Tree alphabetically.
- **Hash Table:** Used to store building records after the AVL Tree data is saved to `hash.txt`.
- **Double Hashing:** Used as the open addressing technique for resolving collisions in the Hash Table.
- **Structures:** Used to represent building records, AVL Tree nodes, and Hash Table entries.
- **File Handling:** Used to read building information from `info.txt` and save the processed information to `hash.txt`.

### Building Record

Each building contains the following information:

- Building name.
- Building number.
- Building address.
- Number of apartments.
- Establishment year.
- Paid fees status.

The input format is:

    Name:number:address:number_of_apartments:establishment_year:paid_fees

Example:

    Alquds:20:Al Marj:15:1999:yes
    Jbeneh:121:Nishan street:15:2012:no
    Iskan Al Andalus:120:Gabi Baramke:10:2020:no
    Al Rabee 1:512:Rabee Main Street:10:2021:yes
    Al Rabee 2:513:Rabee Main Street:25:2024:yes

---

## 2. Application Menu

The application provides a menu that allows the user to perform operations on both the AVL Tree and the Hash Table.

### Menu

    +===========================================+
    |     Building Information Maintainer       |
    +===========================================+
    |            --- AVL Tree ---               |
    |  0  Read file and create tree             |
    |  1  Insert building                       |
    |  2  Delete building                       |
    |  3  Search building and update            |
    |  4  List all buildings                    |
    |  5  List buildings with apartments > x    |
    |  6  List buildings with unpaid fees       |
    |  7  Save AVL to file & create hash table  |
    |-------------------------------------------|
    |           --- Hash Table ---              |
    |  8  Insert building                       |
    |  9  Delete building                       |
    |  10 Search building                       |
    |  11 Print hash table                      |
    |  12 Print size and load factor            |
    |  13 Save hash to file                     |
    |-------------------------------------------|
    |  14 Exit & Save                           |
    +===========================================+

    Enter choice:

### AVL Tree Menu Options

#### 0. Read File and Create Tree

The program reads the building records from `info.txt` and inserts them into an AVL Tree.

The building name is used as the key for organizing the tree.

The AVL Tree automatically performs the required rotations to maintain balance after insertion.

#### 1. Insert Building

The user enters all information for a new building:

- Name.
- Building number.
- Address.
- Number of apartments.
- Establishment year.
- Paid fees status.

The new building is inserted into the AVL Tree according to its name.

Duplicate building names are not inserted.

#### 2. Delete Building

The user enters the name of a building to delete.

If the building exists, it is removed from the AVL Tree and the tree is rebalanced.

If the building does not exist, an appropriate message is displayed.

#### 3. Search Building and Update

The user enters the name of a building.

If the building is found, its current information is displayed and the user can choose whether to update it.

The following information can be updated:

- Building number.
- Address.
- Number of apartments.
- Paid fees status.

The establishment year remains unchanged during the update.

#### 4. List All Buildings

All buildings stored in the AVL Tree are displayed in alphabetical order.

This is achieved using an inorder traversal of the AVL Tree.

#### 5. List Buildings with Apartments Greater Than a Given Number

The user enters a number of apartments.

The program displays all buildings whose number of apartments is greater than the given number.

#### 6. List Buildings with Unpaid Fees

The program searches through the AVL Tree and displays all buildings whose fees have not been paid.

Both `no` and `n` are treated as unpaid.

#### 7. Save AVL Data and Create Hash Table

The current AVL Tree information is saved to:

    hash.txt

The program then reads the data from `hash.txt` and uses it to create the Hash Table.

---

### Hash Table Menu Options

#### 8. Insert Building

The user enters a new building record.

The building is inserted into the Hash Table using the implemented hashing technique.

If the building already exists, its information is updated.

#### 9. Delete Building

The user enters the name of a building to delete.

If the building exists, its Hash Table position is marked as deleted.

The deleted position can later be reused by another insertion.

#### 10. Search Building

The user enters the name of a building to search for.

The program searches for the building using the Hash Table probing sequence.

If the building is found, the program displays:

- The building information.
- The position of the building in the Hash Table.
- The number of collisions required to reach the building.

#### 11. Print Hash Table

The program prints the entire Hash Table, including:

- Active records.
- Empty positions.
- Deleted positions.

Each position is displayed with its corresponding index.

#### 12. Print Hash Table Size and Load Factor

The program displays:

- Hash Table size.
- Number of stored records.
- Load factor.

The load factor is calculated as:

    Load Factor = Number of Records / Hash Table Size

#### 13. Save Hash Table

The active records in the Hash Table are saved to:

    hash.txt

Only active records are written to the file.

#### 14. Exit and Save

Before exiting, the program saves the current Hash Table data to `hash.txt`.

The program then terminates.

---

## 3. AVL Tree, Hashing, Converting and Storing Details

### 3.1 AVL Tree

The AVL Tree is used as the main data structure for managing building information.

Each AVL Tree node contains:

- Building information.
- Left child.
- Right child.
- Height.

The building name is used as the key for ordering the nodes.

The AVL Tree maintains balance after insertion and deletion.

The balance condition is maintained by applying rotations when necessary.

### AVL Rotations

The following rotations are used:

- Single Rotation with Left Child.
- Single Rotation with Right Child.
- Double Rotation with Left Child.
- Double Rotation with Right Child.

These rotations maintain the AVL Tree balance and keep searching, insertion, and deletion efficient.

### AVL Search

Building names are compared using alphabetical order.

If the searched name is smaller than the current node, the search continues in the left subtree.

If the searched name is greater than the current node, the search continues in the right subtree.

If the names are equal, the building is found.

### AVL Inorder Traversal

The inorder traversal is used to list all buildings alphabetically.

The traversal follows:

    Left -> Root -> Right

Since the building name is used as the key, the resulting list is in alphabetical order.

---

### 3.2 Hash Table

After the AVL Tree data is saved to `hash.txt`, the program creates a Hash Table using the saved building records.

The Hash Table uses open addressing with double hashing for collision resolution.

The implemented Hash Table size is:

    53

The table stores the status of every position as:

- `EMPTY`
- `ACTIVE`
- `DELETED`

### Hash Table Key

Only the first four characters of the building name are used to generate the key.

The key value is calculated using the character values of the first four characters.

For example, if the building name is:

    Alquds

The characters considered for the key are:

    A l q u

The resulting key value is then used by the hash functions.

### Primary Hash Function

The first hash function calculates the initial position using:

    Hash1 = KeyValue(name) % TableSize

With a table size of 53:

    Hash1 = KeyValue(name) % 53

### Secondary Hash Function

The second hash function determines the step used during probing.

The implemented prime number is:

    7

The secondary hash function is:

    Hash2 = 7 - (KeyValue(name) % 7)

### Double Hashing

When a collision occurs, double hashing is used to calculate the next position.

The probing formula is:

    Index = (Hash1 + i * Hash2) % TableSize

where:

- `Hash1` is the initial hash position.
- `Hash2` is the probing step.
- `i` is the number of probing attempts.
- `TableSize` is the size of the Hash Table.

This process continues until the required building is found or an available position is reached.

---

### 3.3 Collision Handling

A collision occurs when two different building names are mapped to the same Hash Table position.

Instead of using separate chaining, the project uses open addressing with double hashing.

During a search, the program counts collisions whenever an active position contains a different building.

The number of collisions is displayed when searching for a building.

Example:

    Building found at index 15
    Number of collisions to reach it: 2

---

### 3.4 Hash Table Insertion

When inserting a building into the Hash Table:

1. The key is generated using the first four characters of the building name.
2. The primary hash position is calculated.
3. If the position is occupied by another building, a collision occurs.
4. The secondary hash function determines the next position.
5. The probing sequence continues until an available position is found.
6. A previously deleted position can also be reused.
7. The building record is stored in the selected position.

If a building with the same name already exists, its information is updated.

---

### 3.5 Hash Table Search

When searching for a building:

1. The key is generated from the first four characters of the name.
2. The primary hash position is calculated.
3. The building at that position is checked.
4. If the building is not found and the position contains another active record, a collision is counted.
5. The secondary hash value is used to calculate the next position.
6. The process continues until the building is found or the search reaches an empty position.

The program displays the number of collisions required to reach the building.

---

### 3.6 Hash Table Deletion

When a building is deleted from the Hash Table, its position is not simply marked as empty.

Instead, the position is marked as:

    DELETED

This allows future searches to continue correctly through the probing sequence.

Deleted positions can also be reused during future insertions.

---

### 3.7 Load Factor

The load factor represents how much of the Hash Table is currently occupied.

It is calculated using:

    Load Factor = Number of Active Records / Hash Table Size

For example, if the Hash Table contains 10 active records and its size is 53:

    Load Factor = 10 / 53

    Load Factor = 0.19

The program displays the table size, number of records, and load factor.

---

### 3.8 Storing Details

For each building, the program stores:

- Building name.
- Building number.
- Address.
- Number of apartments.
- Establishment year.
- Paid fees status.

The building information is initially read from:

    info.txt

After processing the AVL Tree, the building records are saved to:

    hash.txt

The Hash Table is then created using the information stored in `hash.txt`.

After Hash Table operations are completed, the active records are saved back to:

    hash.txt

### File Format

Each building record is stored using the following format:

    Name:number:address:number_of_apartments:establishment_year:paid_fees

Example:

    Alquds:20:Al Marj:15:1999:yes
    Jbeneh:121:Nishan street:15:2012:no
    Iskan Al Andalus:120:Gabi Baramke:10:2020:no
    Al Rabee 1:512:Rabee Main Street:10:2021:yes
    Al Rabee 2:513:Rabee Main Street:25:2024:yes

---

### 3.9 Complete Processing Flow

The complete processing of the project follows these steps:

    info.txt
        |
        v
    Read Building Records
        |
        v
    Create AVL Tree
        |
        v
    Insert / Search / Update / Delete
        |
        v
    List Building Information
        |
        v
    Save AVL Data
        |
        v
    hash.txt
        |
        v
    Create Hash Table
        |
        v
    Insert / Search / Delete
        |
        v
    Print Hash Table
        |
        v
    Calculate Load Factor
        |
        v
    Save Final Data
        |
        v
    hash.txt

---

## Project Structure

    Data-Structures-Project-2/
    │
    ├── README.md
    ├── main.c
    ├── info.txt
    ├── hash.txt
    └── .gitignore

---


**Output File:** `hash.txt`

**Deadline:** Sunday, June 21, 2026 — 8:00 PM
