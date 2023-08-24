#ifndef AVL_H
#define AVL_H
#include<iostream>
#include <optional>
using namespace std;

class avl_cvor
{
public:
    int kljuc;
    avl_cvor *levo;
    avl_cvor *desno;
    int visina;
};


int visina(avl_cvor *N)
{
    if (N == NULL)
        return 0;
    return N->visina;
}

int max(int a, int b)
{
    return (a > b)? a : b;
}

avl_cvor* napravi_cvor(int a)
{
    avl_cvor* cvor = new avl_cvor();
    cvor->kljuc = a;
    cvor->levo = NULL;
    cvor->desno = NULL;
    cvor->visina = 1;
    return(cvor);
}
avl_cvor *desna_rotacija(avl_cvor *y)
{
    avl_cvor *x = y->levo;
    avl_cvor *tmp = x->desno;

    x->desno = y;
    y->levo = tmp;
    y->visina = max(visina(y->levo),
                    visina(y->desno)) + 1;
    x->visina = max(visina(x->levo),
                    visina(x->desno)) + 1;
    return x;
}

avl_cvor *leva_rotacija(avl_cvor *x)
{
    avl_cvor *y = x->desno;
    avl_cvor *T2 = y->levo;

    y->levo = x;
    x->desno = T2;

    x->visina = max(visina(x->levo),visina(x->desno)) + 1;
    y->visina = max(visina(y->levo),visina(y->desno)) + 1;
    return y;
}

int getBalance(avl_cvor *N)
{
    if (N == NULL)
        return 0;
    return visina(N->levo) - visina(N->desno);
}
avl_cvor* upisi_avl(avl_cvor* koren, int a)
{
    if (koren == NULL)
        return(napravi_cvor(a));

    if (a < koren->kljuc)
        koren->levo = upisi_avl(koren->levo, a);
    else if (a > koren->kljuc)
        koren->desno = upisi_avl(koren->desno, a);
        return koren;

    koren->visina = 1 + max(visina(koren->levo),visina(koren->desno));

    int razlika_visina = getBalance(koren);
    if (razlika_visina > 1 && a < koren->levo->kljuc)
        return desna_rotacija(koren);

    if (razlika_visina < -1 && a > koren->desno->kljuc)
        return leva_rotacija(koren);

    if (razlika_visina > 1 && a > koren->levo->kljuc)
    {
        koren->levo = leva_rotacija(koren->levo);
        return desna_rotacija(koren);
    }

    if (razlika_visina < -1 && a < koren->desno->kljuc)
    {
        koren->desno = desna_rotacija(koren->desno);
        return leva_rotacija(koren);
    }
    return koren;
}


void ispisi_prefiksno(avl_cvor *root)
{
    if(root != NULL)
    {
        cout << root->kljuc << " ";
        ispisi_prefiksno(root->levo);
        ispisi_prefiksno(root->desno);
    }
}

int broj_elemenata(avl_cvor* koren){
    if(koren==nullptr) return 0;
    return 1+broj_elemenata(koren->levo)+ broj_elemenata(koren->desno);
}

avl_cvor* avl_podeli(avl_cvor* koren, avl_cvor** levo){//vraća desno podstablo
    avl_cvor *rez=koren->desno;
    *levo=upisi_avl(koren->levo,koren->kljuc);
    delete(koren);
    return rez;
}

bool avl_sadrzi(avl_cvor *root, int a){
    if(root==nullptr) return false;
    if(root->kljuc==a) return true;
    if(root->kljuc>a) return avl_sadrzi(root->desno,a);
    return avl_sadrzi(root->levo,a);
}
optional<int> avl_prethodnik(avl_cvor *root, int a){
    int tmp=-1;
    while(root!=nullptr){
        if(root->kljuc<=a && (tmp==-1|| tmp<=root->kljuc)){
            tmp=root->kljuc;
            root=root->desno;
        }else {
            root=root->levo;
        }
    }
    if(tmp==-1) return nullopt;
    return tmp;
}
void avl_oslobodi(avl_cvor* koren){
    if(koren!=nullptr){
        avl_oslobodi(koren->levo);
        avl_oslobodi(koren->desno);
        delete(koren);
    }
}








#endif // AVL_H
