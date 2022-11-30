
#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "../includes/network.h"


TEST_CASE("Test build_network_from_file", "[data cleaning, parsing]") {
    Network n(9);
    n.build_network_from_file("tests/test_edges.txt");
    map<int, vector<int, int>> network = n.get_network();
    REQUIRE(network[1] == vector<int>{2,9});
    REQUIRE(network[2] == vector<int>{1,3,4});
    REQUIRE(network[3] == vector<int>{2,4});
    REQUIRE(network[4] == vector<int>{2,3,5});
    REQUIRE(network[5] == vector<int>{4,6,7,8,9});
    REQUIRE(network[6] == vector<int>{5});
    REQUIRE(network[7] == vector<int>{5});
    REQUIRE(network[8] == vector<int>{5});
    REQUIRE(network[9] == vector<int>{1,5});

}

TEST_CASE("Test k-depth neighbors (BFS) start-5", "[algorithms]") {
    
    Network n(9);
    n.build_network_from_file("tests/test_edges.txt");
    vector<int> neighbors = n.get_k_depth_connections(5, 1);
    vector<int> expected = {4,6,7,8,9};
    REQUIRE(neighbors == expected);

    neighbors = n.get_k_depth_connections(5, 2);
    expected = {4,6,7,8,9,2,3,1};
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