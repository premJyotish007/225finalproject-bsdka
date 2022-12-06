#include "../includes/network.h"

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

void Network::load_checkins(json checkins_json) {
    std::ifstream ifs(checkins_json);
    json j = json::parse(ifs);
    map_checkins = j.get<map<int, vector<pair<string, pair<double, double>>>>>();
}

PNG* Network::plot_k_depth_friends(int user_id, int k, string time_stamp, int image_size) {
    
    PNG* p = new PNG(image_size, image_size);
    vector<pair<int, int>> vect_loc;
    cout << "plotting all users based on time..." << endl;
    pair<int, int> src_coordinates;
    for (auto& entry: map_checkins) {
        pair<double, double> current_loc = get_closest_timestamp(entry.first, time_stamp).second;
        pair<int, int> coords = adjusted_coordinates(current_loc, image_size);
        if (entry.first == user_id) src_coordinates = coords;
        plot_coordinate(p, coords.first, coords.second, black);
    }

    cout << "getting connection network..." << endl;
    map<int, pair<double, double>> friends = get_k_depth_locations(user_id, k, time_stamp);

    for (auto& entry : friends) {
        pair<int, int> coords = adjusted_coordinates(entry.second, image_size);
        vect_loc.push_back(coords);
        plot_coordinate(p, coords.first, coords.second, red);
    }

    for (pair<int, int> loc: vect_loc) {
        plot_line(p, src_coordinates.first, src_coordinates.second, loc.first, loc.second, green);
    }

    return p;
}

void Network::plot_coordinate(PNG* src_image, int x, int y, HSLAPixel& color) {
    for (int i = x - 1; i <= x + 1; i++) {
        if (i >= 0 &&  i < src_image->width()) {
            for (int j = y - 1; j <= y + 1; j++) {
                if (j >= 0 && j < src_image->height()) {
                    src_image->getPixel(i, j) = color;
                }
            }
        }
    }
}

void Network::plot_line(PNG* src_image, int x0, int y0, int x1, int y1, HSLAPixel& color) {
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int error = dx + dy;
    
    while (true) {
        src_image->getPixel(x0, y0) = color;
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

    int x = static_cast<int>((input_coords.first - min_x) * image_size / (max_x - min_x));
    int y = static_cast<int>((input_coords.second - min_y) * image_size / (max_y - min_y));
    return pair<int, int>{min(x, image_size - 1), min(y, image_size - 1)};

}


map<int, int> Network::dijsktra(PNG* src_image, int user_id, int k, int source, string time_stamp) {
    map<int, double> dist;
    map<pair<int, int>, double> weights;
    map<int, pair<double, double>> map_loc = get_k_depth_locations(user_id, k, time_stamp);
    vector<int> vertices;
    for (auto entry : map_loc) {
        vertices.push_back(entry.first);
        dist[entry.first] = numeric_limits<int>::max();    
    }

    // building edges with weights
    for (size_t i = 0; i < vertices.size(); i++) {
        int source = vertices[i];
        for (size_t j = i + 1; j < vertices.size(); j++) {
            int dest = vertices[j];
            vector<int> neighbors = map_network[source];
            if (find(neighbors.begin(), neighbors.end(), dest) != neighbors.end()) {
                if (source > dest) {
                    swap(source, dest);
                }
                weights[{source, dest}] = calculate_distance(map_loc[source], map_loc[dest]);
            }
        }
    }
    map<int, int> predecessor;

    // dijkstras implementation using priority queue
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    pq.push(make_pair(0, source));
    dist[source] = 0;
    while (!pq.empty()) {
        int current = pq.top().second;
        pq.pop();
        for (int neighbor: map_network[current]) {
            int source = current, dest = neighbor;
            if (source > dest) {
                swap(source, dest);
            }
            if (dist[current] + weights[{source, dest}] < dist[neighbor]) {
                dist[neighbor] = dist[current] + weights[{source, dest}];
                predecessor[neighbor] = current;
                pq.push(make_pair(dist[neighbor], neighbor));
            }
        }
    }
    int destination = source;
    double max_dest = 0;
    for (auto entry: dist) {
        if (entry.second > max_dest) {
            destination = entry.first;
            max_dest = entry.second;
        }
    }
    plot_coordinate(src_image, adjusted_coordinates(map_loc[source], src_image->width()).first,
                                adjusted_coordinates(map_loc[source], src_image->width()).second, purple);
    plot_coordinate(src_image, adjusted_coordinates(map_loc[destination], src_image->width()).first,
                                adjusted_coordinates(map_loc[destination], src_image->width()).second, purple);

    // evaluating longest dijkstra path from source through backtracking
    vector<int> path = {destination};
    int current = predecessor[destination];
    while (current != source) {
        path.push_back(current);
        int old_current = 
        current = predecessor[current];
    }
    path.push_back(source);
    for (size_t node = 0; node < path.size() - 1; node++) {
        pair<int, int> src_coord = adjusted_coordinates(map_loc[path[node]], src_image->width());
        pair<int, int> dest_coord = adjusted_coordinates(map_loc[path[node + 1]], src_image->width());
        plot_line(src_image, src_coord.first, src_coord.second, dest_coord.first, dest_coord.second, blue);

    }
    return predecessor;
}

// Returns locations for k-depth friends at a given timestamp
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
    vector<int> visited = {user_id};
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

// Calculates Eucledian distance between 2 points
double Network::calculate_distance(pair<double, double> p1, pair<double, double> p2) const {
    double x1 = p1.first, x2 = p2.first, y1 = p1.second, y2 = p2.second;
    return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
}

pair<string, pair<double, double>> Network::get_closest_timestamp(int user_id, string time_stamp) {
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
        string to_compare = timestamps[mid].first;
        for (size_t i = 0; i < (time_stamp.size() - to_compare.size()); i++) {
            to_compare = "0" + to_compare;
        }
        if (to_compare < time_stamp) {
            r = mid - 1;
        } else if (to_compare > time_stamp) {
            l = mid + 1;
        } else {
            desired_index = mid;
            break;
        }
    }
    desired_index = desired_index == -1 ? min(max(l, r), static_cast<int>(timestamps.size() - 1)) : desired_index; 
    return timestamps[desired_index];
}



// DO NOT EDIT AFTER THIS LINE

// The functions "build_network_from_file" and "store_coordinate_timestamps" are used to build the initial jsons from file inputs.
// The file inputs are the network data and checkin data.
// The jsons are stored as "locations.json" and "checkins.json" that map latitude-longitude pairs to location ids
// and users to their checkin timestamps respectively.

// These functions are called only when the graph is to be built on a database of a different user number.

// Constructor functions
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
            map_network[dest].push_back(source);

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
        min_x = min(latitude, min_x);
        max_x = max(latitude, max_x);
        min_y = min(longitude, min_y);
        max_y = max(longitude, max_y);

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

// Utility function to delimit string by a character
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