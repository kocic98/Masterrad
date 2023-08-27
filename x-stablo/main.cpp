
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <chrono>
using namespace std;
using namespace std::chrono;

struct x_cvor{
    int nivo;
    int vrednost;
    x_cvor *levo, *desno;

    x_cvor(){
        nivo = -1;
        levo = nullptr;
        desno = nullptr;
    }
};

class x_brzo_prefiksno{
    int w;
    vector<unordered_map<int, x_cvor*>> mapa;

    int getDigitCount(int x){
        int count = 0;
        for(; x > 0; x >>= 1, ++count);
        return count;
    }

    int leftChild(int x){
        return (x << 1);
    }

    int rightChild(int x){
        return ((x << 1) | 1);
    }

    x_cvor *getLeftmostLeaf(x_cvor *parent){
        while(parent->nivo != w){
            if(parent->levo != nullptr)
                parent = parent->levo;
            else
                parent = parent->desno;
        }
        return parent;
    }

    x_cvor * getRightmostLeaf(x_cvor *parent){
        while(parent->nivo != w){
            if(parent->desno != nullptr)
                parent = parent->desno;
            else
                parent = parent->levo;
        }
        return parent;
    }

public:
    x_brzo_prefiksno(int U){
        w = getDigitCount(U);
        mapa.assign(w + 1,unordered_map<int, x_cvor *>());
        x_cvor *root = new x_cvor();
        root->nivo = 0;
        mapa[0][0] = root;

    }

    x_cvor* x_pretraga(int k){
        if(mapa[w].find(k) == mapa[w].end())
            return nullptr;
        return mapa[w][k];
    }

    x_cvor* x_sledbenik(int k){
        int low = 0,
            high = w + 1,
            mid, prefix;

        x_cvor *tmp = nullptr;
        while(high - low > 1){
            mid = (low + high) >> 1;
            prefix = k >> (w - mid);
            if(mapa[mid].find(prefix) == mapa[mid].end())
                high = mid;
            else{
                low = mid;
                tmp  = mapa[mid][prefix];
            }
        }

        if(tmp == nullptr || tmp->nivo == 0){
            // occours on first insertion
            return nullptr;
        }

        if(tmp->nivo == w)
            return tmp;

        // use descendant node
        if((k >> (w - tmp->nivo - 1)) & 1)
            tmp = tmp->desno;
        else
            tmp = tmp->levo;

        if(tmp->vrednost < k){
            return tmp->desno;
        }
        return tmp;
    }

    x_cvor* x_prethodnik(int k){
        int visi_nivo = 0,
            nizi_nivo = w + 1,
            sredina, prefiks;

        x_cvor *tmp = nullptr;
        while(nizi_nivo - visi_nivo > 1){
            sredina = (visi_nivo + nizi_nivo) >> 1;
            prefiks = k >> (w - sredina);
            if(mapa[sredina].find(prefiks) == mapa[sredina].end())
                nizi_nivo = sredina;
            else{
                visi_nivo = sredina;
                tmp  = mapa[sredina][prefiks];
            }
        }

        if(tmp == nullptr || tmp->nivo == 0)
            return nullptr;

        if(tmp->nivo == w)
            return tmp;

        if((k >> (w - tmp->nivo - 1)) & 1)
            tmp = tmp->desno;
        else
            tmp = tmp->levo;

        if(tmp->vrednost > k){
            return tmp->levo;
        }
        return tmp;
    }

    void x_upisi(int k){
        x_cvor *node = new x_cvor();
        node->vrednost = k;
        node->nivo = w;

        // update linked list
        x_cvor *pre = x_prethodnik(k);
        x_cvor *suc = x_sledbenik(k);
        if(pre != nullptr){
            if(pre->nivo != w){
                std::cout << "Wierd level " << pre->nivo << '\n';
            }
            node->desno = pre->desno;
            pre->desno = node;
            node->levo = pre;
        }
        if(suc != nullptr){
            if(suc->nivo != w){
                std::cout << "Wierd level " << suc->nivo << '\n';
            }
            node->levo = suc->levo;
            suc->levo = node;
            node->desno = suc;
        }

        int lvl = 1, prefix;
        while(lvl != w){
            prefix = k >> (w - lvl);
            if(mapa[lvl].find(prefix) == mapa[lvl].end()){
                x_cvor *inter = new x_cvor();
                inter->nivo = lvl;
                mapa[lvl][prefix] = inter;
                if(prefix & 1)
                    mapa[lvl - 1][prefix >> 1]->desno = inter;
                else
                    mapa[lvl - 1][prefix >> 1]->levo = inter;
            }
            else if(lvl!= w-1 && mapa[lvl][prefix]->levo->nivo==w ){
                mapa[lvl][prefix]->levo=nullptr;
            }else if(lvl!= w-1 && mapa[lvl][prefix]->desno->nivo==w){
                mapa[lvl][prefix]->desno=nullptr;

            }
            ++lvl;
        }
        mapa[w][k] = node;
        if(k & 1)
            mapa[w - 1][k >> 1]->desno = node;
        else
            mapa[w - 1][k >> 1]->levo = node;
        prefix = k;
        lvl = w - 1;
        while(lvl != 0){
            prefix = prefix >> 1;
            if(mapa[lvl][prefix]->levo == nullptr)
                mapa[lvl][prefix]->levo = getLeftmostLeaf(mapa[lvl][prefix]->desno);
            else if(mapa[lvl][prefix]->desno == nullptr)
                mapa[lvl][prefix]->desno = getRightmostLeaf(mapa[lvl][prefix]->levo);
            --lvl;
        }
        if(mapa[0][0]->levo == nullptr){
            mapa[0][0]->levo = getLeftmostLeaf(mapa[0][0]->desno);
        }
        if(mapa[0][0]->desno == nullptr){
            mapa[0][0]->desno = getRightmostLeaf(mapa[0][0]->levo);
        }
    }

    ~x_brzo_prefiksno(){
        //
    }

};

int main(){
    //u datoteci se nalazi velicina stabla, broj elemenata u stablu, toliko elemenata
    ifstream dat("elementi.txt");
    if(!dat.is_open()) {cerr<<"greska\n"; return 1;}
    int n,x;
    dat>>n;
    cout<<"ovo je u: "<<n;
    x_brzo_prefiksno stablo(n);
    dat>>n;
    cout<< "ovo je n:"<<n;
    while(n>0){
        dat>>x;
        stablo.x_upisi(x);
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
        x_cvor *tmp=stablo.x_prethodnik(a);
//        if(tmp==nullptr)
//            cout<<-1<<endl;
//        else
//            cout<<tmp->vrednost<<endl;
    }
    auto end= high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    cout<<" a ovo je vreme:"<<duration.count()<<endl;
    return 0;
}
