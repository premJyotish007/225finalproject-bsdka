#include "network.h"

Network::Network(json locations_data, json checkins_data, json network_data) {
    cout << "Loading network..." << endl;
    load_network(network_data);
    cout << "Network loaded. Social network has " << map_network.size() << " users." << endl;
    cout << "Loading checkins..." << endl;
    load_checkins(checkins_data);
    cout << "Checkins loaded. Checkin data for " << map_checkins.size() << " distinct users." << endl;

}

void Network::load_network(json network_data) {
    std::ifstream ifs(network_data);
    json j = json::parse(ifs);
    map_network = j.get<map<int, vector<int>>>();
}

void Network::load_locations(json locations_json) {
    std::ifstream ifs(locations_json);
    json j = json::parse(ifs);
    map_coords = j.get<map<int, pair<double, double>>>();
}

void Network::load_checkins(json checkins_json) {
    std::ifstream ifs(checkins_json);
    json j = json::parse(ifs);
    map_checkins = j.get<map<int, vector<pair<string, pair<double, double>>>>>();
}


// Input: timestamp, user_id
// Output: closest checkin for given user_id before (or at) the given timestamp
// Output format: pair<string, pair<doulble, double>> where the string is the timestamp
//                and pair<double, double> is location at that point in time.
// Example: Input: user_id = 2, time_stamp = 1120
//          Output: Closest checkin entry for before or equal to 1120 at which user 2 has checked in.
pair<string, pair<double, double>> get_closest_timestamp(int user_id, string time_stamp) {
    // TODO
}



// The functions "build_network_from_file" and "store_coordinate_timestamps" are used to build the initial jsons from file inputs.
// The file inputs are the network data and checkin data.
// The jsons are stored as "locations.json" and "checkins.json" that map latitude-longitude pairs to location ids
// and users to their checkin timestamps respectively.

// These functions are called only when the graph is to be built on a database of a different user number.

Network::Network(int data_size) : data_size_(data_size) {}

void Network::build_network_from_file (string file_name) {
    ifstream infile (file_name);
    string line;
    while (std::getline(infile, line)) {
        vector<string> parts = delimit_string(line, "\t");
        int source = stoi(parts[0]);
        int dest = stoi(parts[1]);
        if (source > data_size_) break;
        if (source <= data_size_ && dest <= data_size_) {
            if (map_network.find(source) == map_network.end()) {
                map_network[source] = {};
            }
            if (map_network.find(dest) == map_network.end()) {
                map_network[dest] = {};
            }
            map_network[source].push_back(dest);
        }
    }
    auto output_json = json(map_network);
    std::ofstream ofile("network.json");
    ofile << output_json;

}


void Network::store_coordintes_timestamps(string file_name) {
    ifstream infile (file_name);
    string line;
    while (std::getline(infile, line)) {
        vector<string> parts = delimit_string(line, "\t");
        int user = stoi(parts[0]);
        if (user > data_size_) {
            break;
        }
        string time_stamp = parts[1];
        double latitude = stod(parts[2]);
        double longitude = stod(parts[3]);
        int location_id = stoi(parts[4]);

        // map_coords[location_id] = pair<double, double>{latitude, longitude};

        if (map_checkins.find(user) == map_checkins.end()) {
            map_checkins[user] = {};
        }
        map_checkins[user].push_back(pair<string, pair<double, double>>{time_stamp, pair<double, double>{latitude, longitude}});
    }

    auto locations_json = json(map_coords);
    auto checkins_json = json(map_checkins);
    std::ofstream file_locations("locations.json");
    std::ofstream file_checkins("checkins.json");
    file_locations << locations_json;
    file_checkins << checkins_json;
}


vector<string> Network::delimit_string(string s, string delimiter) {
    size_t pos = 0;
    string token;
    vector<string> to_return;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        to_return.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    to_return.push_back(s);
    return to_return;
}
