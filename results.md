# Written Report for CS 225 Final Project Team-bsdka

## Problem Statement:

*Given a network of users and their app-checkin times and information, where were a user's connection upto a k-depth at a time closest to a given time?
Who are the farthest 2 connected users in a given sub-network?*

## Data collection and parsing:

We had 2 source data files:
1. Gowalla_edges.txt: This contains the network of users in a "node node" representation where if 2 nodes are on a line, they are directly connected.
2. Gowalla_checkins.txt: This contains the checkin information of the users in a "user_id checkin_timestamp latitude longitude location_id" format
which contained information about the checkin times and locations of every user.

We used ``` #include <ifstream> ``` to parse the data line-by-line, and a util function (implemented by us) called delimit_string that delimits a string by a given
character, which in our file format was tab spaced.

#### Challenges
Our dataset for checkins is very large (around 300MB) worth of data, therefore it was taking a long time to build the checkin map private variable of our network class.
As a result, we considered dumping the parsed map structure into a json file. Through this, we hypothesized that it would be efficient to construct an instance
of our Network class by reading from a json file passed as an argument.
For that we used the json.hpp file from https://github.com/nlohmann/json.
```
std::ifstream ifs(checkins_json);
json j = json::parse(ifs);
map_checkins = j.get<map<int, vector<pair<string, pair<double, double>>>>>();
```
However, we realized that caching works efficiently only when we're running our script on the full dataset or near about that. Loading times don't have as much of a
difference (or improvement) when sampling for a small number of users.
Therefore, we settled on sampling a small subsest of the entire dataset for testing and development process.

## Closest Timestamp:

Since our dataset is temporal as well as spatial, we had to take user checkin timestamp into consideration as well in order to obtain the exact location for a user
closest to a given timestamp.
To integrate timestamp relavance in our analysis, we implemented a binary search algorithm that compares timestamps as strings and returns the the closest timestamp
before or on any given timestamp.

![WhatsApp Image 2022-12-10 at 18 13 26](https://user-images.githubusercontent.com/81874557/206880473-eb8a616e-44f2-4cb4-90d4-74f7d78547c3.jpg)


## Breadth First Search
Our breadth first search algorithm was centered around finding all connections of users upto a k-depth. 0-depth means the user itself, 1-depth refers to immediate neighbors, 2-depth refers to neighbors of neighbors and so on. This was used to define and create a network of users for a particular user-id and confine the running of the timestamp algorithm to within k-depth connections to plot their locations.

![WhatsApp Image 2022-12-10 at 18 13 27](https://user-images.githubusercontent.com/81874557/206880485-12430a8e-d9a2-45e3-9f38-7c25f6b77928.jpg)

## Dijkstra's (modified)
Our implementation of dijkstra's algorithm is a modified version of a traditional application of finding a route from one node to another in a given network. Our aim is to feed a user_id (k-depth and timestamp) to build a network from, a source node (starting point) within the network to evaluate the farthest point from the source.
The assigned weights of the graph are the eucledian distances between latitude and longitudinal coordinates.
The latitude and longitudes are calculated from the get_closest_timestamp by obtaining the location of a user closest to an input timestamp.

![WhatsApp Image 2022-12-10 at 18 13 27](https://user-images.githubusercontent.com/81874557/206880669-21cabeb0-d42b-4a93-b363-cbbdbdf85035.jpg)

In this is example, if we build the network from node 2 with depth 2, and choose the source to be 2 as well, then we would visualize the path from 2 to 9 since its the longest dijsktra's path.




