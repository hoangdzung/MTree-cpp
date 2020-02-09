#ifndef HELPER_H
#define HELPER_H

#include "Mtree.hpp"

float distance(Embedding x, Embedding y);
void promote(std::set<Entry> allEntries, Embedding& routingObject1, Embedding& routingObject2);
void partition(std::set<Entry> allEntries, std::set<Entry>& entries1, std::set<Entry>& entries2);

#endif