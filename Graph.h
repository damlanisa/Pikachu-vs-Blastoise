//
//  Graph.h
//  pokemon
//
//  Created by Damla Nisa Cevik on 23.03.2020.
//  Copyright © 2020 Damla Nisa Cevik. All rights reserved.
//

#ifndef Graph_h
#define Graph_h

#include "Node.h"

class Graph{
public:
    int levelCount = 0;
    Node *root;
    Graph(){
        root = nullptr;
    }
    void BFS(Node*, int);
    void recursiveDFS(Node*, int, queue<Node*>&);
    void DFS(Node*, int);
    void easiestPathBFS(Node*, char);
};

#endif /* Graph_h */
