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
    if(tableau->nb_elem == 0)
        cout << "Le tableau est vide" << endl;
    else
    {
        for (int i = 0; i < tableau->nb_elem; i++)
        {
            cout << "Valeur de la case n°" << i << " = " << tableau->donnees[i] << endl;
        }
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
    for(size_t i = 0; i < tableau->nb_elem; i++)
    {
        if(tableau->donnees[i] == valeur)
            return i;
    }

    return -1;
}

int stocke(DynaTableau* tableau, int n, int valeur)
{
    if(n >= tableau->capacite || n < 0)
        return -1;

    tableau->donnees[n] = valeur;
    return 1;
}

int getCapacite(DynaTableau* tableau)
{
    return tableau->capacite;
}

/* ********** FIFO / LIFO ********** */

// mettre au début
void pousse_file(DynaTableau* tableau, int valeur)
{
    // si le tableau est vide
    if(tableau->nb_elem == 0)
        tableau->donnees[0] = valeur;
    else
    {
        int* newTableau = new int[tableau->capacite + 1];
        newTableau[0] = valeur;
        // il faut agrandir la capacité
        if(tableau->nb_elem >= tableau->capacite)
        {
            for (int i = 0; i < tableau->capacite; i++)
            {
                newTableau[i+1] = tableau->donnees[i];
            }
            tableau->capacite++;
        }
        // pas besoin d'agrandir la capacite
        else
        {
            /*int* newTableau = new int[tableau->capacite];
            newTableau[0] = valeur;*/

            for (int i = 0; i < tableau->nb_elem; i++)
            {
                newTableau[i+1] = tableau->donnees[i];
            }
        }

        delete[] tableau->donnees;
        tableau->donnees = newTableau;
    }

    tableau->nb_elem++;
}

// retire à la fin
int retire_file(DynaTableau* tableau)
{
    // Tableau vide
    if(tableau->nb_elem == 0)
        return -1;
    else
    {
        int* newTableau = new int[tableau->capacite-1];
        int valeur = tableau->donnees[0];

        for(size_t i = 1; i < tableau->nb_elem; i++)
        {
            newTableau[i-1] = tableau->donnees[i];
        }

        delete[] tableau->donnees;
        tableau->donnees = newTableau;
        tableau->nb_elem -= 1;

        return valeur;
    }
}

// ajoute à la fin
void pousse_pile(DynaTableau* tableau, int valeur)
{
    // si le tableau est vide
    if(tableau->nb_elem == 0)
    {
        tableau->donnees[0] = valeur;
        tableau->nb_elem++;
    }
    else
    {
        // il faut agrandir la capacité
        if(tableau->nb_elem >= tableau->capacite)
        {
            ajoute(tableau, valeur);
        }
        // pas besoin d'agrandir la capacite
        else
        {
            int* newTableau = new int[tableau->capacite];
            newTableau[tableau->nb_elem] = valeur;

            for (size_t i = 0; i < tableau->nb_elem; i++)
            {
                newTableau[i] = tableau->donnees[i];
            }

            delete[] tableau->donnees;
            tableau->donnees = newTableau;
            tableau->nb_elem++;
        }
    }
}

//int 
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
    cout << "\nF O N C T I O N   R E C U P E R E" << endl;
    cout << "Je vais chercher un élément en dehors de la capacité du tableau: " << endl;
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

    cout << "\nF O N C T I O N   A J O U T E" << endl;
    cout << "J'ajoute sans dépasser la capacité du tableau." << endl;
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

    cout << "\nF O N C T I O N   C H E R C H E" << endl;
    int nombre = 21;
    cout << "Je cherche un nombre qui ne se trouve pas dans le tableau" << endl;
    recup = cherche(&tableau, nombre);
    if(indice != -1)
        cout << "Le nombre " << nombre << " se trouve à la case " << recup << endl;
    else
        cout << "Le nombre " << nombre << " ne se trouve pas dans le tableau" << endl;

    nombre = 8;
    cout << "\nJe cherche un nombre qui se trouve dans le tableau" << endl;
    indice = cherche(&tableau, nombre);
    if(indice != -1)
        cout << "Le nombre " << nombre << " se trouve à la case " << indice << endl;
    else
        cout << "Le nombre " << nombre << " ne se trouve pas dans le tableau" << endl;

    cout << "\nF O N C T I O N   S T O C K E" << endl;
    nombre = 169;
    indice = 66;
    cout << "Je stocke un nombre avec un mauvais indice:" << endl;
    cout << "Indice: " << indice << endl;
    recup = stocke(&tableau, indice, nombre);
    if(recup == -1)
        cout << "L'indice où vous voulez stocker votre valeur est trop petit/grand pour le tableau" << endl;
    else
        affiche(&tableau);

    indice = 5;
    cout << "\nJe stocke un nombre avec un bon indice:" << endl;
    cout << "Indice: " << indice << endl;
    recup = stocke(&tableau, indice, nombre);
    if(recup == -1)
        cout << "L'indice où vous voulez stocker votre valeur est trop petit/grand pour le tableau" << endl;
    else
        affiche(&tableau);


    cout << "\nF O N C T I O N   P O U S S E   F I L E" << endl;
    DynaTableau tableau2;
    initialise(&tableau2, 2);
    affiche(&tableau2);

    cout << "\nJ'utilise pousse_file sur un tableau vide" << endl;
    pousse_file(&tableau2, 66);
    cout << "Capacite: " << getCapacite(&tableau2) << endl;
    affiche(&tableau2);

    cout << "\nJ'utilise pousse_file sur un tableau pas entièrement rempli" << endl;
    pousse_file(&tableau2, 54);
    cout << "Capacite: " << getCapacite(&tableau2) << endl;
    affiche(&tableau2);

    cout << "\nJ'utilise pousse_file sur un tableau entièrement rempli" << endl;
    pousse_file(&tableau2, 21);
    cout << "Capacite: " << getCapacite(&tableau2) << endl;
    affiche(&tableau2);

    cout << "\nF O N C T I O N   R E T I R E   F I L E" << endl;
    DynaTableau tableau3;
    initialise(&tableau3, 3);
    affiche(&tableau3);
    cout << "\nJ'utilise retire_file sur un tableau vide" << endl;
    recup = retire_file(&tableau3);
    if(recup == -1)
         cout << "Votre tableau est vide, il n'y a rien à retirer" << endl;
     else
     {
        cout << "La valeau enlevée est " << recup << endl;
        affiche(&tableau3);
     }

    cout << "\nJ'utilise retire_file sur un tableau rempli" << endl;
    for(size_t i = 1; i <= 3; i++)
    {
        ajoute(&tableau3, i*5);
    }
    affiche(&tableau3);
    recup = retire_file(&tableau3);
    if(recup == -1)
         cout << "\nVotre tableau est vide, il n'y a rien à retirer" << endl;
    else
    {
        cout << "\nLa valeau enlevée est " << recup << endl;
        affiche(&tableau3);
    }

    cout << "\nF O N C T I O N   P O U S S E   P I L E" << endl;
    DynaTableau tableau4;
    initialise(&tableau4, 2);
    affiche(&tableau4);

    cout << "\nJ'utilise pousse_pile sur un tableau vide" << endl;
    pousse_pile(&tableau4, 66);
    cout << "Capacite: " << getCapacite(&tableau4) << endl;
    affiche(&tableau4);

    cout << "\nJ'utilise pousse_pile sur un tableau pas entièrement rempli" << endl;
    pousse_pile(&tableau4, 54);
    cout << "Capacite: " << getCapacite(&tableau4) << endl;
    affiche(&tableau4);

    cout << "\nJ'utilise pousse_pile sur un tableau entièrement rempli" << endl;
    pousse_pile(&tableau4, 21);
    cout << "Capacite: " << getCapacite(&tableau4) << endl;
    affiche(&tableau4);



    return 0;
}
