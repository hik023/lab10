#include <iostream>
#include "stdlib.h"
#include <vector>
#include <time.h>
#include <random>
#include <fstream>
#include <string>

using namespace std;

static vector<vector<int>> population;
const int n = 15, Wmax = 106;
static int c[n], w[n];
static int c_all;


int Weight(vector<int> genom){
    int all_weigth = 0;
    for (int i = 0; i < genom.size(); i++) {
        all_weigth += genom[i] == 1 ? w[i] : 0;
    }
    return all_weigth;
}

int Price(vector<int> genom){
    int all_price = 0;
    for (int i = 0; i < genom.size(); i++) {
        all_price += genom[i] == 1 ? c[i] : 0;
    }
    return all_price;
}

void PrintPopulation(){
    for(int i = 0; i < population.size(); i++) {
        for (int j = 0; j < population[i].size(); j++) {
            cout << population[i][j] << ' ';
        }
        cout << '\t' << Price(population[i]);
        cout << "\t\t" << Weight(population[i]);
        cout << '\n';
    }
}

void Mutation(int population_size){
    int is_mutate;
    for (int i = population_size; i < population.size(); i++) {
        is_mutate = random() % 100 < 10 ? true : false;
        if (is_mutate){
            population[i][random() % population[i].size()] = population[i][random() % population[i].size()] == 0 ? 1 : 0;
        }
    }
}

void Crossover1(int population_size){
    int p1_ind = random() % population.size();
    int p2_ind = random() % population.size();
    while (p2_ind == p1_ind){
        p2_ind = random() % population.size();
    }
    vector<int> parent1 = population[p1_ind];
    vector<int> parent2 = population[p2_ind];
    vector<int> child(parent1);

    for (int i = 0; i < child.size(); i++) {
        int p_num = random() % 2 + 1;
        child[i] = p_num == 1 ? parent1[i] : parent2[i];
    }

    population.insert(population.end(), child);
}

void Crossover2(int population_size){
    int slice_ind = random() % (population[0].size()-4) + 2;
    int p1_ind = random() % population.size();
    int p2_ind = random() % population.size();
    while (p2_ind == p1_ind){
        p2_ind = random() % population.size();
    }
    vector<int> parent1 = population[p1_ind];
    vector<int> parent2 = population[p2_ind];
    vector<int> child(parent1);

    for (int i = 0; i < child.size(); i++) {
        child[i] = i <= slice_ind ? parent1[i] : parent2[i];
    }

    population.insert(population.end(), child);
}

void Selection1(int population_size){
    vector<vector<int>> population_new;
    for (int i = 0; i < population.size()-1; i+=2) {
        population_new.insert(population_new.end(), Price(population[i]) < Price(population[i+1]) ? population[i+1] : population[i]);
    }
    int d = population.size() - population_new.size();
    for (int j = 0; j < d; j++) {
        population.pop_back();
    }

    for (int k = 0; k < population.size(); k++) {
        population[k] = population_new[k];
    }

}

void Selection2(int population_size){
    vector<vector<int>> population_new;
    for (int i = 0; i < population.size(); i++) {
        if (random() % c_all < Price(population[i])) population_new.insert(population_new.end(), population[i]);
    }
    int d = population.size() - population_new.size();
    for (int j = 0; j < d; j++) {
        population.pop_back();
    }

    for (int k = 0; k < population.size(); k++) {
        population[k] = population_new[k];
    }
}

void Corrector(){
    for (int i = 0; i < population.size(); i++) {
        if(Weight(population[i]) > Wmax){
            for (int j = 0; j < population[i].size(); j++) {
                population[i][j] = 0;
                if (Weight(population[i]) < Wmax) break;
            }
        }
    }
}

int main() {
    int genom_size, population_size, pop_num, step = 0, cross_num, selection_num;
    srand(time(NULL));
    cout << "Введите кол-во генов: ";
    cin >> genom_size;
    cout << "Введите размер популяции: ";
    cin >> population_size;
    cout << "Введите количество популяций:  ";
    cin >> pop_num;
    cout << "Введите номер кроссовера:  ";
    cin >> cross_num;
    cout << "Введите номер селекции:  ";
    cin >> selection_num;


    for (int i = 0; i<n; i++)
    {
        c[i] = random() % 30 + 1;
        c_all += c[i];
        w[i] = random() % 30 + 1;
    }

    cout << "Популяция " << step << '\n';
    cout << "Геном\t\t\t\t\tЦена\tВес\n";
    step ++;
    for(int i = 0; i < population_size; i++) {
        vector<int> genom(genom_size);
        population.insert(population.end(), genom);

        for (int j = 0; j < genom_size; j++) {
            population[i][j] = random() % 2;
        }
    }
    PrintPopulation();
    for (int l = 0; l < pop_num; l++) {
        cout << '\n';
        cout << '\n';
        cout << '\n';
        cout << '\n';
        cross_num == 1 ? Crossover1(population_size) : Crossover2(population_size);

        Mutation(population_size);
        selection_num == 1 ? Selection1(population_size) : Selection2(population_size);
        int d = population_size - population.size();
        for (int k = 0; k < d; k++) {
            Crossover1(population_size);
        }
        Corrector();
        cout << "Популяция " << step << '\n';
        cout << "Геном\t\t\t\t\tЦена\tВес\n";
        step ++;
        PrintPopulation();
    }

    return 0;
}