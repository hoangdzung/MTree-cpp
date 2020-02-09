#include <assert.h> 
#include <cmath>
#include "helper.hpp"

float distance(Embedding x, Embedding y) {
    assert (x.len == y.len);
    float dist = 0;
    for (int i =0 ;i < x.len; i++) {
        dist += pow(x.features[i] - y.features[i],2);
    }
    return sqrt(dist);
}

void promote(std::set<Entry> allEntries, Embedding& routingObject1, Embedding& routingObject2) {

}

void partition(std::set<Entry> allEntries, std::set<Entry>& entries1, std::set<Entry>& entries2) {
    
}