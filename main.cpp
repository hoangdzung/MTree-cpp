#include <iostream>
#include "Mtree.hpp"

int main(int argc, char const *argv[]) {
    
    Mtree mtree {4};
    // Node * leaf = new Node(false, &mtree);
    // mtree.root = leaf;
    for (float i = 0 ; i < 10;i++) {
        float features[5] = {i+1,i+2,i+3,i+4,i+5};
        Embedding embedding = {features, 5};
        mtree.addObject(embedding);
        std::cout << mtree.size << std::endl;
    }
    return 0;
}
