#include "Mtree.hpp"

void Mtree::addObject(Embedding embedding) {
    size ++;
    this->root->addObject(embedding);
}

Mtree::Mtree(int maxNodeSize_) :maxNodeSize(maxNodeSize_) {
    this->root = new Node(false, this);
}