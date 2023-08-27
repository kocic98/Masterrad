#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <cmath>

using namespace std;
using namespace std::chrono;
class vEB{
public:
    vEB(int x);
    ~vEB();
    void upisi(int x);
    void obrisi(int x);
    int unutra(int x);
    int sledbenik(int x);
    int prethodnik(int x);
private:
    int min;
    int max;
    vector<struct vEB*> klasteri;// u i-tom klasteru se nalaze brojevi od i*sqrt(u) do (i+1)*sqrt(u)-1 siftovani na 0-sqrt(u)-1
    struct vEB* info; // ako i-ti klaster nije prazan u info se nalazi broj i
    unsigned long velicina;
    unsigned velicina_klastera; //sqrt(velicina) a i broj vEB stabla u vektoru klasteri

    int klaster(int x){
        return x/velicina_klastera;
    }
    int pozicija_u_klasteru(int x){
        return x%velicina_klastera;
    }
};

//pomocne funkcije:

unsigned long nadji(int x){// vraca broj oblika 2^(2^k) najmanji takav veci od x
    int a=(int)log2(log2(x))+1;
    //cout<<x<<a<<endl;
    if (a>=5){
        return 2ul<<31;
    }
    return pow(2,pow(2,a));
}
void swap(int &a, int &b){
    int tmp=a;
    a=b;
    b=tmp;
}

//kraj pomocnih funkcija

//metode strukture

vEB::vEB(int x){

    min=max=-1;
    if(x<=2){
        velicina=2;
        velicina_klastera=0;
        info=nullptr;
        return;
    }else{
        unsigned long y=nadji(x-1);
        //cout<<"pravim stablo"<<y<<endl;
        velicina=y;
        if(y== 2ul<<31) velicina_klastera=2<<15;
        else velicina_klastera=sqrt(y);
        info=new vEB(velicina_klastera);
        for(int i=0;i<velicina_klastera;i++){
            klasteri.push_back(nullptr);
        }

    }
}

vEB::~vEB(){
    if(info!=nullptr) delete info;
}
void vEB::upisi(int x){
    //cout<<"upisujem "<<x<<endl;
    if(x==min || x==max) return;
    if(x>velicina-1) {
        cout<<"necu"<<x<<"\n";return;
    }
    if(min==max){
        if(min==-1){
            min=max=x;
        }
        else if(x<min) min=x;
        else max=x;
        return;
    }

    if(x<min){
        swap(min,x);
    }else if(x>max){
        swap(max,x);
    }
    if(velicina<=2)return;
    int uklasteru = x % velicina_klastera;
    int klaster = x / velicina_klastera;
    if(klasteri[klaster]==nullptr){
        info->upisi(klaster);
        klasteri[klaster]= new vEB(velicina_klastera);
        klasteri[klaster]->min=klasteri[klaster]->max=uklasteru;
        return;
    }
    klasteri[klaster]->upisi(uklasteru);

}

//void ispisi(vEB* v){
//    if(v->velicina==2) {
//        cout<<v->min<<"  "<<v->max<<endl;
//        return;
//    }
//    cout<<"ima"<<v->velicina_klastera<<"klastera";
//    for(auto x:v->klasteri){
//        if(x==nullptr) cout<<"null\n";
//        else ispisi(x);
//    }

//}

int vEB::unutra(int x){
    //cout<<"trazim\n";
    if(x==min || x== max)return true;
    if(velicina<=2) {cout<<"nisam nasao\n";return false;}
    int klaster=x/velicina_klastera;
    int uklasteru=x%velicina_klastera;
    if(klasteri[klaster]==nullptr) return false;
    return klasteri[klaster]->unutra(uklasteru);
}
int vEB::sledbenik(int x){
    //cout<<"sledbenik"<<velicina<<endl;
    if(x<min) return min;
    if(velicina<=2) return max;
    if(x>=max){cerr<<"nije u stablu\n";return -1;}

    if(min==max) return -1;//visak


    int klaster=x/ velicina_klastera;
    int uklasteru= x % velicina_klastera;
    if(klasteri[klaster]!=nullptr && uklasteru<klasteri[klaster]->max){
        return klaster*velicina_klastera+klasteri[klaster]->sledbenik(uklasteru);
    }else if(klasteri[klaster]==nullptr || uklasteru>=klasteri[klaster]->max){
        int tmp=info->sledbenik(klaster);
        if(tmp==-1) { return max;}
        return tmp*velicina_klastera+klasteri[tmp]->min;
    }else{
        cerr<<"greska";
        return -1;
    }



}


int vEB::prethodnik(int x){
    if(x>max) return max;
    if(x<=min){return -1;}
    if(velicina<=2) return min;


    if(min==max) return -1;//visak


    int klaster=x/ velicina_klastera;
    int uklasteru= x % velicina_klastera;
    if(klasteri[klaster]!=nullptr && uklasteru>klasteri[klaster]->min){
        return klaster*velicina_klastera+klasteri[klaster]->prethodnik(uklasteru);
    }else if(klasteri[klaster]==nullptr || uklasteru<=klasteri[klaster]->min){
        int tmp=info->prethodnik(klaster);
        if(tmp==-1) {return min;}
        return tmp*velicina_klastera+klasteri[tmp]->max;
    }else{
        cerr<<"greska";
        return -1;
    }



}

int main()
{

    //u datoteci se nalazi velicina stabla, broj elemenata u stablu, toliko elemenata
    ifstream dat("elementi.txt");
    if(!dat.is_open()) {cerr<<"greska\n"; return 1;}
    int n,x;
    dat>>n;
    cout<<"ovo je u: "<<n;
//    cout<<nadji(n)<<endl;
//    return 1;
    vEB stablo(n);
    dat>>n;

    cout<< " ovo je n:"<<n;
    while(n>0){
        dat>>x;
        //if(x>255) {cout<< x<<" ";n--; }
        stablo.upisi(x);
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
        int tmp=stablo.prethodnik(a);
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

