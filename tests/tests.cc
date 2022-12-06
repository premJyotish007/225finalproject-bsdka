
#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "../includes/network.h"


TEST_CASE("Test build_network_from_file", "[data cleaning, parsing]") {
    Network n(9);
    n.build_network_from_file("tests/test_edges.txt");
    map<int, vector<int>> network = n.get_network();
    vector<vector<int>> adj_lists = {{2,9}, {1,3,4}, {2,4}, {2,3,5}, {4,6,7,8,9}, {5}, {5}, {5}, {1,5}};
    for (int i = 1; i < 10; i++) {
        REQUIRE(network[i] == adj_lists[i - 1]);
    }

}

TEST_CASE("Test k-depth neighbors (BFS) start-5", "[algorithms]") {
    
    Network n(9);
    n.build_network_from_file("tests/test_edges.txt");
    vector<int> neighbors = n.get_k_depth_connections(5, 1);
    vector<int> expected = {5,4,6,7,8,9};
    REQUIRE(neighbors == expected);

    neighbors = n.get_k_depth_connections(5, 2);
    expected = {5,4,6,7,8,9,2,3,1};
    REQUIRE(neighbors == expected);

    
}


TEST_CASE("Test get_closest_time_stamp (Binary Search)", "[algorithms]") {
    Network n(9);
    n.store_coordintes_timestamps("tests/test_checkins.txt");
    pair<string, pair<double, double>> timestamp = n.get_closest_timestamp(2, "87");
    string expected_timestamp = "85";
    REQUIRE(timestamp.first == expected_timestamp);

    timestamp = n.get_closest_timestamp(1, "87");
    expected_timestamp = "76";
    REQUIRE(timestamp.first == expected_timestamp);


    // exact match
    timestamp = n.get_closest_timestamp(7, "81");
    expected_timestamp = "81";
    REQUIRE(timestamp.first == expected_timestamp);

}

TEST_CASE("Test dijkstras", "[algorithms]") {
    Network n(9);
    n.build_network_from_file("tests/test_edges.txt");
    n.store_coordintes_timestamps("tests/test_checkins.txt");
    PNG* p = n.plot_k_depth_friends(5, 2, "65", 200);

    // source 4
    map<int, int> predecessor = n.dijsktra(p, 5, 2, 4, "65");
    map<int, int> expected = {{1, 2}, {2, 4}, {3, 4}, {5, 4}, {6, 5}, {7, 5}, {8, 5}, {9, 1}};
    REQUIRE(expected.size() == predecessor.size());
    for (auto entry: expected) {
        REQUIRE(expected[entry.first] == predecessor[entry.first]);
    }

    // source 2
    predecessor = n.dijsktra(p, 5, 2, 2, "65");
    map<int, int> expected2 = {{1, 2}, {3, 2}, {4, 2}, {5, 9}, {6, 5}, {7, 5}, {8, 5}, {9, 1}};
    REQUIRE(expected.size() == predecessor.size());
    for (auto entry: expected2) {
        REQUIRE(expected2[entry.first] == predecessor[entry.first]);
    }


}