#ifndef NODE_H
#define NODE_H

#include <vector>
#include <cmath>

using namespace std;

template<typename T>
class BTree;

template<typename T>
class Node {
public:
    unsigned int degree;
    unsigned int currentKeys; //Numbers of keys
    vector<unsigned int> keys;
    vector<Node<T> *> childs;
    bool isLeaf;

    Node(unsigned int size, bool isLeaf = true) : degree(size), isLeaf(isLeaf) {
        keys.resize(size - 1);
        childs.resize(size);
        currentKeys = 0;
    }

    Node<T> *search(int key) {
        int i = 0;
        while (i < this->currentKeys && key > this->keys.at(i)) {
            ++i;
        }

        if(i == this->degree - 1)
            return nullptr;   //new

        if (this->keys.at(i) == key)
            return this;

        if (this->isLeaf)
            return nullptr;

        return this->childs.at(i)->search(key);
    }

    void split(int pos, Node<T> *pNode) {
        auto newNode = new Node<T>(pNode->degree, pNode->isLeaf);
        int middle = ceil((this->degree - 1) / 2);

        newNode->currentKeys = middle - (this->degree % 2);

        //for keys
        for(int j=0 ; j < newNode->currentKeys ; ++j)
            newNode->keys.at(j) = pNode->keys.at(middle + 1 + j);

        //for children
        if(!pNode->isLeaf)
            for(int j=0 ; j <= newNode->currentKeys ; ++j) {
                newNode->childs.at(j) = pNode->childs.at(middle + 1 + j);
            }

        pNode->currentKeys = middle;

        for(int i = this->currentKeys + 1 ; i > pos + 1 ; --i)
            this->childs.at(i) = this->childs.at(i-1);

        this->childs[pos+1] = newNode;

        for(int i=this->currentKeys ; i > pos ; --i)
            this->keys.at(i) = this->keys.at(i-1);

        this->keys.at(pos) = pNode->keys.at(middle);
        ++this->currentKeys;
    }

    void insertNonFull(T data) { //according to Cormem
        int i = this->currentKeys;

        if(this->isLeaf) {
            while( i > 0 && data < this->keys.at(i-1)) {
                this->keys.at(i) = this->keys.at(i-1);
                --i;
            }
            this->keys.at(i) = data;
            ++this->currentKeys;

        } else {
            while(i > 0 && data < this->keys.at(i-1))
                --i;

            if(this->childs.at(i)->currentKeys == this->degree - 1) { // is full?
                split(i, this->childs[i] );

                if(this->keys[i] < data)
                    ++i;
            }
            this->childs[i]->insertNonFull(data);
        }
    }

    void inOrderPrinting() {
        int index;
        for (index = 0; index < this->currentKeys; ++index) {
            if (!this->isLeaf)
                this->childs.at(index)->inOrderPrinting();
            cout << " " << this->keys.at(index);
        }

        // Last child
        if (!this->isLeaf)
            this->childs.at(index)->inOrderPrinting();
    }

    /**
     * An alternative is to create two different nodes (Internal and Leaf) that inherite from Node
     * an implement this function
     */
     void killSelf() {
         for(auto eleForDel : this->childs) {
             if(eleForDel)
                 eleForDel->killSelf();
         }
         delete this;

     }
    //virtual bool isLeaf() = 0;

    ~Node() {
         this->childs.clear();
         this->keys.clear();
     }

    friend class BTree<T>;
};

#endif