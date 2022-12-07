
#include "../includes/network.h"
#include "../lib/cs225/PNG.hpp"
#include "../lib/cs225/HSLAPixel.hpp"

using namespace std;
using namespace cs225;
int main() {
    // Creates the network map for the first data_size number of users.
    // int data_size = 100000;
    // Network n(data_size);
    // n.build_network_from_file("data/Gowalla_edges.txt");
    // n.store_coordintes_timestamps("data/Gowalla_totalCheckins.txt");
    // PNG* p = n.plot_k_depth_friends(15000, 2, "2011-09-15T22:17:43Z", 250);
    // map<int, int> predecessor = n.dijsktra(p, 15000, 2, 4609, "2011-09-15T22:17:43Z");
    // p->writeToFile("test_images/test2.png");
    Network n(9);
    n.build_network_from_file("tests/test_edges.txt");
    n.store_coordintes_timestamps("tests/test_checkins.txt");

    PNG* p = n.plot_k_depth_friends(5, 2, "65", 200);
    map<int, int> predecessor = n.dijsktra(p, 5, 2, 2, "65");
    for (auto entry: predecessor) {
        cout << entry.first << " " << entry.second << endl;
    }
    p->writeToFile("test_images/test.png");

    

    return 0;
}