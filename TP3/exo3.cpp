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
        this->value = value;
        this->right = nullptr;
        this->left = nullptr;
    }

	void insertNumber(int value) 
    {
        if(value < this->value)
        {
            if(this->left == nullptr)
                this->left = new SearchTreeNode(value);
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

	uint height() const	
    {
        if(this->isLeaf())
            return 1;
        else 
        {
            uint h_left = 0;
            uint h_right = 0;

            if(this->left != nullptr)
                h_left = left->height();

            if(this->right != nullptr)
                h_right = right->height();

            return 1+std::max(h_left, h_right);
        }

    }

	uint nodesCount() const {
        // should return the sum of nodes within left child and
        // right child +1 for itself. If there is no child, return
        // just 1
        if (this->isLeaf())
            return 1;
        else
        {
            uint left_child = 0;
            uint right_child = 0;

            if(this->left != nullptr)
                left_child = left->nodesCount();
            if(this->right != nullptr)
                right_child = right->nodesCount();

            return 1 + left_child + right_child;
        }

	}

	bool isLeaf() const 
    {
        // return True if the node is a leaf (it has no children)
        if(this->left == nullptr && this->right == nullptr)
            return true;
        else
            return false;
	}

	void allLeaves(Node* leaves[], uint& leavesCount)
    {
        // fill leaves array with all leaves of this tree
        if(this->isLeaf())
        {
            leaves[leavesCount] = this;
            leavesCount++;
        } 
        else
        {
            if(left != nullptr)
                left->allLeaves(leaves, leavesCount);
            if(right != nullptr)
                right->allLeaves(leaves, leavesCount);
        }

	}

	void inorderTravel(Node* nodes[], uint& nodesCount) 
    {
        // fill nodes array with all nodes with inorder travel
        if(left!=nullptr)
            left->inorderTravel(nodes, nodesCount);

        nodes[nodesCount] = this;
        nodesCount++;

        if(right != nullptr)
            right->inorderTravel(nodes, nodesCount);
	}

	void preorderTravel(Node* nodes[], uint& nodesCount)
    {
        // fill nodes array with all nodes with preorder travel
        nodes[nodesCount]=this;
        nodesCount++;

        if(left != nullptr)
            left->preorderTravel(nodes, nodesCount);

        if(right != nullptr)
            right->preorderTravel(nodes, nodesCount);
	}

	void postorderTravel(Node* nodes[], uint& nodesCount) 
    {
        // fill nodes array with all nodes with postorder travel
        if(left != nullptr)
            left->postorderTravel(nodes, nodesCount);
        
        if(right!=nullptr)
            right->postorderTravel(nodes, nodesCount);
        
        nodes[nodesCount]=this;
        nodesCount++;
	}

	Node* find(int value) 
    {
        // find the node containing value
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
