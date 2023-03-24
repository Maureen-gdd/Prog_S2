#ifndef HUFFMANNNODE_H
#define HUFFMANNNODE_H

#include <QString>

#include "binarytree.h"
#include "array.h"

struct HuffmanNode : public Node
{
    HuffmanNode* left;
    HuffmanNode* right;

    int frequences;
    unsigned char character;
    std::string code;

    HuffmanNode(unsigned char c='\0', int frequences=0) : Node(frequences)
    {
        this->frequences = frequences;
        this->character = c;
        this->code = std::string();
        this->left = this->right = NULL;
    }

    HuffmanNode(const HuffmanNode& node): Node(node)
    {
        this->frequences = node.frequences;
        this->character = node.character;
        this->code = std::string(node.code);
        this->left = this->right = NULL;
    }

    virtual bool isLeaf() const {return !this->left && !this->right;}

    void insertNode(HuffmanNode*);
    void processCodes(const std::string &baseCode);
    void fillCharactersArray(std::string nodes_for_chars[]);


    virtual ~HuffmanNode() {}
    virtual QString toString() const override
        {return QString("%1:\n%2").arg(QString(character)).arg(frequences);}
    Node* get_left_child() const override {return left;};
    Node* get_right_child() const override {return right;};
};


class HuffmanHeap : public TemplateArray<HuffmanNode*>
{
public:
    typedef TemplateArray<HuffmanNode*> Base;

    virtual ~HuffmanHeap() {}

    HuffmanHeap(uint size=100) : TemplateArray<HuffmanNode*>()
    {
        if (size>0)
        {
            _data.resize(size);
            for (uint i=0; i<size; ++i)
                _data[i] = nullptr;
        }
    }

    QString toString() const
    {
        QStringList list;
        for (const HuffmanNode* value : _data)
        {
            if(!value)
                break;
            list.append(value->toString().replace('\n', ""));
        }
        return QString("[\n%1\n]").arg(list.join(", "));
    }


    virtual size_t effectiveSize() const
    {
        size_t size=0;
        for (const HuffmanNode* value : _data)
            if (value == nullptr)
                return size;
            else
                size++;
        return size;
    }

    HuffmanNode* get(uint index) const {
        return _data[index];
    }

    void insertHeapNode(int heapSize, HuffmanNode *newNode);
    void heapify(int heapSize, int nodeIndex);
    HuffmanNode* extractMinNode(int heapSize);

protected:
    void insert(const int index, const ElementType& value) {
        // Base::insert(index, value);
    }
};

#endif // HUFFMANNNODE_H
