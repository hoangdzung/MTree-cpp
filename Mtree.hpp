
#include <vector>
#include <set>
#include <limits>
#include <assert.h> 
#include "helper.hpp"

struct Embedding;
struct Node;
struct Entry;
struct Mtree;

struct Embedding {
    float * features;
    int len;
    public:
        Embedding(float * features_, int len_) :
        features(features_), len(len_) {}
        Embedding() :
        features(), len() {}
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
        embedding(embedding_), distanceToParent(distanceToParent_), radius(radius_), subTree(subTree_) {}

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
        
        Node(bool isLeaf_, Mtree * mtree_):
        isLeaf(isLeaf_), mtree(mtree_) {}

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
        if (this->isLeaf) 
            this->addObjectToLeaf(embedding);
        else 
            this->addObjectToInner(embedding);
    }

    void addObjectToLeaf(Embedding embedding) {
        float distanceToParent = -1;
        if (this->parentEntry->embedding != NULL)
            distanceToParent = distance(embedding, *(this->parentEntry->embedding));
        Entry newEntry = {&embedding, distanceToParent, -1, NULL};
        if (!this->isFull())
            this->entries.insert(newEntry);
        else
            this->split(newEntry);
        assert(this->isRoot() || this->parentNode);

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
        bestEntry->subTree->addObject(embedding);

        assert(this->isRoot() || this->parentNode);
    }

    void split(Entry newEntry) {
        assert(this->isFull());
        Mtree* mtree = this->mtree;
        Node* newNode = new Node (this->isLeaf, mtree, NULL, std::set<Entry>(), NULL);
        std::set<Entry> allEntries = this->entries;
        allEntries.insert(newEntry);
        Embedding routingObject1, routingObject2;
        promote(allEntries, routingObject1, routingObject2);

        std::set<Entry> entries1, entries2;
        partition(allEntries, entries1, entries2);

        Entry* oldParentEntry = this->parentEntry;

        Entry existNodeEntry {&routingObject1, -1, -1, this};
        this->setEntriesAndParentEntry(entries1, &existNodeEntry);

        Entry newNodeEntry {&routingObject2, -1, -1, newNode};
        newNode->setEntriesAndParentEntry(entries2, &newNodeEntry);

        if (this->isRoot()) {
            Node * newRootNode = new Node (false, mtree, NULL, std::set<Entry>(), NULL);
            this->parentNode = newRootNode;
            newRootNode->entries.insert(existNodeEntry);
            newNode->parentNode = newRootNode;
            newRootNode->entries.insert(newNodeEntry);
            mtree->root = newRootNode;
        } else {
            Node * parentNode = this->parentNode;
            if (!parentNode->isRoot()) {
                existNodeEntry.distanceToParent = distance(*(existNodeEntry.embedding), *(parentNode->parentEntry->embedding));
                newNodeEntry.distanceToParent = distance(*(newNodeEntry.embedding), *(parentNode->parentEntry->embedding));
            } 
            parentNode->entries.erase(*oldParentEntry);
            parentNode->entries.insert(existNodeEntry);

            if (parentNode->isFull()) 
                parentNode->split(newNodeEntry);
            else {
                parentNode->entries.insert(newNodeEntry);
                newNode->parentNode = parentNode;
            }
        }
        assert(this->isRoot() || this->parentNode);
        assert(newNode->isRoot() || newNode->parentNode);
    }

};


