#include <iostream>
using namespace std;

#define INDICE 4
#define VALEUR 49

struct Noeud
{
    int donnee;
    Noeud* suivant;
};

struct Liste
{
    int nb_noeud;
    Noeud* premier;
};

struct DynaTableau
{
    int* donnees;
    int capacite;
    int nb_elem;
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
        return -1;

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
    Noeud *n_actuel = liste->premier;
    int i = 0;
    while (n_actuel->suivant != NULL)
    {
        if (i == n)
        {
            n_actuel->donnee = valeur;
        }
        n_actuel = n_actuel->suivant;
        i++;
    }
}

/* ********** TABLEAU DYNAMIQUE ********** */

void ajoute(DynaTableau* tableau, int valeur)
{
    if(tableau->nb_elem >= tableau->capacite)
    {
        int* newTableau = new int[tableau->capacite + 1];

        for (int i = 0; i < tableau->capacite; i++)
        {
            newTableau[i] = tableau->donnees[i];
        }
        newTableau[tableau->capacite] = valeur;
        tableau->capacite++;
        delete[] tableau->donnees;
        tableau->donnees = newTableau;
    }
    else
    {
        tableau->donnees[tableau->nb_elem] = valeur;
    }

    tableau->nb_elem++;
}


void initialise(DynaTableau* tableau, int capacite)
{
    tableau->donnees = new int[capacite];
    tableau->capacite = capacite;
    tableau->nb_elem = 0;

}

bool est_vide(const DynaTableau* liste)
{
    if (liste->nb_elem == 0) 
    {
        return true;
    }

    return false;
}

void affiche(const DynaTableau* tableau)
{
    for (int i = 0; i < tableau->nb_elem; i++)
    {
        cout << "Valeur de la case n°" << i << " = " << tableau->donnees[i] << endl;
    }
}

int recupere(const DynaTableau* tableau, int n)
{
    // Indice trop grand ou petit pour le tableai
    if(n >= tableau->capacite || n < 0)
        return -1;

    // Indice dans la capacite mais le tableau n'est pas entièrement rempli
    if(n >= tableau->nb_elem)
        return -2;

    return tableau->donnees[n];
}

int cherche(const DynaTableau* tableau, int valeur)
{
    return -1;
}

void stocke(DynaTableau* tableau, int n, int valeur)
{

}

int getCapacite(DynaTableau* tableau)
{
    return tableau->capacite;
}

/* ********** FIFO / LIFO ********** */

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

    Liste liste;
    initialise(&liste);
    DynaTableau tableau;
    initialise(&tableau, 5);

    if (!est_vide(&liste) )
        std::cout << "La liste est cassée snif... Elle devrait être vide" << std::endl;

    if (!est_vide(&tableau))
        std::cout << "Le tableau est cassé snif... Il devrait être vide" << std::endl;

    for (int i=1; i<=7; i++) 
    {
        ajoute(&liste, i*7);
    }

    for (int i=1; i<=3; i++) 
    {
        ajoute(&tableau, i*5);
    }

    if (est_vide(&liste))
        std::cout << "Oups ma liste ne devrait pas être vide" << std::endl;

    if (est_vide(&tableau))
    {
        std::cout << "Oups mon tableau ne devrait pas être vide" << std::endl;
    }

    cout << "Elements initiaux:" << endl;
    cout << "Liste: " << endl;
    affiche(&liste);
    cout << "\nTableau Dynamique: " << endl;
    affiche(&tableau);
    std::cout << std::endl;

    /*** ********* TEST FONCTIONS LISTE ********* ***/
    recup = recupere(&liste, INDICE);
    if(recup == -1)
        cout << "Oups vous utilisez un indice qui est trop grand/petit pour la liste" << endl;
    else
        cout << "Element indice " << INDICE << " de la liste: " << recupere(&liste, INDICE) << std::endl;

    recup = cherche(&liste, VALEUR);
    if(recup == -1)
        cout << "La valeur " << VALEUR << " ne se trouve pas dans la liste\n";
    else
        cout << "La valeur " << VALEUR << " se trouve à l'indice " << recup << endl;

    stocke(&liste, 4, 7);
    std::cout << "\nElements de la liste après stockage de 7 à l'indice 4:" << std::endl;
    affiche(&liste);

    /*** ********* TEST FONCTIONS TABLEAU DYNAMIQUE ********* ***/
    cout << "\nT E S T   T A B L E A U   D Y N A" << endl;
    cout << "\nJe vais chercher un élément en dehors de la capacité du tableau: " << endl;
    int indice = 10;
    cout << "Indice: " << indice << endl;
    recup = recupere(&tableau, indice);
    if(recup == -1)
        cout << "Oups vous utilisez un indice qui est trop grand/petit pour le tableau" << endl;
    else if(recup == -2)
        cout << "Oups vous utilisez un indice dont la case du tableau n'est pas encore remplis" << endl;
    else
        cout << "Element indice " << indice << " de la liste: " << recup << std::endl;

    cout << "\nJe vais chercher un élément dans la capacité du tableau mais la case est vide: " << endl;
    indice = 4;
    cout << "Indice: " << indice << endl;
    recup = recupere(&tableau, indice);
    if(recup == -1)
        cout << "Oups vous utilisez un indice qui est trop grand/petit pour le tableau" << endl;
    else if(recup == -2)
        cout << "Oups vous utilisez un indice dont la case du tableau n'est pas encore remplis" << endl;
    else
        cout << "Element indice " << indice << " du tableau: " << recup << std::endl;

    cout << "\nJe vais chercher un élément dans le tableau: " << endl;
    indice = 2;
    cout << "Indice: " << indice << endl;
    recup = recupere(&tableau, indice);
    if(recup == -1)
        cout << "Oups vous utilisez un indice qui est trop grand/petit pour le tableau" << endl;
    else if(recup == -2)
        cout << "Oups vous utilisez un indice dont la case du tableau n'est pas encore remplis" << endl;
    else
        cout << "Element indice " << indice << " de la liste: " << recup << std::endl;


    cout << "\nJ'ajoute sans dépasser la capacité du tableau." << endl;
    cout << "Capacité tableau avant: " << getCapacite(&tableau) << endl;
    for (int i=1; i<=2; i++) 
    {
        ajoute(&tableau, i*4);
    }
    cout << "Capacité tableau après: " << getCapacite(&tableau) << endl;
    affiche(&tableau);

    cout << "\nJ'ajoute en dépassant la capacité du tableau." << endl;
    cout << "Capacité tableau avant: " << getCapacite(&tableau) << endl;
    for (int i=1; i<=2; i++) 
    {
        ajoute(&tableau, i*6);
    }
    cout << "Capacité tableau après: " << getCapacite(&tableau) << endl;
    affiche(&tableau);


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
