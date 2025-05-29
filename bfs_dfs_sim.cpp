#include <bits/stdc++.h>
#include "raylib.h"
int w_rect, h_rect,w_arg,h_arg;
int **board;
int **visited;
std::pair<int,int> **parent;
int scale = 1;
int b_x,b_y,e_x,e_y; bool picked;
constexpr int INF = 1e9 + 7;
Color color_array[]{
    WHITE,BLACK,RED,GREEN,BLUE,PURPLE,ORANGE
};
void draw_board(int W, int H){
    W = W/w_rect;
    H = H/h_rect;
    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            DrawRectangle(j*w_rect, i*h_rect,w_rect, h_rect, color_array[board[i][j]]);
            DrawRectangleLines(j*w_rect, i*h_rect,w_rect, h_rect, color_array[1]);
        }
    }
}
void draw_updated(int x, int y){
    BeginDrawing();
    if(WindowShouldClose()){
        return ;
    }
    DrawRectangle(x*w_rect, y*h_rect,w_rect,h_rect,color_array[board[y][x]]);
    DrawRectangleLines(x*w_rect, y*h_rect,w_rect, h_rect, color_array[1]);
    EndDrawing();
}
bool is_empty(int x, int y){
    return (board[y][x] == 0);
}
bool is_in_bounds(int x,int y){
    return (x >= 0 && y >= 0 && x < w_arg/w_rect && y < h_arg/h_rect);
}
std::array<std::pair<int,int>,4> msk_arr {std::make_pair(-1,0), std::make_pair(1,0), std::make_pair(0,-1), std::make_pair(0,1)};


int cnt = 0;
void dfs(int i, int j, int udel){
    // i - y, j - x
    if(WindowShouldClose()){
        return ;
    }
    visited[i][j] = 1; cnt++;
    if(is_empty(j,i)){
        board[i][j] = 5;
        draw_updated(j,i);
    }
    std::cerr<<i<<" "<<j<<std::endl;
    usleep(udel);

    for(auto a : msk_arr){
        int y = i + a.first; int x = j + a.second;
       // draw_updated(j,i);
        if(is_in_bounds(x,y)){
            if(!visited[y][x] && (is_empty(x,y) || board[y][x] == 2)){
                parent[y][x] = std::make_pair(i,j);
                //std::cerr<<a.first<<" "<<a.second<<std::endl;
                dfs(y,x,udel);
            }
        }
    }

}
void bfs(int i, int j, int udel){
    if(WindowShouldClose()){
        return ;
    }
    std::queue<std::pair<int,int>> Q;
    visited[i][j] = 1; cnt++;
    visited[i][j] = 0;
    parent[i][j] = {i,j};
    Q.push({i,j});

    while(!Q.empty()){
        if(WindowShouldClose()){
            return ;
        }
        int x = Q.front().second;
        int y = Q.front().first;
        Q.pop();
        if(is_empty(x,y)){
            board[y][x] = 5;
            draw_updated(x,y);
        }
        usleep(udel);
        for(auto a : msk_arr){
            int xi,yi; xi = x + a.second; yi = y + a.first;
            if(is_in_bounds(xi,yi)){

                if((is_empty(xi,yi) || board[yi][xi] == 2) && visited[yi][xi] > visited[y][x] + 1){
                    parent[yi][xi].first = y;
                    parent[yi][xi].second = x;
                   // std::cout<<"PARENT : "<<y<<" "<<x<<" "<<yi<<" "<<xi<<std::endl;
                    visited[yi][xi] = visited[y][x] + 1;
                    Q.push({yi,xi});
                }
            }
        }
    }
}
void clear_arr(){
    for(int i = 0; i < 9 * scale; ++i){
        for(int j = 0; j < 16 * scale; ++j){
            board[i][j] = 0;
            visited[i][j] = 0;
            parent[i][j] = {0,0};
        }
    }
}
int main(int argc, char **argv){
    if(argc != 4){
        if(strcmp(argv[1],"help") == 0){
            std::cout<<"1 opcja - dfs/bfs, 2 opcja - szerokosc ekranu, 3 - wysokosc ekranu\n";
            exit(0);
        }
        std::cout<<"Podaj DOKŁADNIE 3 argumenty\n";
        exit(1);
    }
    //shuffle(msk_arr.begin(),msk_arr.end(),std::mt19937(time(NULL)));
    w_arg = std::stoi(argv[2]);
    h_arg = std::stoi(argv[3]);
    w_rect = w_arg / (16 * scale);
    h_rect = h_arg / (9 * scale);
    std::cerr<<w_rect<<" "<<h_rect<<std::endl;
    //SetConfigFlags(FLAG_FULLSCREEN_MODE);
  //  SetConfigFlags(FLAG_FULLSCREEN_MODE);
    SetConfigFlags(FLAG_WINDOW_TOPMOST | FLAG_WINDOW_UNDECORATED);
    InitWindow(w_arg, h_arg, "graph algorithms on a board :)");
    SetTargetFPS(60);
    board = (int**)malloc(sizeof(int*) * 9 * scale);
    visited = (int**)malloc(sizeof(int*) * 9 * scale);
    parent = (std::pair<int,int> **)malloc(sizeof(std::pair<int,int> *) * 9 * scale);
    for(int i = 0; i < 9*scale; ++i){
        board[i] = (int*)malloc(sizeof(int) * 16 * scale);
        visited[i] = (int*)malloc(sizeof(int) * 16 * scale);
        parent[i] = (std::pair<int,int> *)malloc(sizeof(std::pair<int,int>) * 16 * scale);
    }
    clear_arr();
    if(strcmp(argv[1],"bfs") == 0){
        for(int i = 0; i < 9 * scale; ++i){
            for(int j = 0; j < 16 * scale; ++j){
                visited[i][j] = INF;
            }
        }
    }
    std::ifstream mp("bfs_map.txt");
        for(int i = 0; i < 9 * scale; ++i){
            std::string s; mp >> s;
            for(int j = 0; j < 16 * scale; ++j){
                if(s[j] == '1'){
                    board[i][j] = 4;
                }
            }
        }
    mp.close();
    bool DONE = 0;
    b_x = -1; b_y = -1; e_x = -1; e_y = -1;
    while(!WindowShouldClose()){

        ClearBackground(RAYWHITE);
        if(picked){
            BeginDrawing();
            draw_board(w_arg,h_arg);
            EndDrawing();
            if(!DONE){
                draw_board(w_arg,h_arg);
                if(strcmp(argv[1],"bfs") == 0){
                    bfs(b_y,b_x,3e5);

                }else{
                    dfs(b_y,b_x,3e5);
                }
                DONE = 1;
                int s_x,s_y;
                s_x = e_x;
                s_y = e_y;
                std::cout<<s_x<<" "<<s_y<<'\n';
                while(parent[s_y][s_x] != std::make_pair(b_y,b_x)){
                    std::pair<int,int> temp = parent[s_y][s_x];
                    s_x = temp.second; s_y = temp.first;
                    board[s_y][s_x] = 6;
                    std::cout<<s_x<<" "<<s_y<<'\n';
                }
                std::cout<<"done\n";
            }else{
                BeginDrawing();
                draw_board(w_arg,h_arg);
                EndDrawing();
               // std::cerr<<"cn : "<<cnt<<'\n';
            }

        }else{
            BeginDrawing();
            draw_board(w_arg,h_arg);
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                if(b_x < 0 && b_y < 0){

                    b_x = GetMousePosition().x / (w_rect);
                    b_y = GetMousePosition().y / (h_rect);
                    if(is_empty(b_x,b_y)){
                        board[b_y][b_x] = 3;
                    }else{
                        b_x = -1; b_y = -1;
                    }

                }else{
                    e_x = GetMousePosition().x / (w_rect);
                    e_y = GetMousePosition().y / (h_rect);
                    if(is_empty(e_x,e_y)){
                        board[e_y][e_x] = 2;
                        picked = 1;
                    }else{
                        e_x = -1; e_y = -1;
                    }
                }
            }
            EndDrawing();
        }

    }
    CloseWindow();
    for(int i = 0; i < 9*scale; ++i){
        free(board[i]);
        free(visited[i]);
        free(parent[i]);
    }
    free(board);
    free(visited);
    free(parent);

    std::cerr<<w_arg<<" "<<h_arg<<'\n';
}
