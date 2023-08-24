#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

typedef struct vEB{

    vEB(int x);
    ~vEB();
    void upisi(int x);
    void obrisi(int x);
    int unutra(int x);
    int sledbenik(int x);
    int prethodnik(int x);
    int min;
    int max;
    vector<struct vEB*> klasteri;// u i-tom klasteru se nalaze brojevi od i*sqrt(u) do (i+1)*sqrt(u)-1 siftovani na 0-sqrt(u)-1
    struct vEB* info; // ako i-ti klaster nije prazan u info se nalazi broj i
    int velicina;
    int velicina_klastera; //sqrt(velicina) a i broj vEB stabla u vektoru klasteri

    int klaster(int x){
        return x/velicina_klastera;
    }
    int pozicija_u_klasteru(int x){
        return x%velicina_klastera;
    }
}vEB;

//pomocne funkcije:

int nadji(int x){// vraca broj oblika 2^(2^k) najmanji takav veci od x
    return pow(2,pow(2,(int)log2(log2(x))+1));
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
        int y=nadji(x-1);
        //cout<<"pravim stablo"<<y<<endl;
        velicina=y;
        velicina_klastera=sqrt(y);
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
        cout<<"necu\n";return;
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
    if(velicina==2)return;
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

void ispisi(vEB* v){
    if(v->velicina==2) {
        cout<<v->min<<"  "<<v->max<<endl;
        return;
    }
    cout<<"ima"<<v->velicina_klastera<<"klastera";
    for(auto x:v->klasteri){
        if(x==nullptr) cout<<"null\n";
        else ispisi(x);
    }

}

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
    if(velicina<=2) return min;
    if(x<=min){return -1;}

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
//




int main()
{
    int x;
    cin>>x;
    //cout<<nadji(14)<<endl;
    int i;
    vEB a(145);
    a.upisi(3);
    a.upisi(5);
    a.upisi(1);
    a.upisi(2);

    a.upisi(7);
    a.upisi(6);
    //ispisi(&a);
    //cout<<a.velicina;
    cout<<(a.unutra(x)?"jeste": "nije")<<endl;
    cout<<a.prethodnik(x);
    //cout<<x;
    return 0;
}

//vEB stablo kraj
//xfast stablo pocetak
