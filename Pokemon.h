//
//  Pokemon.h
//  pokemon
//
//  Created by Damla Nisa Cevik on 22.03.2020.
//  Copyright © 2020 Damla Nisa Cevik. All rights reserved.
//

#ifndef Pokemon_h
#define Pokemon_h

#include "Atack.h"
#include <vector>

using namespace std;

class Pokemon{
public:
    vector<Atack> atackStats;
    int HP;
    int PP = 100;
    
    Pokemon(int HP): PP(100) {
        this->HP = HP;
    }
};


#endif /* Pokemon_h */
