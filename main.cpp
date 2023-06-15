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

#define delay 0

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

struct Battle_log{
    static ofstream battle_log; 

    Battle_log(){
        if(!battle_log.is_open()){
            cout << "erro ao abrir log";
        }
    }
    Battle_log& operator<<(auto e){
        battle_log << e;
        cout << e;

        return *this;
    }
};

ofstream Battle_log::battle_log("battle_log.txt");

class Exercito; //forward declaration

class Soldado
{
friend class Exercito;
protected:
    string nome;
    float saude, poder;
    int chance_contra_ataque;
    Battle_log bl;

public:
    Soldado(string nome, float s, float p, int chance = 0) : nome(nome), saude(s), poder(p), chance_contra_ataque(chance){}

    virtual void atacar(Soldado &s)
    {   
        if(chance(s.chance_contra_ataque)){
            bl << "Contra-ataque!\n";
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
        if(morreu()){
            bl << "FATALITY!\n"; 
            return;
        }
        bl << nome << " perde " << p << " pontos de vida, restam: " << saude << "\n";
    }

    bool morreu(){
        return saude <= 0.0?true:false;
    }

    void operator*(Soldado &s){ //operador de luta
        int i = random(0,1);
        bl << "\n" << nome << " versus " << s.nome << "\n" << "\n";
        while(!morreu() && !s.morreu()){
            if(i % 2 == 0){
                bl << "-> " << *this << " ataca: " << "\n";
                atacar(s); 
            }
            else{
                bl <<"-> "<< s << " ataca: " << "\n";
                s.atacar(*this);
            }
            i++;
            bl << "\n";
            Sleep(delay);
        }
        return;
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
            bl << "O anao errou o ataque :(\n";
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
            bl << "Ataque duplo!\n";
            Soldado::atacar(s);
            if(!s.morreu()){Soldado::atacar(s);};
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
            bl << "Dobro de ataque!\n";
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
            bl << "Ataque duplo!\n";
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
            bl << "O mago bloqueou o ataque, ";
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
    Ladrao(string nome, float s, float p) : Soldado(nome, s, p) {}

    void defender(float p) //20% de chance de desviar do ataque
    {
        if(chance(20)){
            bl << "O ladrao desviou do golpe, ";
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
            bl << "Ataque duplo!" << "\n"; 
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

struct Exercito{
    vector<Soldado*> exercito;
    int uniao;
    ifstream army;
    string nome_ex;
    Battle_log bl;

    Exercito(string army_file, string n):army(army_file), nome_ex(n){
        if(!army.is_open()){
            cout << "erro ao abrir arquivo do " << nome_ex << endl;
            exit(1);
        }
        string classe, nome;
        float poder, vida;
        army >> uniao;
        do{ 
            army >> classe >> nome >> poder >> vida;

            if(classe == "Mago"){
                exercito.push_back(new Mago(nome, poder, vida));
            }
            if(classe == "Humano"){
                exercito.push_back(new Humano(nome, poder, vida));
            }
            if(classe == "Gollum"){
                exercito.push_back(new Gollum(nome, poder, vida));
            }
            if(classe == "Anao"){
                exercito.push_back(new Anao(nome, poder, vida));
            }
            if(classe == "Sauron"){
                exercito.push_back(new Sauron(nome, poder, vida));
            }
            if(classe == "Orc"){
                exercito.push_back(new Orc(nome, poder, vida));
            }
            if(classe == "Ladrao"){
                exercito.push_back(new Ladrao(nome, poder, vida));
            }

        }while(!army.eof());

        army.close();
    }

    void operator++(int){//motivação
        vector<Soldado*>::iterator it;

        bl << "MOTIVACAO!" << " o " << nome_ex << " ganha " << uniao << "\% de saude e poder \n";  
        for(it = exercito.begin(); it != exercito.end(); it++){
            (**it).poder *= 1 + uniao/100;
            (**it).saude *= 1 + uniao/100;
        }

        return;
    }
};

class Confronto{
public:
    //vector <Soldado*> bem; 
    //vector <Soldado*> mal;
    Exercito bem, mal;
    Battle_log bl;

    Confronto():bem("soldiers_bem.txt", "Exercito do bem"), mal("soldiers_mal.txt", "Exercito do mal"){
    }

    ~Confronto(){bl.battle_log.close();}

    void print(vector <Soldado *> army){
        vector <Soldado*>::iterator army_it ;

        for(army_it = army.begin(); army_it != army.end(); army_it++){
            bl << (**army_it) << "\n";
        }
        bl << "\n";
        return;
    }

    void intro(){
        bl << "Exercito do bem: \n" << "\n";
        print(bem.exercito);
        Sleep(delay);
        system("cls");
        bl << "Exercito do mal: \n" << "\n";
        print(mal.exercito);
        Sleep(delay);
        system("cls");
        bl << "QUE COMECEM OS JOGOS!\n";
        Sleep(delay);
        system("cls");
    }

    void guerra(){
        intro();
        //início randomico da batalha:
        srand(time(NULL));
        random_shuffle(mal.exercito.begin(), mal.exercito.end());
        random_shuffle(bem.exercito.begin(), bem.exercito.end());

        vector <Soldado*>::iterator bem_it = bem.exercito.begin();
        vector <Soldado*>::iterator mal_it = mal.exercito.begin();  

        while (!bem.exercito.empty() && !mal.exercito.empty())
        {   
            srand(time(NULL)); //sempre gerando uma nova seed para deixar o mais aleatório possível

            if(mal_it == mal.exercito.end()){ //se chegou ao final, reseta e da shuffle, caso contrário, continua de onde parou (prioridade)
                random_shuffle(mal.exercito.begin(), mal.exercito.end());
                mal_it = mal.exercito.begin();
            }

            if(bem_it == bem.exercito.end()){
                random_shuffle(bem.exercito.begin(), bem.exercito.end());
                bem_it = bem.exercito.begin();
            }

            while(mal_it != mal.exercito.end() && bem_it != bem.exercito.end()){
                (**mal_it)*(**bem_it);

                if((**mal_it).morreu()){
                    bl << "VENCEDOR: " << (**bem_it).getNome() << "\n\n";
                    bem++;
                    mal_it = mal.exercito.erase(mal_it);
                    bem_it++;
                }
                else if((**bem_it).morreu()){
                    bl << "VENCEDOR: " << (**mal_it).getNome() << "\n\n";
                    mal++;
                    bem_it = bem.exercito.erase(bem_it);
                    mal_it++;
                } 
                Sleep(delay);
                system("cls");
            }
        }
        
        if(bem.exercito.empty()){
            bl << "O EXERCITO DO MAL VENCEU";
            return;
        }
        else if(mal.exercito.empty()){
            bl << "O EXERCITO DO BEM VENCEU";
            return;
        }
    }
};


int main()
{   
    Confronto c;

    c.guerra();
}