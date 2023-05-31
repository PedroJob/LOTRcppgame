#include <iostream>
#include <string>
#include <stdbool.h>
#include <vector>
#include <ostream>
#include <algorithm>
#include <iterator>
#include <time.h>
#include <random>
#include <windows.h>

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
        int RANGE = random(0, 10);
        int dano = random((int)poder - RANGE, (int)poder + RANGE);
        s.defender(dano);
    }

    virtual void defender(float p)
    {
        saude -= p;
        cout << nome << " perde " << p << " pontos de vida, restam: " << saude << endl;
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
    float getSaude(){return saude;}
    float getPoder(){return poder;}
};

ostream& operator<<(ostream &os, Soldado &s){
    os << s.getNome() << " (HP: " << s.getSaude() << ")";
    return os; 
}

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
            Soldado::atacar(s);
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
            cout << "Ataque duplo!" << endl;
            Soldado::atacar(s);
            Soldado::atacar(s);
        }
        else
        {
            Soldado::atacar(s);
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
        float aux = poder; 
        if (chance(30))
        {   
            cout << "Dobro de ataque!" << endl;
            poder *= 2;
            Soldado::atacar(s);
        }
        else
        {
            Soldado::atacar(s);
        }

        poder = aux;
        return;
    }
};

class Orc : public Soldado
{
public:
    Orc(string nome, float s, float p) : Soldado(nome, s, p) {}

    void atacar(Soldado &s)
    {
        float aux = poder;
        if (chance(20))
        {
            cout << "Ataque duplo!" << endl;
            poder *= 1.1; 
            Soldado::atacar(s);
            Soldado::atacar(s);
        }
        else
        {
            Soldado::atacar(s);
        }

        poder = aux;
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
            cout << "o mago bloqueou o ataque, ";
            Soldado::defender(0);
            return;
        }
        else
        {
            Soldado::defender(p);
        }
    }
};

class Confronto{
public:
    vector <Soldado*> bem; 
    vector <Soldado*> mal; 
    
    void guerra(){

        vector <Soldado*>::iterator bem_it;
        vector <Soldado*>::iterator mal_it;

        while (!bem.empty() && !mal.empty())
        {   
            srand(time(NULL));
            random_shuffle(bem.begin(), bem.end());
            random_shuffle(mal.begin(), mal.end());
            bem_it = bem.begin();
            mal_it = mal.begin();

            while(mal_it != mal.end() && bem_it != bem.end()){
                int i = random(0,1);
                cout << "#########################################################" << endl;
                cout << (**mal_it) << " versus " << (**bem_it) << endl;
                while(!(**mal_it).morreu() && !(**bem_it).morreu()){
                    if(i % 2 == 0){
                        cout << "-> " << (**mal_it).getNome() << " ataca: " << endl;
                        (**mal_it).atacar(**bem_it);
                    }
                    else{
                        cout <<"-> "<< (**bem_it).getNome() << " ataca: " << endl;
                        (**bem_it).atacar(**mal_it);
                    }
                    i++;
                    Sleep(2000);
                }

                if((**mal_it).morreu()){
                    cout << (**bem_it).getNome() << " vence" << endl;
                    cout << (**mal_it).getNome() << " morreu" << endl;
                    mal_it = mal.erase(mal_it);
                    bem_it++;
                }
                else if((**bem_it).morreu()){
                    cout << (**mal_it).getNome() << " vence" << endl;
                    cout << (**bem_it).getNome() << " morreu" << endl;
                    bem_it = bem.erase(bem_it);
                    mal_it++;
                }

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
    Mago job("job", 100, 50);
    Sauron thais("tata", 100, 20);
    Orc velosum("velosum", 100, 30);
    Humano bilau("bilau", 100, 40);
    Orc super("super", 100, 35);
    Elfo elf("elf", 100, 50);

    Confronto c;

    c.bem.push_back(&job);
    c.bem.push_back(&bilau);
    c.bem.push_back(&elf);
    c.mal.push_back(&super);
    c.mal.push_back(&thais);
    c.mal.push_back(&velosum);

    c.guerra();
}