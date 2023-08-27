#include <iostream>
#include <chrono>
#include <cmath>
#include <fstream>
#include "avl.hpp"
#include "x_stablo.hpp"
using namespace std;
using namespace std::chrono;

class y_brzo_prefiksno{
private:
    x_brzo_prefiksno predstavnici;
    unordered_map<int, avl_cvor*> stabla;
    unordered_map<int, int> velicina_stabla;
    int u,w;
public:
    y_brzo_prefiksno(int a) : predstavnici(a),u(a){
        w=log2(a);
    }
    void y_upisi(int a){
        x_cvor *sad=predstavnici.x_sledbenik(a);
        if(sad==nullptr){
            stabla[u]= napravi_cvor(a);
            velicina_stabla[u]=1;
            predstavnici.x_upisi(u);
            return;
        }
        int predstavnik=sad->vrednost;
        stabla[predstavnik]=upisi_avl(stabla[predstavnik],a);
        velicina_stabla[predstavnik]=velicina_stabla[predstavnik]+1;
        if(velicina_stabla[predstavnik]>=2*w){
            avl_cvor* levo;
            int maks=stabla[predstavnik]->kljuc;
            stabla[predstavnik]=avl_podeli(stabla[predstavnik],&levo);
            predstavnici.x_upisi(maks);
            stabla[maks]=levo;
            velicina_stabla[maks]=broj_elemenata(levo);
            velicina_stabla[predstavnik]-=velicina_stabla[maks];
        }
    }
    bool y_sadrzi(int a){
        int predstavnik=predstavnici.x_sledbenik(a)->vrednost;
        return avl_sadrzi(stabla[predstavnik],a);
    }
    int y_prethodnik(int a){
        //return predstavnici.x_sledbenik(a)->vrednost;
        x_cvor *tmp=predstavnici.x_sledbenik(a);
        if(tmp==nullptr) {cerr<<"greska";return -1;}
        int predstavnik=tmp->vrednost;
        auto s= avl_prethodnik(stabla[predstavnik],a);
        if(s.has_value()){
            return s.value();
        }
        if(tmp->levo==nullptr) return -1;
        predstavnik=tmp->levo->vrednost;
        s= avl_prethodnik(stabla[predstavnik],a);
        return s.value();
    }



};

int main(){

    //u datoteci se nalazi velicina stabla, broj elemenata u stablu, toliko elemenata
    ifstream dat("elementi.txt");
    if(!dat.is_open()) {cerr<<"greska\n"; return 1;}
    int n,x;
    dat>>n;
    cout<<"ovo je u: "<<n;
    y_brzo_prefiksno stablo(n);
    dat>>n;
    cout<< " ovo je n:"<<n;
    while(n>0){
        dat>>x;
        stablo.y_upisi(x);
        n--;
    }
    dat.close();
    ifstream dat2("upit.txt");
    // u datoteci se nalazi broj brojeva cije prethodnike racunamo, i toliko brojeva
    if(!dat2.is_open()) {cerr<<"greska\n"; return 1;}
    vector<int> upit;
    dat2>>n;
    while(n>0){
        dat2>>x;
        upit.push_back(x);
        n--;
    }
    dat2.close();
    auto start = high_resolution_clock::now();
    for(int a:upit){
        int tmp=stablo.y_prethodnik(a);
        //cout<<tmp<<endl;
        //        if(tmp==nullptr)
        //            cout<<-1<<endl;
        //        else
        //            cout<<stablo.x_prethodnik(a)->vrednost<<endl;
    }
    auto end= high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout<<" ovo je vreme:"<<duration.count()<<endl;
    return 0;
}
