## Leading Question 

Given a network of users on Gowalla, and a selected user at a given point in time, what is the closest historical location of all users on the network (until that given point in time), and where were my k-th connection friends?
Just by clicking through other users in the display as connections to build routes, who is my closest kth connection?

K-th connection refers to friends who are connected at a k-step distance between nodes i and j.





## Dataset Acquisition and Processing
We acquired our dataset from http://snap.stanford.edu/data/loc-Gowalla.html
It gives us the data in .txt format, which we will load into a csv and then parse.
The two text files, we obtain our data in contain:
1. Network of friends (node-node representation), where one entry i, j represents that user i is a direct connection of user j.
2. Check-in data containing the user, the time-stamp, and the location of their check-in (latitude and longitude). Every latitude and longitude is mapped to a location id as well, which we will store in a map for easy reference.

Data correction: If a node i doesn’t map to a node j, I would entirely remove that entry from the graph data. Similarly if a latitude or longitude is invalid, I would also neglect that datapoint given that there are many check-ins that a user inputs.

Data storage:
One hashmap with keys as nodes, and values as their immediate (1st) connections stored as an adjacency list.
One hashmap containing keys as location id and latitude and longitude pairs as the values, for easier location referencing.



## Algorithm 
Traversal: BFS to make a level-based distinction between nodes and identify kth connections

Covered Algorithm: Dijkstra’s algorithm where the comparator input to the priority queue implementation would be the euclidean distance between the current and neighboring nodes’ location ids.

Uncovered: Visualizing the points to be displayed using a graph representation, on a PNG by coloring in the appropriate pixels to fill in the edges and connect the nodes. Coloring the desired points and the routes representing that connected component using red as opposed to black. Displaying the route to the closest kth neighbor (evaluated through dijkstra’s) in green.



## Timeline


11/11/2022: Fetch all the data from the friendship dataset and create classes and function outline

11/18/2022: Implement map to filtering mechanism to get the closest geographical points for every user’s historical check in.

11/25/2022: Implement the BFS to filter out points within k-depth. Figure out a mechanism to display points and color the graph black.

12/2/2022: Implement dijkstras to find the route to shortest kth connection and apply color schema of red for k connection network and green for the evaluated route.

12/08/2022: Compile the final code and create a video presentation to explain the algorithm, progress, and contribution.
