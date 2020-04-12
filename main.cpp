// Student Name: Damla Nisa CEVIK
// Student ID: 150170712

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <time.h>
#include <vector>
#include <queue>
#include <stack>
#include "Atack.h"
#include "Pokemon.h"
#include "Node.h"
#include "Graph.h"
using namespace std;

const int PIKACHU_HP = 200;
const int BLASTOISE_HP = 200;
const int POKEMON_PP = 100;
const int MAX_LEVEL_FOR_WIN = 10;


float calculateProbability(Node* node, Pokemon* pokemon){
    float probability = node->probabilty;
    float totalPossibleAttacks = 0;
    
    for (int i = 0; i < pokemon->atackStats.size(); i++)
        if (pokemon->atackStats[i].firstUse <= node->level + 1 && pokemon->atackStats[i].PP <= pokemon->PP) totalPossibleAttacks ++;
    
    return probability / totalPossibleAttacks;
}


void constructChildren(Node* node, Pokemon* pikachu, Pokemon* blastoise, int maxLevel){
    if (node->level == maxLevel) return;
    if (node->turn == 'P') {
        float effectiveProbability = calculateProbability(node, pikachu);
        for (int i = 0; i < pikachu->atackStats.size(); i++) {
            if (pikachu->atackStats[i].firstUse <= node->level + 1 && (pikachu->atackStats[i].PP * (-1)) <= node->pikachuPP && node->pikachuHP > 0) {
                if (pikachu->atackStats[i].accuracy != 100) {
                    Node* uneffectiveChild = new Node(node->pikachuHP, (node->pikachuPP + pikachu->atackStats[i].PP), node->blastoiseHP, node->blastoisePP);
                    uneffectiveChild->parent = node;
                    uneffectiveChild->level = node->level + 1;
                    uneffectiveChild->isEffectiveAttack = false;
                    uneffectiveChild->turn = 'B';
                    uneffectiveChild->atackName = pikachu->atackStats[i].name;
                    uneffectiveChild->probabilty = effectiveProbability - (effectiveProbability * pikachu->atackStats[i].accuracy / 100);
                    if (node->level + 1 == maxLevel) uneffectiveChild->isLeaf = true;
                    node->children.insert(node->children.end(), uneffectiveChild);
                    constructChildren(uneffectiveChild, pikachu, blastoise, maxLevel);
                }
                
                Node* effectiveChild = new Node(node->pikachuHP, (node->pikachuPP + pikachu->atackStats[i].PP), (node->blastoiseHP -                                                                 pikachu->atackStats[i].damage), node->blastoisePP);
                effectiveChild->parent = node;
                effectiveChild->level = node->level + 1;
                effectiveChild->isEffectiveAttack = true;
                effectiveChild->turn = 'B';
                effectiveChild->atackName = pikachu->atackStats[i].name;
                if (pikachu->atackStats[i].accuracy != 100) effectiveChild->probabilty = effectiveProbability * pikachu->atackStats[i].accuracy / 100;
                else effectiveChild->probabilty = effectiveProbability;
                if (node->level + 1 == maxLevel) effectiveChild->isLeaf = true;
                node->children.insert(node->children.end(), effectiveChild);
                constructChildren(effectiveChild, pikachu, blastoise, maxLevel);
            }
        }
    }
    else {
        float effectiveProbability = calculateProbability(node, blastoise);
        for (int i = 0; i < blastoise->atackStats.size(); i++) {
            if (blastoise->atackStats[i].firstUse <= node->level + 1 && (blastoise->atackStats[i].PP * (-1)) <= node->blastoisePP && node->blastoiseHP > 0) {
                if (blastoise->atackStats[i].accuracy != 100) {
                    Node* uneffectiveChild = new Node(node->pikachuHP, node->pikachuPP, node->blastoiseHP, (node->blastoisePP + blastoise->atackStats[i].PP));
                    uneffectiveChild->parent = node;
                    uneffectiveChild->level = node->level + 1;
                    uneffectiveChild->isEffectiveAttack = false;
                    uneffectiveChild->turn = 'P';
                    uneffectiveChild->atackName = blastoise->atackStats[i].name;
                    uneffectiveChild->probabilty = effectiveProbability - (effectiveProbability * blastoise->atackStats[i].accuracy / 100);
                    if (node->level + 1 == maxLevel) uneffectiveChild->isLeaf = true;
                    node->children.insert(node->children.end(), uneffectiveChild);
                    constructChildren(uneffectiveChild, pikachu, blastoise, maxLevel);
                }
                Node* effectiveChild = new Node((node->pikachuHP - blastoise->atackStats[i].damage), node->pikachuPP, node->blastoiseHP,
                                                (node->blastoisePP + blastoise->atackStats[i].PP));
                effectiveChild->parent = node;
                effectiveChild->level = node->level + 1;
                effectiveChild->isEffectiveAttack = true;
                effectiveChild->turn = 'P';
                effectiveChild->atackName = blastoise->atackStats[i].name;
                if (blastoise->atackStats[i].accuracy != 100) effectiveChild->probabilty = effectiveProbability * blastoise->atackStats[i].accuracy / 100;
                else effectiveChild->probabilty = effectiveProbability;
                if (node->level + 1 == maxLevel) effectiveChild->isLeaf = true;
                node->children.insert(node->children.end(), effectiveChild);
                constructChildren(effectiveChild, pikachu, blastoise, maxLevel);
            }
        }
    }
}

void buildGraph(Graph* graph, Pokemon* pikachu, Pokemon* blastoise, int graphLevel){
    Node* node = new Node(PIKACHU_HP, POKEMON_PP, BLASTOISE_HP, POKEMON_PP);
    node->atackName = "No attack";
    graph->root = node;
    constructChildren(graph->root, pikachu, blastoise, graphLevel);
    graph->levelCount = graphLevel;
}

void part1Output(Node* node, int maxLevel){
    if(node->level == maxLevel) cout << "P_HP:" << node->pikachuHP << " P_PP:" << node->pikachuPP << " B_HP:" << node->blastoiseHP << " B_PP:" <<                   node->blastoisePP << " PROB:" << node->probabilty << endl;
    else if (!node->children.empty()){
        for (int i = 0; i < node->children.size(); i++)
            part1Output(node->children[i], maxLevel);
    }
    else return;
}

void Graph::BFS(Node* node, int maxLevel){
    queue<Node*> visited;
    queue<Node*> waiting;
    bool isMaxLevelReached = false;
    
    clock_t time = clock();
   
    visited.push(node);
    for (int i = 0; i < node->children.size(); i++) {
        waiting.push(node->children[i]);
    }
    
    while (!waiting.empty() && isMaxLevelReached == false) {
        for (int i = 0; i < waiting.front()->children.size(); i++) {
            waiting.push(waiting.front()->children[i]);
            if (waiting.front()->children[i]->level > maxLevel) {
                isMaxLevelReached = true;
                break;
            }
        }
        
        if (isMaxLevelReached == false) {
            visited.push(waiting.front());
            waiting.pop();
        }
    }
    time = clock() - time;
    
    cout << "Number of nodes: " << visited.size() << ", running time: " << ((float)time)/CLOCKS_PER_SEC << endl;
}

void Graph::recursiveDFS(Node* node, int maxLevel, queue<Node*>& visited) {
    if (node->level > maxLevel) return;
    
    visited.push(node);
    for (int i = 0; i < node->children.size(); i++) {
        recursiveDFS(node->children[i], maxLevel, visited);
    }
}

void Graph::DFS(Node* node, int maxLevel){
    queue<Node*> visited;
    
    clock_t time = clock();
    
    recursiveDFS(node, maxLevel, visited);
    
    time = clock() - time;
        
    cout << "Number of nodes: " << visited.size() << ", running time: " << ((float)time)/CLOCKS_PER_SEC << endl;
}

void Graph::easiestPathBFS(Node* node, char typeSelect){
    queue<Node*> waiting;
    stack<Node*> attackSequence;
    Node* wanted = node;
    Node* temp = wanted;

    if (typeSelect == 'p'){
        for (int i = 0; i < node->children.size(); i++) {
            waiting.push(node->children[i]);
        }
        while (!waiting.empty()) {
            if (waiting.front()->blastoiseHP <= 0) {
                wanted = waiting.front();
                break;
            }
            for (int i = 0; i < waiting.front()->children.size(); i++)
                waiting.push(waiting.front()->children[i]);
            
            waiting.pop();
        }
    }
    
    else{
        for (int i = 0; i < node->children.size(); i++) {
            waiting.push(node->children[i]);
        }
        while (!waiting.empty()) {
            if (waiting.front()->pikachuHP <= 0) {
                wanted = waiting.front();
                break;
            }
            for (int i = 0; i < waiting.front()->children.size(); i++)
                waiting.push(waiting.front()->children[i]);
            
            waiting.pop();
        }
    }
    
    attackSequence.push(wanted);
    temp = wanted;
    for (int i = 0; i < wanted->level; i++) {
        attackSequence.push(temp->parent);
        temp = temp->parent;
    }
    unsigned long size = attackSequence.size();
    attackSequence.pop();
    for (int i = 1; i < size ; i++) {
        Node* print = attackSequence.top();
        string attacker;
        string isEffective;
        if (print->turn == 'B') attacker = "Pikachu";
        else attacker = "Blastoise";
        if (print->isEffectiveAttack == true) isEffective = "effective";
        else isEffective = "uneffective";
        cout << attacker <<  " used " << print->atackName << ". It's " << isEffective << endl;
        attackSequence.pop();
    }
    cout << "Level Count: " << wanted->level << endl << "Probability: " << wanted->probabilty << endl;
}



void readFile(const char* firstFileName, const char* secondFileName, int part, char typeSelect, int maxLevel){
    ifstream pikachuFile;
    pikachuFile.open(firstFileName);
    Pokemon* pikachu = new Pokemon(PIKACHU_HP);
    if(pikachuFile.is_open() && pikachuFile.good()){
        string line;
        char delimiter = ',';
        string token;
        getline(pikachuFile, line);
        while(getline(pikachuFile, line)){
            Atack pikachuAtack;
            stringstream ss(line);
            getline(ss, token, delimiter);
            pikachuAtack.name = token;
            getline(ss, token, delimiter);
            pikachuAtack.PP = stoi(token);
            getline(ss, token, delimiter);
            pikachuAtack.accuracy = stoi(token);
            getline(ss, token, delimiter);
            pikachuAtack.damage = stoi(token);
            getline(ss, token, delimiter);
            pikachuAtack.firstUse = stoi(token);
            pikachu->atackStats.insert(pikachu->atackStats.end(), pikachuAtack);
        }
    }
    
    ifstream blastoiseFile;
    blastoiseFile.open(secondFileName);
    Pokemon* blastoise = new Pokemon(BLASTOISE_HP);
    if(blastoiseFile.is_open() && blastoiseFile.good()){
        string line;
        char delimiter = ',';
        string token;
        getline(blastoiseFile, line);
        while(getline(blastoiseFile, line)){
            Atack blastoiseAtack;
            stringstream ss(line);
            getline(ss, token, delimiter);
            blastoiseAtack.name = token;
            getline(ss, token, delimiter);
            blastoiseAtack.PP = stoi(token);
            getline(ss, token, delimiter);
            blastoiseAtack.accuracy = stoi(token);
            getline(ss, token, delimiter);
            blastoiseAtack.damage = stoi(token);
            getline(ss, token, delimiter);
            blastoiseAtack.firstUse = stoi(token);
            blastoise->atackStats.insert(blastoise->atackStats.end(), blastoiseAtack);
        }
    }
    
    Graph* graph = new Graph();
    
    if (part == 3) {
        buildGraph(graph, pikachu, blastoise, MAX_LEVEL_FOR_WIN);
        graph->easiestPathBFS(graph->root, typeSelect);
    }
    
    else {
        buildGraph(graph, pikachu, blastoise, maxLevel);
        if (part == 1) part1Output(graph->root, maxLevel);
            
        else if (part == 2){
            if(typeSelect == 'b') graph->BFS(graph->root, maxLevel);
            else graph->DFS(graph->root, maxLevel);
        }
    }
}


int main(int argc, const char * argv[]) {
    
    if(argc == 4){
        if (strcmp(argv[3], "bfs") == 0)
            readFile("pikachu.txt", "blastoise.txt", 2, 'b', stoi(argv[2]));
        else if(strcmp(argv[3], "dfs") == 0)
            readFile("pikachu.txt", "blastoise.txt", 2, 'd', stoi(argv[2]));
        else {
            cout << "Wrong command line arguments!" << endl;
            return -1;
        }
    }
    
    else if(argc == 3){
        if (strcmp(argv[1] ,"part1") == 0){
            readFile("pikachu.txt", "blastoise.txt", 1, 'n', stoi(argv[2]));
        }
        
        else if (strcmp(argv[1], "part3") == 0){
            if (strcmp(argv[2], "pikachu") == 0)
                       readFile("pikachu.txt", "blastoise.txt", 3, 'p', 0);
            else if (strcmp(argv[2], "blastoise") == 0)
                       readFile("pikachu.txt", "blastoise.txt", 3, 'b', 0);
            else {
                cout << "Wrong command line arguments!" << endl;
                return -1;
            }
        }
            
        else {
            cout << "Wrong command line arguments!" << endl;
        }
    }
    
    else {
        cout << "Wrong command line arguments!" << endl;
        return -1;
    }
    
    return 0;
}
