#include <iostream>
#include "Mtree.hpp"

void Mtree::addObject(Embedding embedding) {
    this->size ++;
    std::cout << "Check info of node root " << std::endl;
    std::cout << "isLeaf " << this->root->isLeaf << " number of entries " << this->root->entries.size() << std::endl;
    std::cout << "isRoot " << this->root->isRoot() << std::endl;
    this->root->addObject(embedding);
}

Mtree::Mtree(int maxNodeSize_) {
    maxNodeSize = maxNodeSize_;
    this->root = new Node(true, this);
}

float distance(Embedding x, Embedding y) {
    assert (x.len == y.len);
    float dist = 0;
    for (int i =0 ;i < x.len; i++) {
        dist += pow(x.features[i] - y.features[i],2);
    }
    return sqrt(dist);
}

void promote(std::vector<Entry> allEntries, Embedding& routingObject1, Embedding& routingObject2) {
    // Temporally random pick
    int i =0;
    for (auto entry : allEntries) {
        if (i==0) routingObject1 = *(entry.embedding);
        if (i==1) routingObject2 = *(entry.embedding);
        i++;
        // printEmbedding(*(entry.embedding));
        // if (i>1) return;
    }
}

void partition(std::vector<Entry> allEntries, std::vector<Entry>& entries1, std::vector<Entry>& entries2, const Embedding& routingObject1, const Embedding& routingObject2) {
    // std::cout << "routingObject1 ";
    // printEmbedding(routingObject1);
    // std::cout << "routingObject2 ";
    // printEmbedding(routingObject2);

    for (auto entry : allEntries) {
        // printEmbedding(*(entry.embedding));
        // std::cout << (distance(*(entry.embedding), routingObject1) < distance(*(entry.embedding), routingObject2)) << std::endl;
        if (distance(*(entry.embedding), routingObject1) < distance(*(entry.embedding), routingObject2) )
            entries1.push_back(entry);
        else 
            entries2.push_back(entry);
    }
}

void printEmbedding(Embedding embedding) {
    std::cout << "[ ";
    for (int i =0;i < embedding.len;i++)
        std::cout << embedding.features[i] << " ";
    std::cout << " ]" << std::endl;
}