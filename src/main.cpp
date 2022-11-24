
#include "network.h"
#include "../lib/cs225/PNG.hpp"
#include "../lib/cs225/HSLAPixel.hpp"

using namespace std;
using namespace cs225;
int main() {
    // Creates the network map for the first data_size number of users.
    int data_size = 1000;
    Network n(data_size);
    n.build_network_from_file("data/Gowalla_edges.txt");
    n.store_coordintes_timestamps("data/Gowalla_totalCheckins.txt");

    PNG* p = n.plot_k_depth_friends(86, 1, "2010-07-11T:10:10Z", 500);
    p->writeToFile("test.png");

    PNG* p = n.plot_k_depth_friends(86, 1, "2011-07-11T:10:10Z", 500);
    p->writeToFile("test2.png");

    // Network n("checkins.json", "network.json");
    return 0;
}