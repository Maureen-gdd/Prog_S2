#include <iostream>
using namespace std;

struct Noeud
{
    int donnee;
    Noeud* suivant;
};

struct Liste
{
    int nb_noeud;
    Noeud* premier;
    // your code
};

struct DynaTableau
{
    int* donnees;
    // your code
};


void initialise(Liste* liste)
{
    liste->premier = nullptr;
    liste->nb_noeud = 0;
}

// Pour vérifier si la liste est vide. Retourne true si elle est vide, false sinon.
bool est_vide(const Liste* liste)
{
    // Si le premier élément ne renvoie vers rien donc liste vide, return true
    if (liste->premier == nullptr) 
        return true;

    return false;
}

// Pour ajouter une valeur à la fin d'une liste 
void ajoute(Liste* liste, int valeur)
{
    Noeud* n_nouveau = new Noeud();
    if (!n_nouveau)
        std::cout << "Allocation failed" << std::endl;

    n_nouveau->donnee = valeur;
    n_nouveau->suivant = nullptr;

    // Si la liste est vide, on ajoute la valeur en premier de la liste
    if(est_vide(liste))
    {
        liste->premier = n_nouveau;
    }
    // Sinon on l'ajoute à la fin
    else
    {
        Noeud* n_dernier = liste->premier;
        // Tant qu'on arrive pas à la fin de la liste
        while (n_dernier->suivant != nullptr)
        {
            n_dernier = n_dernier->suivant;
        }
        n_dernier->suivant = n_nouveau;
    }
    int temp = liste->nb_noeud;
    liste->nb_noeud = ++temp;
}

// Permet d'afficher les noeuds de la liste
void affiche(const Liste* liste)
{
    int i = 0;
    Noeud *n_actuel = liste->premier;

    while(n_actuel != nullptr) 
    {
        cout << "Valeur du noeud n°" << i << " = " << n_actuel->donnee << endl;
        i++;
        n_actuel = n_actuel->suivant;
    }
}

// Récupérer un élément dans la liste à un indice n
int recupere(const Liste* liste, int n)
{
    if(n > liste->nb_noeud || n < 0)
        return -666;

    int i = 0;
    Noeud *n_actuel = liste->premier;

    // Tant qu'on a pas atteint l'indice n
    while(i < n)
    {
        i++;
        n_actuel = n_actuel->suivant;
    }

    return n_actuel->donnee;

}

// Retourne l’index de valeur dans la structure ou -1 s’il n’existe pas
int cherche(const Liste* liste, int valeur)
{
    Noeud *n_actuel = liste->premier;
    int i = 0;

    while(n_actuel != nullptr)
    {
        if(n_actuel->donnee == valeur)
            return i;

        n_actuel = n_actuel->suivant;
        i++; 
    }
    return -1;
}

void stocke(Liste* liste, int n, int valeur)
{
    /*Noeud *n_actuel = liste->premier;
    int i = 0;
    aimNode->donnee = valeur;

    int iterator = 0;
    Noeud *currentNode = liste->premier;

    while (currentNode != nullptr) {
        if (iterator == n - 1) {
            aimNode->suivant = currentNode->suivant;
            currentNode->suivant = aimNode;
        }
        iterator++;
        currentNode = currentNode->suivant;
    }*/


}

/* ********** TABLEAU DYNAMIQUE ********** */

void ajoute(DynaTableau* tableau, int valeur)
{

}


void initialise(DynaTableau* tableau, int capacite)
{

}

bool est_vide(const DynaTableau* liste)
{
    return false;
}

void affiche(const DynaTableau* tableau)
{

}

int recupere(const DynaTableau* tableau, int n)
{
    return 0;
}

int cherche(const DynaTableau* tableau, int valeur)
{
    return -1;
}

void stocke(DynaTableau* tableau, int n, int valeur)
{

}

//void pousse_file(DynaTableau* liste, int valeur)
void pousse_file(Liste* liste, int valeur)
{

}

//int retire_file(Liste* liste)
int retire_file(Liste* liste)
{
    return 0;
}

//void pousse_pile(DynaTableau* liste, int valeur)
void pousse_pile(Liste* liste, int valeur)
{

}

//int retire_pile(DynaTableau* liste)
int retire_pile(Liste* liste)
{
    return 0;
}


int main()
{
    int recup = 0;
    int valeur = 49;

    Liste liste;
    initialise(&liste);
    /*DynaTableau tableau;
    initialise(&tableau, DynaTableau, 5);*/

    if (!est_vide(&liste) )
        std::cout << "La liste est cassé(e) snif..." << std::endl;

    /*if (!est_vide(&tableau))
        std::cout << "Oups y a une anguille dans mon tableau" << std::endl;*/

    for (int i=1; i<=7; i++) 
    {
        ajoute(&liste, i*7);
        //ajoute(&tableau, i*5);
    }

    if (est_vide(&liste))
    {
        std::cout << "Oups y a une anguille dans ma liste" << std::endl;
    }

    /*if (est_vide(&tableau))
    {
        std::cout << "Oups y a une anguille dans mon tableau" << std::endl;
    }*/

    cout << "Elements initiaux:" << endl;
    affiche(&liste);
    //affiche(&tableau);
    std::cout << std::endl;

    recup = recupere(&liste, 4);
    if(recup == -666)
        cout << "Oups vous utilisez un indice qui est trop grand/petit pour la liste" << endl;
    else
        cout << "Element indice 4 de la liste: " << recupere(&liste, 4) << std::endl;
    /*std::cout << "5e valeur du tableau " << recupere(&tableau, 4) << std::endl;*/

    recup = cherche(&liste, valeur);
    if(recup == -1)
        cout << "La valeur " << valeur << " ne se trouve pas dans la liste\n";
    else
        cout << "La valeur " << valeur << " se trouve à l'indice " << recup << endl;
        
    /*std::cout << "15 se trouve dans la liste à " << cherche(&tableau, 15) << std::endl;

    stocke(&liste, 4, 7);
    stocke(&tableau, 4, 7);

    std::cout << "Elements après stockage de 7:" << std::endl;
    affiche(&liste);
    affiche(&tableau);
    std::cout << std::endl;

    Liste pile; // DynaTableau pile;
    Liste file; // DynaTableau file;

    initialise(&pile);
    initialise(&file);

    for (int i=1; i<=7; i++) {
        pousse_file(&file, i);
        pousse_pile(&pile, i);
    }

    int compteur = 10;
    while(!est_vide(&file) && compteur > 0)
    {
        std::cout << retire_file(&file) << std::endl;
        compteur--;
    }

    if (compteur == 0)
    {
        std::cout << "Ah y a un soucis là..." << std::endl;
    }

    compteur = 10;
    while(!est_vide(&pile) && compteur > 0)
    {
        std::cout << retire_pile(&pile) << std::endl;
        compteur--;
    }

    if (compteur == 0)
    {
        std::cout << "Ah y a un soucis là..." << std::endl;
    }*/

    return 0;
}
