#ifndef MTREE_H
#define MTREE_H

#include <iostream>
#include <cmath>
#include <vector>
#include <set>
#include <limits>
#include <assert.h> 

struct Embedding;
struct Node;
struct Entry;
struct Mtree;

float distance(Embedding x, Embedding y);
void promote(std::set<Entry> allEntries, Embedding& routingObject1, Embedding& routingObject2);
void partition(std::set<Entry> allEntries, std::set<Entry>& entries1, std::set<Entry>& entries2, const Embedding& routingObject1, const Embedding& routingObject2);
void printEmbedding(Embedding embedding);

struct Embedding {
    float * features;
    int len;
    public:
        // Embedding(float * features_, int len_) :
        // features(features_), len(len_) {}
        // Embedding() :
        // features(), len() {}
        Embedding(float * features_, int len_) {
            features = new float[len_]();
            len = len_;
            for (int i =0 ; i< len;i++) {
                features[i] = features_[i];
            }
        }
        Embedding() {}
};

struct Mtree {
    int maxNodeSize = 5;
    int size = 0;
    Node * root;
    public:
        Mtree(int maxNodeSize_);

    void addObject(Embedding embedding);

};

struct Entry {
    Embedding * embedding;
    float distanceToParent;
    float radius;
    Node * subTree;
    public:
        Entry(Embedding * embedding_, float distanceToParent_, float radius_, Node * subTree_) :
        subTree(subTree_) {
            embedding = new Embedding();
            *embedding = *embedding_;
            distanceToParent = distanceToParent_;
            radius = radius_;
            subTree = subTree_;
        }
    bool operator < (const Entry &other) const { 
        for (int i=0; i < embedding->len; i++)
            if (embedding->features[i] != other.embedding->features[i]) return embedding->features[i] < other.embedding->features[i];
        return true;
    }

    bool operator == (const Entry &other) {
        for (int i=0; i < embedding->len; i++)
            if (embedding->features[i] != other.embedding->features[i]) return false;
        if (subTree != other.subTree) return false;
        if (distanceToParent != other.distanceToParent) return false;
        if (radius != other.radius) return false;

        return true;
    }
};

struct Node {
    bool isLeaf = true; 
    Mtree * mtree;
    Node * parentNode;
    std::set<Entry> entries;
    Entry * parentEntry;
    public:
        Node(bool isLeaf_, Mtree * mtree_, Node * parentNode_, std::set<Entry> entries_, Entry * parentEntry_):
        isLeaf(isLeaf_), mtree(mtree_), parentNode(parentNode_), entries(entries_), parentEntry(parentEntry_) {}
        
        Node(bool isLeaf_, Mtree * mtree_) {
            isLeaf = isLeaf_;
            mtree = mtree_;
        }

    bool isFull() {
        return entries.size() == mtree->maxNodeSize;
    }

    bool isRoot() {
        return (this == this->mtree->root);
    }

    void setEntriesAndParentEntry(std::set<Entry> entries, Entry* parentEntry) {
        this->entries = entries;
        this->parentEntry = parentEntry;
        this->parentEntry->radius = updateRadius(parentEntry->embedding);
        this->updateEntryDistanceToParent();
    
    }

    float updateRadius(Embedding* embedding) {
        float maxRadius = 0;
        if (this->isLeaf) {
            for (auto entry : this->entries) {
                float radius = distance(*embedding, *(entry.embedding));
                if (radius > maxRadius) 
                    maxRadius = radius;
            }
        } else {
            for (auto entry : this->entries) {
                float radius = distance(*embedding, *(entry.embedding)) + entry.radius;
                if (radius > maxRadius) 
                    maxRadius = radius;
            }
        }
        return maxRadius;
    }

    void updateEntryDistanceToParent() {
        if (this->parentEntry != NULL) {
            for (auto entry : this->entries) {
                entry.distanceToParent = distance(*(entry.embedding), *(this->parentEntry->embedding));
            }
        }
    }

    void addObject(Embedding embedding) {
        std::cout << "Address of node " << this << std::endl;
        std::cout << "Size of node " << this->entries.size() << std::endl;
        if (this->isLeaf) {
            std::cout << "addObjectToLeaf" << std::endl;
            this->addObjectToLeaf(embedding);
        } else {
            std::cout << "addObjectToInner" << std::endl; 
            this->addObjectToInner(embedding);
        }
        std::cout << "Inside addObject Method of Node: Done" << std::endl;
    }

    void addObjectToLeaf(Embedding embedding) {
        float distanceToParent = -1;
        std::cout << "Address of parent Entry " << this->parentEntry << std::endl;
        std::cout << "Size of entries " << this->entries.size() << std::endl;
        // if (this->parentEntry)  
        //     std::cout << "Address of embedding of parent Entry " << this->parentEntry->embedding << std::endl;
        if (this->parentEntry) {
            if (this->parentEntry->embedding) {
                distanceToParent = distance(embedding, *(this->parentEntry->embedding));
            }
        }
        std::cout << "distanceToParent " << distanceToParent << std::endl;
        // std::cout << "new embedding " << std::endl;
        // printEmbedding(embedding);
        // std::cout << "saved  embedding " << std::endl;
        // for (auto entry : this->entries) {
        //     printEmbedding(*(entry.embedding));
        // }
        Entry newEntry = {&embedding, distanceToParent, -1, NULL};
        if (!this->isFull()) {
            std::cout << "Leaf is not full" << std::endl;
            this->entries.insert(newEntry);
        } else {
            std::cout << "Split node" << std::endl;
            this->split(newEntry);
        }
        assert(this->isRoot() || this->parentNode);
        // for (auto entry : this->entries) {
        //     printEmbedding(*(entry.embedding));
        // }
    }

    void addObjectToInner(Embedding embedding) {
        Entry* bestEntry;
        float minDistance = std::numeric_limits<float>::max();
        for (auto entry : this->entries) {
            float distanceToObject = distance(*(entry.embedding), embedding) - entry.radius;
            if (distanceToObject < minDistance) {
                minDistance = distanceToObject;
                bestEntry = &entry;
            }
        }
        if (minDistance < 0)
            (*bestEntry).radius += minDistance;
        std::cout << bestEntry->subTree << std::endl;
        bestEntry->subTree->addObject(embedding);

        assert(this->isRoot() || this->parentNode);
    }

    void split(Entry newEntry) {
        std::cout << "begin =========================\n";
        assert(this->isFull());
        Mtree* mtree = this->mtree;
        Node* newNode = new Node (this->isLeaf, mtree);
        std::cout << "Create new node at address " << newNode << std::endl;
        std::set<Entry> allEntries = this->entries;
        allEntries.insert(newEntry);
        Embedding routingObject1, routingObject2;
        promote(allEntries, routingObject1, routingObject2);

        std::set<Entry> entries1, entries2;
        partition(allEntries, entries1, entries2, routingObject1, routingObject2);
        std::cout << "After partition " << entries1.size() << " " << entries2.size() << std::endl;
        Entry* oldParentEntry = this->parentEntry;
        std::cout << "Address of oldParentEntry " << oldParentEntry << std::endl;
    
        // Entry existNodeEntry {&routingObject1, -1, -1, this};
        Entry * existNodeEntry = new Entry (&routingObject1, -1, -1, this);
        std::cout << "Create new existNodeEntry at address " << existNodeEntry << std::endl;
        this->setEntriesAndParentEntry(entries1, existNodeEntry);

        // Entry newNodeEntry {&routingObject2, -1, -1, newNode};
        Entry * newNodeEntry = new Entry (&routingObject2, -1, -1, newNode);
        std::cout << "Create new newNodeEntry at address " << newNodeEntry << std::endl;
        newNode->setEntriesAndParentEntry(entries2, newNodeEntry);

        if (this->isRoot()) {
            std::cout << "The current node is root" << std::endl;
            Node * newRootNode = new Node (false, mtree);
            std::cout << "Create new root node at address " << newRootNode << std::endl;
            this->parentNode = newRootNode;
            newRootNode->entries.insert(*existNodeEntry);
            newNode->parentNode = newRootNode;
            newRootNode->entries.insert(*newNodeEntry);
            mtree->root = newRootNode;

        } else {
            std::cout << "The current node is not root" << std::endl;
            Node * parentNode = this->parentNode;
            std::cout << "Address of parentNode " << parentNode << std::endl;
            if (!parentNode->isRoot()) {
                existNodeEntry->distanceToParent = distance(*(existNodeEntry->embedding), *(parentNode->parentEntry->embedding));
                newNodeEntry->distanceToParent = distance(*(newNodeEntry->embedding), *(parentNode->parentEntry->embedding));
            } 
            std::cout << "Entry side " << parentNode->entries.size() << std::endl;
            parentNode->entries.erase(*oldParentEntry);
            for (auto entry:parentNode->entries ) {
                std::cout << (entry == *oldParentEntry) << " ";
            }
            std::cout << std::endl;
            std::cout << "Entry side " << parentNode->entries.size() << std::endl;
            parentNode->entries.insert(*existNodeEntry);

            if (parentNode->isFull()) {
                std::cout << "Continue split" << std::endl;
                parentNode->split(*newNodeEntry);
                std::cout << "Split done at " << this <<  std::endl;
            }
            else {
                std::cout << "Insert directly" << std::endl;
                parentNode->entries.insert(*newNodeEntry);
                newNode->parentNode = parentNode;
            }
        }
        assert(this->isRoot() || this->parentNode);
        std::cout << newNode->isRoot() << " " << newNode->parentNode << std::endl;
        assert(newNode->isRoot() || newNode->parentNode);
        std::cout << "end =========================\n";

    }

};

#endif