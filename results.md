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

We implemented Dijsktra's through using a priority queue instead of a regular queue, and by constructing the graph weights' map in a map<pair<int, int>, double> representing the source, destination, and the distance between the users. This allows us to achieve O(E + VlogV) time complexity and O(n) space complexity for map.

![WhatsApp Image 2022-12-10 at 18 13 27](https://user-images.githubusercontent.com/81874557/206880669-21cabeb0-d42b-4a93-b363-cbbdbdf85035.jpg)

In this is example, if we build the network from node 2 with depth 2, and choose the source to be 2 as well, then we would visualize the path from 2 to 9 since its the longest dijsktra's path.

## Visualization

We used the PNG and HSLAPixel classes from the CS 225 lib directories of the MPs to create, write to and load from PNG's.

While instantiating a new instance of our Network class, 4 private variables: min_x, max_x, min_y, and max_x are also created and updated which ultimately contain the maximum and minimum of the latitudes and longitudes passed in as inputs (x mapped to latitude and y mapped to longitude respectively).

These private variables facilitate the scaling of images to a given size such that the pixels are neither too spread out nor clustered.
To determine the adjusted coordinates to plot the HSLAPixels, we use a formula:

![image](https://user-images.githubusercontent.com/81874557/206881363-1dfdb53f-c2ec-4228-a95f-90df18e7c004.png)

Users within a connection network are colored in red, non-friends are colored in black.

#### Bresenham's line algorithm

We used the Bresenham's line algorithm to color pixels between 2 given points. It iteratively determines at every point, which points to color around the slope line of best fit, so that the line is visible.

The inputs to this algorithm are 2 coordinates, and a pointer to a PNG, and an HSLAPixel config representing a color. The algorithm colors all pixels in its path with that input pixel.

![image](https://user-images.githubusercontent.com/81874557/206881873-2a01a255-b8d6-4183-8971-740022a41d79.png)

This images contains the network visualized in green as rays extending out from the source to its friends (coordinates in red). The blue path visualized the resultant of dijkstra's algorithm which is the longest path from a given source in the network to any other point in the network.

A visualization formed as a result of this code snippet:

```
int data_size = 10000;
Network n(data_size);
n.build_network_from_file("data/Gowalla_edges.txt");
n.store_coordintes_timestamps("data/Gowalla_totalCheckins.txt");
PNG* p = n.plot_k_depth_friends(1500, 2, "2011-09-15T22:17:43Z", 250);
map<int, int> predecessor = n.dijsktra(p, 6798, 2, 4600, "2011-09-15T22:17:43Z");
p->writeToFile("test_images/test_actual.png");
```

![image](https://user-images.githubusercontent.com/81874557/206882039-9848704e-7dd5-48c6-b875-18fff04df632.png)







