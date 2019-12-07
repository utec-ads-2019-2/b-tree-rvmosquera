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
    unsigned int minKeys;
    unsigned int currentKeys; //Numbers of keys
    vector<unsigned int> keys;
    vector<Node<T> *> childs;
    bool isLeaf;

    Node(unsigned int size, bool isLeaf = true) : degree(size), isLeaf(isLeaf) {
        keys.resize(size - 1);
        childs.resize(size);
        currentKeys = 0;
        minKeys = ceil(size/2) - 1;
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
        //clear the rest of pNode
        for(int j=middle ; j < pNode->degree-1 ;++j)
            pNode->keys.at(j) = 0;
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

    bool remove(int key) {
        bool result=true;
        auto index = getIndexOfKey(key);

        if(index < this->currentKeys && this->keys.at(index) == key) {
            if(this->isLeaf)
                removefromLeaf(index);
            else
                removefromNonLeaf(index);
        } else {
            if(this->isLeaf) {
                return false;
            }

            //if(!result)
            //    throw runtime_error("can't find the key");

            auto exists = index == this->currentKeys;

            if(this->childs.at(index)->currentKeys < this->minKeys )
                fill(index);

            if(exists && index > this->currentKeys)
                this->childs.at(index-1)->remove(key);
            else
                this->childs.at(index)->remove(key);
        }

        return result;
    }

    void fill(int index) {
        if (index != 0 && this->childs.at(index-1)->currentKeys >= this->minKeys)
            borrowfromPrev(index);
        else if (index != this->currentKeys && this->childs.at(index+1)->currentKeys >= this->minKeys)
            borrowfromNext(index);
        else {
            if (index != this->currentKeys)
                merge(index);
            else
                merge(index-1);
        }
    }

    void borrowfromPrev(int index) {
        auto nodeChild = this->childs.at(index);
        auto nodeSibling = this->childs.at(index - 1);

        for(int i= nodeChild->currentKeys - 1 ; i >= 0; --i)
            nodeChild->keys.at(i + 1) = nodeChild->keys.at(i);

        if(!nodeChild->isLeaf)
            for(int i=nodeChild->currentKeys ; i >= 0 ; --i)
                nodeChild->childs.at(i + 1) = nodeChild->childs.at(i);

        nodeChild->keys.at(0) = this->keys.at(index - 1);

        if(!nodeChild->isLeaf)
            nodeChild->childs.at(0) = nodeSibling->childs.at(nodeSibling->currentKeys );

        this->keys.at(index-1) = nodeSibling->keys.at(nodeSibling->currentKeys - 1 );

        nodeChild->currentKeys++;
        nodeSibling->currentKeys--;
    }

    void borrowfromNext(int index) {
        auto nodeChild = this->childs.at(index);
        auto nodeSibling = this->childs.at(index+1);

        nodeChild->keys.at( nodeChild->currentKeys ) = this->keys.at(index);

        if (!nodeChild->isLeaf)
            nodeChild->childs.at( nodeChild->currentKeys+1 ) = nodeSibling->childs.at(0);

        this->keys.at(index) = nodeSibling->keys.at(0);

        for(int i=1; i<nodeSibling->currentKeys; ++i)
            nodeSibling->keys.at(i-1) = nodeSibling->keys.at(i);

        if(!nodeSibling->isLeaf)
            for(int i=1; i<=nodeSibling->currentKeys ; ++i)
                nodeSibling->childs.at(i-1) = nodeSibling->childs.at(i);

        nodeChild->currentKeys = nodeChild->currentKeys + 1;
        nodeSibling->currentKeys = nodeSibling->currentKeys - 1;
    }

    void removefromLeaf(int index) {
        for(int i=index+1; i < this->currentKeys ; ++i)
            this->keys.at(i-1) = this->keys.at(i);

        //clear the rest of keys
        for(int j=this->currentKeys-1 ; j < this->degree - 1;++j)
            this->keys.at(j) = 0;

        --this->currentKeys;
    }

    void removefromNonLeaf(int index) {
        auto key = this->keys.at(index);

        if ( this->childs.at(index)->currentKeys >= this->minKeys) {

            auto previous = this->getPreviousKey(index);
            this->keys.at(index) = previous;

            this->childs.at(index)->remove(previous);

        } else if ( this->childs.at(index+1)->currentKeys >= this->minKeys) {

            auto next = this->getNextKey(index);
            this->keys.at(index) = next;

            this->childs.at(index+1)->remove(next);
        } else {
            merge(index);
            this->childs.at(index)->remove(key);
        }
    }

    void merge(int index) {
        auto child = this->childs.at(index);
        auto sibling = this->childs.at(index+1);

        child->keys.at(this->minKeys-1) = this->keys.at(index);

        for(int i=0; i<sibling->currentKeys; ++i)
            child->keys.at(i+this->minKeys) = sibling->keys.at(i);

        if(!child->isLeaf)
            for(int i=0; i<sibling->currentKeys;++i)
                child->childs.at(i+this->minKeys) = sibling->childs.at(i);

        for(int i=index+1; i < this->currentKeys ; ++i)
            this->keys.at(i-1) = this->keys.at(i);

        for(int i=index+2; i < this->currentKeys ; ++i)
            this->childs.at(i-1) = this->childs.at(i);

        child->currentKeys = child->currentKeys + sibling->currentKeys + 1;
        --this->currentKeys;

        //clear the rest of keys
        for(int j=this->currentKeys ; j < this->degree - 1;++j)
            this->keys.at(j) = 0;

        delete sibling;
    }

    T getPreviousKey(int index) {
        auto current = this->childs.at(index);

        while(!current->isLeaf)
            current = current->childs.at(current->currentKeys);

        return current->keys.at( current->currentKeys-1);
    }

    T getNextKey(int index) {
        auto current = this->childs.at(index+1);

        while(!current->isLeaf)
            current = current->childs.at(0);

        if (!current->keys.empty())
            return current->keys.at(0);
        else
            return 0;
    }

    int getIndexOfKey(int key) {
        int index=0;

        while(index < this->currentKeys && this->keys.at(index) < key )
            ++index;

        return index;
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
         if (this->childs.empty())
             return;

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