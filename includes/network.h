#pragma once
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include "../includes/json_parser/json.hpp"

using json = nlohmann::json;

using namespace std;

class Network {
    public:
        // Initializing graph from json
            Network(json checkins_data, json network_data);
            void load_checkins(json checkins_json);
            void load_network(json network_data);
        
        
        // Initializing json
            Network(int data_size);

            // Constructing edges and vertices for the map
            void build_network_from_file (string file_name);

            // Storing a map of coordinates and timestamps for every given user from a file.
            void store_coordintes_timestamps(string file_name);

            // Utilities to delimit/parse string and converting map to a json file
            vector<string> delimit_string(string s, string delimiter);

    private:
        int data_size_ = 0;
        map<int, vector<int>> map_network;
        map<int, vector<pair<string, pair<double, double>>>> map_checkins;
        map<int, pair<double, double>> map_coords;
};