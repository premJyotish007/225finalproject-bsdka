#pragma once
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <queue>
#include "../includes/json_parser/json.hpp"
#include "../lib/cs225/PNG.hpp"
#include "../lib/cs225/HSLAPixel.hpp"

using json = nlohmann::json;
using namespace std;
using namespace cs225;

class Network {
    public:

        // Visualizing
            PNG* plot_k_depth_friends(int user_id, int k, string time_stamp, int image_size);
            pair<int, int> adjusted_coordinates(pair<double, double> input_coords, int image_size);

            void plot_coordinate(PNG* src_image, int x, int y, HSLAPixel& color);
            void plot_line(PNG* src_image, int x0, int y0, int x1, int y1);
        // Algorithms
            pair<string, pair<double, double>> get_closest_timestamp(int user_id, string time_stamp);
            vector<int> get_k_depth_connections(int user_id, int k);
            map<int, pair<double, double>> get_k_depth_locations(int user_id, int k, string time_stamp);
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
        HSLAPixel red = HSLAPixel(0, 1, 0.5);
        HSLAPixel black = HSLAPixel(0, 0, 0);
        HSLAPixel green = HSLAPixel(120, 1, 0.5);


        int data_size_ = 0;
        map<int, vector<int>> map_network;
        map<int, vector<pair<string, pair<double, double>>>> map_checkins;
        map<int, pair<double, double>> map_coords;
};
