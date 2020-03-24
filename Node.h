//
//  Node.h
//  pokemon
//
//  Created by Damla Nisa Cevik on 22.03.2020.
//  Copyright © 2020 Damla Nisa Cevik. All rights reserved.
//

#ifndef Node_h
#define Node_h

#include <vector>

class Node{
public:
    int pikachuHP;
    int pikachuPP;
    int blastoiseHP;
    int blastoisePP;
    string atackName;
    bool isEffectiveAttack;
    char turn = 'P';
    float probabilty = 1;
    int level = 0;
    bool isLeaf= false;
    vector<Node*> children;
    Node *parent;
    
    Node(int pikachuHP, int pikachuPP, int blastoiseHP, int blastoisePP){
        this->pikachuHP = pikachuHP;
        this->pikachuPP = pikachuPP;
        this->blastoiseHP = blastoiseHP;
        this->blastoisePP = blastoisePP;
    }
    
    ~Node(){
        for (int i = 0; i < children.size(); i++) {
            delete children[i];
        }
    }
};

#endif /* Node_h */
