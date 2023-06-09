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
#include <fstream>

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
    int chance_contra_ataque;

public:
    Soldado(string nome, float s, float p, int chance = 0) : nome(nome), saude(s), poder(p), chance_contra_ataque(chance){}

    virtual void atacar(Soldado &s)
    {   
        if(chance(s.chance_contra_ataque)){
            cout << "Contra-ataque!" << endl;
            this->defender(s.getPoder());
            return;
        }
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
        float aux = poder;

        if (chance(40))
        {
            cout << "O anao errou o ataque :(" << endl;
            poder = 0;
            Soldado::atacar(s);
            poder = aux;
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

class Gollum : public Soldado{
public:
    //Gollum tem 20% de chance de contra-atacar "MY PRECIOUSSS"
    Gollum(string nome, float s, float p) : Soldado(nome, s, p, 20){}
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

class Mago : public Soldado //o mago tem um poderoso escudo áureo
{
public:
    Mago(string nome, float s, float p) : Soldado(nome, s, p + 30) {}

    void defender(float p) // 10% de chance de bloquear um ataque com seu escudo mágico
    {
        if (chance(10)) //toda vez que o mago bloqueia um ataque, aumenta sua chance de contra-ataque
        {
            cout << "O mago bloqueou o ataque, ";
            Soldado::defender(0);
            chance_contra_ataque += 15;
            return;
        }
        else
        {
            Soldado::defender(p);
        }
    }
};

class Ladrao: public Soldado{
public:
    Ladrao(string nome, float s, float p) : Soldado(nome, s, p, 20) {}

    void defender(float p) //20% de chance de desviar do ataque e de dar ataque duplo
    {
        if(chance(20)){
            cout << "O ladrao desviou do golpe, ";
            Soldado::defender(0);
            return;
        }
        else{
            Soldado::defender(p);
        }
    }

    void atacar(Soldado &s)
    {
        float aux = poder;
        if (chance(20))
        {
            cout << "Ataque duplo!" << endl; 
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

class Confronto{
public:
    vector <Soldado*> bem; 
    vector <Soldado*> mal; 

    void print(vector <Soldado *> army){
        vector <Soldado*>::iterator army_it ;

        for(army_it = army.begin(); army_it != army.end(); army_it++){
            cout << (**army_it) << " | ";
        }
        cout << endl;
        return;
    }

    void intro(){
        cout << "Exercito do bem: ";
        print(bem);
        cout << endl;
        Sleep(2000);
        cout << "Exercito do mal: ";
        print(mal);
        cout << endl;
        Sleep(2000);
        cout << "QUE COMECEM OS JOGOS!" << endl;
        Sleep(2000);
        system("cls");
    }

    void guerra(){
        intro();
        //início randomico da batalha:
        srand(time(NULL));
        random_shuffle(mal.begin(), mal.end());
        random_shuffle(bem.begin(), bem.end());

        vector <Soldado*>::iterator bem_it = bem.begin();
        vector <Soldado*>::iterator mal_it = mal.begin();  



        while (!bem.empty() && !mal.empty())
        {   
            srand(time(NULL)); //sempre gerando uma nova seed para deixar o mais aleatório possível

            if(mal_it == mal.end()){ //se chegou ao final, reseta e da shuffle, caso contrário, continua de onde parou (prioridade)
                random_shuffle(mal.begin(), mal.end());
                mal_it = mal.begin();
            }

            if(bem_it == bem.end()){
                random_shuffle(bem.begin(), bem.end());
                bem_it = bem.begin();
            }

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
                    cout << (**bem_it).getNome() << " vence, ";
                    cout << (**mal_it).getNome() << " morreu" << endl;
                    mal_it = mal.erase(mal_it);
                    bem_it++;
                }
                else if((**bem_it).morreu()){
                    cout << (**mal_it).getNome() << " vence, ";
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
    Mago job("job", 100, 30);
    Humano bilau("bilau", 100, 30);
    Gollum gollum("gollum", 100, 20);
    Anao anao("anao", 100, 20);
    Sauron thais("tata", 20, 30);
    Orc velosum("velosum", 100, 30);
    Ladrao ladrao("ladrao", 100, 20);
    

    Confronto c;

    c.bem.push_back(&job);
    c.bem.push_back(&bilau);
    c.bem.push_back(&gollum);
    c.bem.push_back(&anao);
    c.mal.push_back(&ladrao);
    c.mal.push_back(&thais);
    c.mal.push_back(&velosum);

    c.guerra();
}