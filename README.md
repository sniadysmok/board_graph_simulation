# board_graph_simulation
a short program i wrote to demonstrate bfs and dfs on a 16:9 display for a presentation, its a mess :>
# COMPILATION
1. install raylib https://www.raylib.com/
2. compile with ```g++ bfs_dfs_sim.cpp -lraylib```
# Usage
```./your_binary [bfs/dfs] [your screen width] [your screen height]```
its a 16x9 maze, defined in the bfs_map.txt file, where a 1 indicates a wall, and a 0 an empty cell
# WARNING ! this will be fullscreen by default, if you dont want this, remove the line responsible for making the window fullscreen
