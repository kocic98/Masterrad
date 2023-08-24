#include <iostream>
#include <chrono>
#include <cmath>
#include "AVL.h"
#include "xfast.h"
using namespace std;
using namespace std::chrono;

class yfastTrie{
private:
    x_brzo_prefiksno predstavnici;
    unordered_map<int, avl_cvor*> stabla;
    unordered_map<int, int> velicina_stabla;
    int u,w;
public:
    yfastTrie(int a) : predstavnici(a),u(a-1){
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
//            cout<<"delim\n";
            avl_cvor* levo;
            int maks=stabla[predstavnik]->kljuc;
            stabla[predstavnik]=avl_podeli(stabla[predstavnik],&levo);
            predstavnici.x_upisi(maks);
            stabla[maks]=levo;
            velicina_stabla[maks]=broj_elemenata(levo);
            velicina_stabla[predstavnik]-=velicina_stabla[maks];
        }
    }
    bool y_unutra(int a){
        int predstavnik=predstavnici.x_sledbenik(a)->vrednost;
        return avl_sadrzi(stabla[predstavnik],a);
    }
    int y_prethodnik(int a){
        x_cvor *tmp=predstavnici.x_sledbenik(a);
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

    yfastTrie a(64);
    a.y_upisi(5);
    a.y_upisi(51);
    a.y_upisi(25);
    a.y_upisi(15);
    a.y_upisi(18);
    a.y_upisi(12);
    a.y_upisi(43);
    a.y_upisi(27);
    a.y_upisi(33);
    a.y_upisi(38);
    a.y_upisi(4);
    a.y_upisi(31);
    a.y_upisi(22);
    a.y_upisi(23);
    a.y_upisi(24);
    a.y_upisi(11);
    a.y_upisi(12);
    a.y_upisi(13);
    a.y_upisi(39);
    a.y_upisi(40);
    int x;
    cin>>x;
    auto start = high_resolution_clock::now();
    cout<<"\n"<<a.y_prethodnik(x);
    auto end= high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout<<duration.count()<<endl;
    return 0;
}
