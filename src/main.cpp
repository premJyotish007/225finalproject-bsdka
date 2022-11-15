
#include "Network.h"
using namespace std;
int main() {
    // Creates the network map for the first data_size number of users.
    // int data_size = 10000;
    // Network n(data_size);
    // n.build_network_from_file("data/Gowalla_edges.txt");
    // n.store_coordintes_timestamps("data/Gowalla_totalCheckins.txt");

    Network n("checkins.json", "network.json");
    return 0;
}