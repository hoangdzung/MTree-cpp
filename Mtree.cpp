#include <iostream>
#include <queue>
#include "Mtree.hpp"

void Mtree::addObject(Embedding embedding) {
    this->size ++;
    // std::cout << "Check info of node root " << std::endl;
    // std::cout << "isLeaf " << this->root->isLeaf << " number of entries " << this->root->entries.size() << std::endl;
    // std::cout << "isRoot " << this->root->isRoot() << std::endl;
    this->root->addObject(embedding);
}

Mtree::Mtree(int maxNodeSize_) {
    maxNodeSize = maxNodeSize_;
    this->root = new Node(true, this);
}

std::vector<Embedding> queryRange(Node * N, Embedding embedding, float range){
    std::vector<Embedding> results;
    Entry * Op = N->parentEntry;
    if (!N->isLeaf) {
        for (auto Or : N->entries) {
            if (abs( distance(*(Op->embedding), embedding) - Or.distanceToParent ) <= range + Or.radius) {
                if (distance(*(Or.embedding), embedding) <= range + Or.radius) {
                    std::vector<Embedding> tempResults = queryRange(Or.subTree, embedding, range);
                    results.insert(results.end(), tempResults.begin(), tempResults.end());
                }
            }
        }
    } else {
        for (auto Oj : N->entries) {
            if (abs( distance(*(Op->embedding), embedding) - Oj.distanceToParent ) <= range) {
                if (distance(*(Oj.embedding), embedding) <= range) {
                    results.push_back(*(Oj.embedding));
                }
            }
        }
    }
    return results;
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
    // int i =0;
    // for (auto entry : allEntries) {
    //     if (i==0) routingObject1 = *(entry.embedding);
    //     if (i==1) routingObject2 = *(entry.embedding);
    //     i++;
    //     // printEmbedding(*(entry.embedding));
    //     // if (i>1) return;
    // }
    float maxDistance = 0;
    for (int i=0; i < allEntries.size() ;i++) {
        for (int j=i; j< allEntries.size(); j++) {
            float dist = distance(*(allEntries.at(i).embedding), *(allEntries.at(j).embedding));
            if (dist>maxDistance) {
                routingObject1 = *(allEntries.at(i).embedding);
                routingObject2 = *(allEntries.at(j).embedding);
                maxDistance = dist;
            }
        }
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

void printTree(Mtree mtree) {
    std::queue<Node*> nodeQueue;
    std::cout << "Queue is created" << std::endl;
    std::cout << "Add root of tree " << mtree.root << std::endl;
    nodeQueue.push(mtree.root);
    while(!nodeQueue.empty()) {
        Node * node = nodeQueue.front();
        std::cout << "-------------------------" << std::endl;
        if (node->isLeaf)
            std::cout << "Address of leaf node " << node << std::endl;
        else 
            std::cout << "Address of inner node " << node << std::endl;
        // ->embedding->features[0] <<  std::endl;
        nodeQueue.pop();

        if (node == NULL) continue;
        if (node->parentEntry) {
            std::cout << "parentEntry" << node->parentEntry->embedding->features[0] << "(" << node->parentEntry->radius << ")" << std::endl;
        }
        else {
            std::cout << "empty Entry" <<  std::endl;
        }
        for (auto entry : node->entries) {
            std::cout << entry.embedding->features[0] << "(" << entry.radius << ")" << " ";
            if (!node->isLeaf)
                nodeQueue.push(entry.subTree);
        }
        std::cout << std::endl;
        std::cout << "-------------------------" << std::endl;
    }
}