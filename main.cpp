#include <iostream>
#include <string>
#include <stdbool.h>
#include <vector>
#include <ostream>
#include <algorithm>
#include <iterator>
#include <time.h>
#include <random>

using namespace std;

int random(int min, int max){
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<mt19937::result_type> dist(min,max);

    return (int)dist(rng); 
}

bool chance(int perc){
    int r = random(1,100);

    if(r <= perc){
        return true;
    }
    else{
        return false;
    }
}

class Soldado
{

protected:
    string nome;
    float saude, poder;

public:
    Soldado(string nome, float s, float p) : nome(nome), saude(s), poder(p){}

    virtual void atacar(Soldado &s)
    {   
        int RANGE = random(0, (int)poder);
        s.defender(random(poder - RANGE, poder + RANGE));
    }

    virtual void defender(float p)
    {
        saude -= p;
    }

    bool morreu(){
        if(saude <= 0.0){
            return true;
        }
        else{
            return false;
        }
    }

    string getNome(){return nome;}
};

class Elfo : public Soldado
{
public:
    Elfo(string nome, float s, float p) : Soldado(nome, s, p + 1) {}
};

class Anao : public Soldado
{
public:
    Anao(string nome, float s, float p) : Soldado(nome, s, p + 20) {}

    void atacar(Soldado &s)
    {
        if (chance(40))
        {
            cout << "missed" << endl;
            return;
        }
        else
        {
            s.defender(poder);
        }
    }
};

class Humano : public Soldado
{
public:
    Humano(string nome, float s, float p) : Soldado(nome, s, p) {}

    void atacar(Soldado &s)
    {
        if (chance(10))
        {
            s.defender(poder);
            s.defender(poder);
        }
        else
        {
            s.defender(poder);
        }
        return;
    }
};

class Sauron : public Soldado
{
public:
    Sauron(string nome, float s, float p) : Soldado(nome, 10 * s, p) {}

    void atacar(Soldado &s)
    {
        if (chance(30))
        {
            s.defender(2 * poder);
        }
        else
        {
            s.defender(poder);
        }
        return;
    }
};

class Orc : public Soldado
{
public:
    Orc(string nome, float s, float p) : Soldado(nome, s, p) {}

    void atacar(Soldado &s)
    {
        if (chance(20))
        {
            s.defender(1.1 * poder);
            s.defender(1.1 * poder);
        }
        else
        {
            s.defender(poder);
        }
        return;
    }
};

class Mago : public Soldado
{
public:
    Mago(string nome, float s, float p) : Soldado(nome, s, p + 30) {}

    void defender(float p) // 10% de chance de bloquear um ataque com um escudo mágico
    {
        if (chance(10))
        {
            cout << "blocked" << endl;
            return;
        }
        else
        {
            saude -= p;
        }
    }
};

class Confronto{
public:
    vector <Soldado> bem; 
    vector <Soldado> mal; 
    
    void guerra(){

        vector <Soldado>::iterator bem_it;
        vector <Soldado>::iterator mal_it;

        while (!bem.empty() && !mal.empty())
        {   
            random_shuffle(bem.begin(), bem.end());
            random_shuffle(mal.begin(), mal.end());
            bem_it = bem.begin();
            mal_it = mal.begin();

            while(mal_it != mal.end() && bem_it != bem.end()){
                int i = random(0,1);
                while(!(*mal_it).morreu() && !(*bem_it).morreu()){
                    if(i % 2 == 0){(*mal_it).atacar(*bem_it);}
                    else{(*bem_it).atacar(*mal_it);}
                    i++;
                }

                if((*mal_it).morreu())
                    mal_it = mal.erase(mal_it);
                    bem_it++;

                if((*bem_it).morreu())
                    bem_it = bem.erase(bem_it);
                    mal_it++;
            }
        }
        
        if(bem.empty()){
            cout << "o mal venceu";
            return;
        }
        else if(mal.empty()){
            cout << "o bem venceu";
            return;
        }
    }
};


int main()
{   
    cout << random(0,1) << endl;
}