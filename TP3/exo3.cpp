#include "mainwindow.h"
#include "tp3.h"
#include <QApplication>
#include <time.h>
#include <stack>
#include <queue>

MainWindow* w = nullptr;
using std::size_t;

struct SearchTreeNode : public Node
{    
    SearchTreeNode* left;
    SearchTreeNode* right;
    int value;

    void initNode(int value)
    {
        this->left = nullptr;
        this->right = nullptr;
        this->value = value;
    }

	void insertNumber(int value) 
    {
        // create a new node and insert it in right or left child
        // Si la valeur est inférieur au noeau courant
        if(value < this->value)
        {
            // Crée nouveau noeud
            if(this->left == nullptr)
                this->left = new SearchTreeNode(value);
            // Sinon appelle récursivement sur le noeud gauche
            else
                left->insertNumber(value);
        } 
        else 
        {
            if(this->right == nullptr)
                this->right = new SearchTreeNode(value);
            else
                right->insertNumber(value);
        }

    }

    // should return the maximum height between left child and
    // right child +1 for itself. If there is no child, return
    // just 1
	uint height() const	
    {
        // C'est une feuille donc pas d'enfant
        if(this->isLeaf())
            return 1;
        else 
        {
            uint h_left = 0;
            uint h_right = 0;

            // Calcule la hauteur de l'enfant gauche s'il existe
            if(this->left != nullptr)
                h_left = left->height();
            // Calcule la hauteur de l'enfant droit s'il existe
            if(this->right != nullptr)
                h_right = right->height();

            return 1+std::max(h_left, h_right);
        }

    }

    // should return the sum of nodes within left child and
    // right child +1 for itself. If there is no child, return
    // just 1
	uint nodesCount() const 
    {
        // Si c'est une feuille, il n'a pas d'enfant
        if (this->isLeaf())
            return 1;
        else
        {
            uint sum_LChild = 0;
            uint sum_RChild = 0;

            // Calcule le nombre de nœuds de l'enfant gauche s'il existe
            if(this->left != nullptr)
                sum_LChild = left->nodesCount();
            // Calcule le nombre de nœuds de l'enfant droit s'il existe
            if(this->right != nullptr)
                sum_RChild = right->nodesCount();

            return 1 + sum_LChild + sum_RChild;
        }

	}

    // return True if the node is a leaf (it has no children)
	bool isLeaf() const 
    {
        // Verification pas d'enfant à droite et à gauche
        if(this->left == nullptr && this->right == nullptr)
            return true;
        else
            return false;
	}

    // fill leaves array with all leaves of this tree
	void allLeaves(Node* leaves[], uint& leavesCount)
    {
        // Si le noeud est une feuille on l'ajoute au tableau
        if(this->isLeaf())
        {
            // On l'ajoute au tableau de feuille et on incrémente le compteur
            leaves[leavesCount] = this;
            leavesCount++;
        } 
        else
        {
            // Si y'a un enfant à gauche, on appelle la méthode sur ce dernier de manière récursive
            if(left != nullptr)
                left->allLeaves(leaves, leavesCount);
            // Idem pour l'enfant de droite
            if(right != nullptr)
                right->allLeaves(leaves, leavesCount);
        }

	}

    // fill nodes array with all nodes with inorder travel
    // fils gauche, parent, fils droite (ET POURQUOI PAS DES FILLES HEIN ?????)
	void inorderTravel(Node* nodes[], uint& nodesCount) 
    {
        // Si l'enfant gauche existe, la méthode est appelée récursivement sur l'enfant gauche
        // ce qui permet de parcourir tous les nœuds de l'enfant gauche avant le nœud courant.
        if(left != nullptr)
            left->inorderTravel(nodes, nodesCount);

        // Quand il n'y a plus d'enfant gauche, on ajoute le noeaud courant au tableau
        nodes[nodesCount] = this;
        nodesCount++;

        // On fait la même chose pour l'enfant droite
        if(right != nullptr)
            right->inorderTravel(nodes, nodesCount);
	}

    // fill nodes array with all nodes with preorder travel
    // parent, fils gauche, fils droite
	void preorderTravel(Node* nodes[], uint& nodesCount)
    {
        nodes[nodesCount] = this;
        nodesCount++;

        // Si l'enfant gauche existe, la méthode est appelée récursivement sur l'enfant gauche
        // ce qui permet de parcourir tous les nœuds de l'enfant gauche.
        if(left != nullptr)
            left->preorderTravel(nodes, nodesCount);

        // Idem avec droite
        if(right != nullptr)
            right->preorderTravel(nodes, nodesCount);
	}

    // fill nodes array with all nodes with postorder travel
    // fils gauche, fils droit, parent
	void postorderTravel(Node* nodes[], uint& nodesCount) 
    {
        if(left != nullptr)
            left->postorderTravel(nodes, nodesCount);
        
        if(right != nullptr)
            right->postorderTravel(nodes, nodesCount);
        
        // Ajout du noeaud courant / parent
        nodes[nodesCount] = this;
        nodesCount++;
	}

    // find the node containing value
	Node* find(int value) 
    {
		if(this->value == value)
            return this;
        else if(this->value > value && this->left != nullptr)
            return left->find(value);
        else if(this->right != nullptr)
            return right->find(value);

        return nullptr;
	}

    void reset()
    {
        if (left != NULL)
            delete left;
        if (right != NULL)
            delete right;
        left = right = NULL;
    }

    SearchTreeNode(int value) : Node(value) {initNode(value);}
    ~SearchTreeNode() {}
    int get_value() const {return value;}
    Node* get_left_child() const {return left;}
    Node* get_right_child() const {return right;}
};

Node* createNode(int value) {
    return new SearchTreeNode(value);
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow::instruction_duration = 200;
    w = new BinarySearchTreeWindow<SearchTreeNode>();
	w->show();

	return a.exec();
}
