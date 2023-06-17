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

inline int random(int min, int max){
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<mt19937::result_type> dist(min,max);

    return (int)dist(rng); 
}

inline bool chance(int perc){
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

class Exercito;

class Soldado
{
friend class Exercito;
protected:
    string nome;
    float saude, poder, full_hp;
    int chance_contra_ataque;
    bool pavor; 
    Battle_log bl;

public:
    Soldado(string nome, float s, float p, int chance = 0, bool pavor = false) : nome(nome), saude(s), poder(p), full_hp(s),chance_contra_ataque(chance), pavor(pavor){}

    virtual void atacar(Soldado &s)
    {   
        if(pavor && chance(20)){
            bl << "O " << nome << " esta apavorado! Errou o ataque! ";
            s.defender(0, *this);
        }

        int RANGE = random(0, 10);
        int dano = random((int)poder - RANGE, (int)poder + RANGE);
        s.defender(dano, *this);
    }

    virtual void defender(float p, Soldado &rival)
    {
        saude -= p;
        if(chance_contra_ataque){
            bl << "Contra-ataque!\n";
            atacar(rival);
            return;
        }

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
        if(delay == 0){bl << "\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0\u25A0";}
        bl << "\n" << *this << " versus " << s << "\n" << "\n";
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

    friend ostream& operator<<(ostream &os, Soldado &s){
        os << s.getNome() << " (HP: " << s.getSaude() << ")";
        return os; 
    }

    string getNome(){return nome;}
    float getSaude(){return saude;}
    float getFullhp(){return full_hp;}
    float getPoder(){return poder;}
    void setPavor(bool pav){pavor = pav;}
    void setSaude(float p){saude = p;}
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
            bl << "O humano desferiu um ataque duplo!\n";
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

class Guerreiro_mistico_da_agua : public Soldado{
public:
    //O guerreiro mistico da agua tem 20% de chance de contra-atacar
    Guerreiro_mistico_da_agua(string nome, float s, float p) : Soldado(nome, s, p, 20){}
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
            bl << "Sauron confunde o inimigo com seu olhar tenebroso e desfere o dobro de ataque!\n";
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
            bl << "O orc desfere um ataque duplo com sua machadinhas!\n";
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

    void defender(float p, Soldado& rival) // 10% de chance de bloquear um ataque com seu escudo mágico
    {
        if (chance(10)) //toda vez que o mago bloqueia um ataque, aumenta sua chance de contra-ataque
        {
            bl << "O mago bloqueou o ataque com seu escudo aureo, ";
            Soldado::defender(0, rival);
            chance_contra_ataque += 15;
            return;
        }
        else
        {
            Soldado::defender(p, rival);
        }
    }
};

class Darknight: public Soldado{
public:
    Darknight(string nome, float s, float p) : Soldado(nome, s, p) {}

    void defender(float p, Soldado& rival)  //20% chance de bloquear com seu escudo sinistro
    {
        if(chance(30)){ 
            bl << "O cavaleiro das trevas usa o seu escudo com sabedoria e bloqueia totalmente o ataque inimigo, ";
            Soldado::defender(0, rival);
            return;
        }
        else{
            Soldado::defender(p, rival);
        }
    }

    void atacar(Soldado &s) //10% de chance de dar HITKILL
    {
        if (chance(10))
        {
            bl << "Darknight utiliza sua lamina de sombras que corta até a luz e finaliza o inimigo!" << "\n"; 
            s.setSaude(0);
            return;
        }
        else
        {
            Soldado::atacar(s);
        }
        return;
    }
};

class Arvrok: public Soldado{
public:
    Arvrok(string nome, float s, float p) : Soldado(nome, s, p) {}

    void atacar(Soldado &s) //a árvore fantasma tem uma chance de 30% de deixar o inimigo apavorado, aumentando sua chance de errar ataques em 20%;
    {
        if (chance(30))
        {
            bl << "A arvore apavorou " << s.getNome() << " com seus olhos tenebrosos!\n"; 
            s.setPavor(true);
        }
        else
        {
            Soldado::atacar(s);
        }
        return;
    }

    void defender(float p, Soldado& rival) //20% de chance curar o mesmo dano que sofreu
    {
        if(chance(20)){
            bl << "A arvore absorve o dano recebido e transforma em saude, ganhando " << p << " HP\n";
            saude += p;
            return;
        }
        else{
            Soldado::defender(p, rival);
        }
    }
};

struct Exercito{
    vector<Soldado*> exercito;
    int uniao;
    ifstream army;
    string nome_ex;
    Battle_log bl;

    Exercito(string army_file, string n);

    void operator++(int){//motivação
        vector<Soldado*>::iterator it;

        bl << "MOTIVACAO!" << " o " << nome_ex << " ganha " << uniao << "\% de saude e poder \n";  
        for(it = exercito.begin(); it != exercito.end(); it++){
            (**it).poder *= 1 + uniao/100;
            (**it).saude *= 1 + uniao/100;
        }

        return;
    }

    void curar(float p){
        vector<Soldado*>::iterator it;

        for(it = exercito.begin(); it != exercito.end(); it++){
            (**it).saude += p;
        }
    }
};

class Confronto{
public:
    //vector <Soldado*> bem; 
    //vector <Soldado*> mal;
    static Exercito bem, mal;
    Battle_log bl;

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
        bl << "O mundo esta em guerra. De um lado, o exercito vermelho, comandado pelo terrivel Sauron, um ser de poder\n";
        bl << "incomensuravel que deseja dominar todas as terras. Ao seu lado, lutam orcs ferozes e sanguinarios, um cavaleiro das\n";
        bl << "trevas chamado Darknight, que possui uma lamina capaz de matar com um so golpe, e uma arvore gigante chamada\n";
        bl << "Arvrok, que espalha o terror e a confusao entre os inimigos com seus galhos e raizes. Do outro lado, o exercito branco,\n";
        bl << "liderado por um sabio e poderoso mago, que protege seus aliados com um escudo mágico impenetrável. Junto com\n";
        bl << "ele, combatem bravos anoes e humanos, armados com machados e espadas, um elfo curandeiro que tem o dom da\n";
        bl << "bencao universal, capaz de curar e fortalecer seus companheiros, e um guerreiro mistico da agua, que usa sua espada de agua\n";
        bl << "para contra-atacar os golpes dos adversarios. A batalha final esta prestes a começar. Qual lado voce vai escolher?\n\n";
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

Exercito Confronto::bem("soldiers_bem.txt", "Exercito branco");
Exercito Confronto::mal("soldiers_mal.txt", "Exercito vermelho");

class Elfo : public Soldado
{
    float cura;
public:
    Elfo(string nome, float s, float p) : Soldado(nome, s, p + 1), cura(1.3){}

    void atacar(Soldado &s) //o elfo tem a benção universal, que cura todos os seus companheiros
    {
        if (chance(30))
        {
            bl << "O elfo usou sua bencao universal, todo o exercito teve sua saude aumentada em: " << (cura-1)*100 <<"%\n"; 
            Confronto::bem.curar(cura);
        }
        else
        {
            Soldado::atacar(s);
        }
        return;
    }

};

Exercito::Exercito(string army_file, string n):army(army_file), nome_ex(n){
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
        if(classe == "Guerreiro"){
            exercito.push_back(new Guerreiro_mistico_da_agua(nome, poder, vida));
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
        if(classe == "Darknight"){
            exercito.push_back(new Darknight(nome, poder, vida));
        }
        if(classe == "Elfo"){
            exercito.push_back(new Elfo(nome, poder, vida));
        }
        if(classe == "Arvrok"){
            exercito.push_back(new Arvrok(nome, poder, vida));
        }
    }while(!army.eof());

    army.close();
}


int main()
{   
    Confronto c;

    c.guerra();
}