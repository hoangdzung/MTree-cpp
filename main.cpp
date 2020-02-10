#include <iostream>
#include "Mtree.hpp"

int main(int argc, char const *argv[]) {
    
    Mtree mtree {3};
    // Node * leaf = new Node(false, &mtree);
    // mtree.root = leaf;
    std::cout << "Tree's created" << std::endl;
    std::cout << mtree.maxNodeSize << " "  << mtree.root->mtree->maxNodeSize << std::endl;
    std::cout << "Address of mtree " << &mtree << " " << mtree.root->mtree << std::endl;
    std::cout << "Address of root of mtree " << mtree.root  << std::endl;
    for (float i = 0 ; i < 10;i++) {
        float features[5] = {i+1,i+2,i+3,i+4,i+5};
        Embedding embedding = {features, 5};
        std::cout << "add Object " << i << std::endl;
        mtree.addObject(embedding);
        std::cout << "Size of mtree " << mtree.size << std::endl;
        std::cout << "=======================================================" << std::endl;
    }
    return 0;
}
