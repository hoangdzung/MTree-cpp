#include <iostream>
#include <vector>
#include "Mtree.hpp"

int main(int argc, char const *argv[]) {
    
    Mtree mtree {2};
    // Node * leaf = new Node(false, &mtree);
    // mtree.root = leaf;
    std::cout << "Tree's created" << std::endl;
    std::cout << mtree.maxNodeSize << " "  << mtree.root->mtree->maxNodeSize << std::endl;
    std::cout << "Address of mtree " << &mtree << " " << mtree.root->mtree << std::endl;
    std::cout << "Address of root of mtree " << mtree.root  << std::endl;
    for (float i = 0 ; i < 10;i++) {
        float features[2] = {i+1,i+2};
        Embedding embedding = {features, 2};
        std::cout << "add Object " << i+1 << std::endl;
        mtree.addObject(embedding);
        // std::cout << "Size of mtree " << mtree.size << std::endl;
        printTree(mtree);
        std::cout << mtree.root << std::endl;
        std::cout << "=======================================================" << std::endl;
    }
    // printTree(mtree);
    std::cout << "=============================================================" << std::endl;
    std::cout << "                         Query Range                         " << std::endl;
    std::cout << "=============================================================" << std::endl;
    float features[2] = {3,4};
    Embedding embedding = {features, 2};
    std::vector<Embedding> results = queryRange(mtree.root, embedding, 3);
    std::cout << "Search done" << std::endl;
    for (auto embeddingResult : results) {
        printEmbedding(embeddingResult);
    }
    return 0;
}
