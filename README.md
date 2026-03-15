# Social Network Simulator (C++)

A command-line **social network simulator** built in **C++** that supports user creation, friendships, friend recommendations, post management, and graph queries such as **degrees of separation**.

The project demonstrates the use of **AVL Trees, Graphs, and Breadth First Search (BFS)** to model a simplified social media backend.

---

## Features

### User Management
Create users in the network.

Command:
ADD_USER <username>

Example:
ADD_USER alice

---

### Friendships
Create bidirectional friendships between users.

Command:
ADD_FRIEND <username1> <username2>

Example:
ADD_FRIEND alice bob

---

### List Friends
Display all friends of a user.

Command:
LIST_FRIENDS <username>

---

### Friend Suggestions
Suggest potential friends based on **mutual connections**.

Command:
SUGGEST_FRIENDS <username> <N>

Outputs the top **N recommended users**.

---

### Degrees of Separation
Find the shortest connection between two users using **BFS**.

Command:
DEGREES_OF_SEPARATION <username1> <username2>

Returns the number of hops between users or `-1` if no connection exists.

---

### Post System
Users can create posts.

Command:
ADD_POST <username> <post content>

---

### View Posts
Display recent posts of a user.

Command:
OUTPUT_POSTS <username> <N>

`N = -1` displays all posts.

---

## Data Structures Used

- **AVL Trees**  
  - Store friend lists  
  - Store posts ordered by timestamp  

- **Graph Representation**  
  - `unordered_map<string, AVLtree>` used to store user connections

- **Breadth First Search (BFS)**  
  - Used for computing degrees of separation

---

## Compilation

Compile:

g++ social_network.cpp -o social_network

Run:

./social_network

---

## Example

ADD_USER alice  
ADD_USER bob  

ADD_FRIEND alice bob  

ADD_POST alice hello world  

LIST_FRIENDS alice  

OUTPUT_POSTS alice 1  

EXIT

---

## Concepts Demonstrated

- AVL Tree implementation
- Graph traversal (BFS)
- Friend recommendation using mutual connections
- Command-line interface design
- Data structure based system simulation

---

## Author

Priyanshu Kumar
