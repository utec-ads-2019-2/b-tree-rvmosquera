#ifndef BTREE_H
#define BTREE_H

#include "node.h"

template <typename T>
class BTree {
    private:
        Node<T>* root;
        unsigned int degree;

    public:
        bool allowRepetition;

        BTree(unsigned int degree) : degree(degree), root(nullptr) {
            allowRepetition = false;
        };

        bool find(int key) {
            if (!this->root)
                return false;
            else
                return this->root->search(key) != nullptr;
        } 

        bool insert(T data) {
            if( find(data) && !allowRepetition) { //no duplicates
                return false;
            }

            if(!this->root) { //Empty
                this->root = new Node<T>(this->degree, true);
                this->root->keys.at(0) = data;
                this->root->currentKeys = 1;

            } else { // not empty
                if(this->root->currentKeys == this->degree - 1) { //is full?
                    auto newRoot = new Node<T>(this->degree, false);

                    newRoot->childs.at(0) = this->root;
                    newRoot->split(0, this->root);

                    int i = 0;
                    if( newRoot->keys.at(0) < data)
                        ++i;

                    newRoot->childs.at(i)->insertNonFull(data);

                    this->root = newRoot;
                } else {
                    this->root->insertNonFull(data);
                }
            }

            return true;
        }

        int height() {
            return 0;
        }

        bool remove(int key) {
            bool result;

            if(!this->root) {
                result = false;
                throw runtime_error("the tree is empty");
            }

            result = this->root->remove(key);

            if(!this->root->currentKeys) {//0
                auto tmp = this->root;

                if(this->root->isLeaf)
                    this->root = nullptr;
                else {
                    this->root = this->root->childs.at(0);

                    if( this->root->isLeaf ) {
                        this->root = nullptr;
                        delete this->root;
                    }

                }

                delete tmp;
            }

            return result;
        }

        void print() {
            if(this->root)
                this->root->inOrderPrinting();
            cout<<endl;
        }

        ~BTree() {
            if(root)
                root->killSelf();

        }
};

#endif