#include "network.h"

Network::Network(json checkins_data, json network_data) {
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


PNG* Network::plot_k_depth_friends(int user_id, int k, string time_stamp, int image_size) {
    
    PNG* p = new PNG(image_size, image_size);
    vector<pair<int, int>> vect_loc;
    cout << "plotting all users based on time..." << endl;
    for (auto& entry: map_checkins) {
        pair<double, double> current_loc = get_closest_timestamp(entry.first, time_stamp).second;
        pair<int, int> coords = adjusted_coordinates(current_loc, image_size);
        plot_coordinate(p, coords.first, coords.second, black);
    }

    cout << "getting connection network..." << endl;
    map<int, pair<double, double>> friends = get_k_depth_locations(user_id, k, time_stamp);

    for (auto& entry : friends) {
        pair<int, int> coords = adjusted_coordinates(entry.second, image_size);
        vect_loc.push_back(coords);
        plot_coordinate(p, coords.first, coords.second, red);
    }
    
    cout << "plotting connections and graph" << endl;
    for (size_t i = 0; i < vect_loc.size(); i++) {
        for (size_t j = i + 1; j < vect_loc.size(); j++) {
            plot_line(p, vect_loc[i].first, vect_loc[i].second, vect_loc[j].first, vect_loc[j].second);
        }
    }

    return p;
}

void Network::plot_coordinate(PNG* src_image, int x, int y, HSLAPixel& color) {
    for (int i = x - 1; i <= x + 1; i++) {
        if (x >= 0 &&  x < src_image->width()) {
            for (int j = y - 1; j <= y + 1; j++) {
                if (y >= 0 && y < src_image->height()) {
                    src_image->getPixel(i, j) = color;
                }
            }
        }
    }
}

void Network::plot_line(PNG* src_image, int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int error = dx + dy;
    
    while (true) {
        src_image->getPixel(x0, y0) = green;
        if ((x0 == x1) && (y0 == y1)) break;
        int e2 = 2 * error;
        if (e2 >= dy) {
            if (x0 == x1) break;
            error = error + dy;
            x0 = x0 + sx;
        }
        if (e2 <= dx) {
            if (y0 == y1) break;
            error = error + dx;
            y0 = y0 + sy;
        }
    }
}

pair<int, int> Network::adjusted_coordinates(pair<double, double> input_coords, int image_size) {
    int x = static_cast<int>((input_coords.first + 64) * image_size / (64 + 66));
    int y = static_cast<int>((input_coords.second + 177) * image_size / (177 + 178));
    return pair<int, int>{x, y};

}

void Network::load_checkins(json checkins_json) {
    std::ifstream ifs(checkins_json);
    json j = json::parse(ifs);
    map_checkins = j.get<map<int, vector<pair<string, pair<double, double>>>>>();
}

map<int, pair<double, double>> Network::get_k_depth_locations(int user_id, int k, string time_stamp) {
    map<int, pair<double, double>> to_return;
    vector<int> connections = get_k_depth_connections(user_id, k);
    for (int c : connections) {
        pair<string, pair<double, double>> closest = get_closest_timestamp(c, time_stamp);
        to_return[c] = closest.second;
    }
    return to_return;
}

vector<int> Network::get_k_depth_connections(int user_id, int k) {
    // Implemented BFS to find all connections till k-depth.

    queue<int> q;
    q.push(user_id);
    vector<int> visited;
    vector<int> to_return;
    map<int, int> distance_map;
    distance_map[user_id] = 0;
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        if (distance_map[current] > k) {
            continue;
        }
        to_return.push_back(current);
        vector<int> neighbors = map_network[current];
        for (int neighbor : neighbors) {
            if (find(visited.begin(), visited.end(), neighbor) == visited.end()) {
                q.push(neighbor);
                visited.push_back(neighbor);
                distance_map[neighbor] = distance_map[current] + 1;
            }
        }
    }
    return to_return;
}

// Input: timestamp, user_id
// Output: closest checkin for given user_id before (or at) the given timestamp
// Output format: pair<string, pair<doulble, double>> where the string is the timestamp
//                and pair<double, double> is location at that point in time.
// Example: Input: user_id = 2, time_stamp = 1120
//          Output: Closest checkin entry for before or equal to 1120 at which user 2 has checked in.
pair<string, pair<double, double>> Network::get_closest_timestamp(int user_id, string time_stamp) {
    // TODO

    // Implementing binary search for timestamp string
    if (map_checkins.find(user_id) == map_checkins.end()) {
        return pair<string, pair<double, double>>{"", pair<double, double>(0.0, 0.0)};
    }
    vector<pair<string, pair<double, double>>> timestamps = map_checkins[user_id];

    int l = 0;
    int r = static_cast<int>(timestamps.size()) - 1;
    int desired_index = -1;
    while (l <= r) {
        int mid = (l + r) / 2;
        if (timestamps[mid].first < time_stamp) {
            r = mid - 1;
        } else if (timestamps[mid].first > time_stamp) {
            l = mid + 1;
        } else {
            desired_index = mid;
            break;
        }
    }

    desired_index = desired_index = -1 ? min(max(l, r), static_cast<int>(timestamps.size() - 1)) : desired_index; 
    return timestamps[desired_index];
}



// DO NOT EDIT AFTER THIS LINE

// The functions "build_network_from_file" and "store_coordinate_timestamps" are used to build the initial jsons from file inputs.
// The file inputs are the network data and checkin data.
// The jsons are stored as "locations.json" and "checkins.json" that map latitude-longitude pairs to location ids
// and users to their checkin timestamps respectively.

// These functions are called only when the graph is to be built on a database of a different user number.

Network::Network(int data_size) : data_size_(data_size) {}

void Network::build_network_from_file (string file_name) {
    cout << "building network from file..." << endl;


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
    // auto output_json = json(map_network);
    // std::ofstream ofile("network.json");
    // ofile << output_json;
    cout << "done." << endl;

}


void Network::store_coordintes_timestamps(string file_name) {
    cout << "building checkins map from file..." << endl;
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

    // auto checkins_json = json(map_checkins);
    // std::ofstream file_checkins("checkins.json");
    // file_checkins << checkins_json.dump(2);
    cout << "done" << endl;
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