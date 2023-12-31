// Copyright 2023 Krishna Saharsh M mkrishna@bu.edu
// Copyright 2023 Yuqi Jin yuqijin8@bu.edu
// Copyright 2023 Abhishek Dhande abhi86@bu.edu

// HW 9

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

using std::cin;
using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::endl;

typedef int TileID;

class Tile {

    public:
    vector<string> tile;
    int size;
    TileID id;

    //Constructor
    Tile(int in_id, int in_size) {
        size = in_size;
        id = in_id;
        tile.resize(size);
        string input;
        for (int i = 0; i < size; i++) {
            cin >> input;
            tile.at(i) = input;
        }
        go_left_up();
    }

    bool is_valid() const {
        for (int row = 0; row < size; row++) {
            if(tile.at(row).size() != size)
                return false;
        }
        int weight = 0;
        for (int row = 0; row < size; row++) {
            for (int column = 0; column < size; column++) {
                char item = tile.at(row).at(column);
                if (item != '*' && item != '.')
                return false;
                if (item == '*') weight++;
            }
        }
        if (weight == 0) return false;
        return true;
    }

    void show() const {
        for (int row = 0; row < tile.size(); row++)
            cout << tile.at(row) << endl;
    }

    void go_left_up() {
        int n = tile.size();
        int up = n, left = n;

        for (int row = 0; row < tile.size(); row++) {
            size_t f = tile.at(row).find('*');
            if (f != string::npos) {
                if (row < up) up = row;
                if (f < left) left = f;
            }
        }
        if (left != 0 or up != 0) {
            for (int i = 0; i < tile.size(); i++) {
                for (int j = 0; j < tile.size(); j++) {
                    if (tile.at(i).at(j) == '*') {
                        tile.at(i - up).at(j - left) = '*';
                        tile.at(i).at(j) = '.';
                    }
                }
            }
        }
  }
  void resize() {
    int row = 0, column = 0;
    int n = tile.size();

    for (int i = 0; i < n; i++) {
        size_t f = tile.at(i).find_last_of('*');
        if (f != string::npos) {
            if (i >= row) row = i;
            if (f >= column) column = f;
        }
    }

    if (row == (n - 1) or column == (n - 1))
        return;
    int max;
    (row > column) ? max = row : max = column;
    int kill = n - 1 - max;

    for (int j = 0; j < kill; j++) {
        tile.pop_back();
        int current_size = tile.size();
        for (int k = 0; k < current_size; k++)
            tile.at(k).pop_back();
    }
  }


    void rotate(){
        vector<string> sec_tile;
        sec_tile.resize(tile.size());

        for (int i = 0; i < tile.size(); i++) {
            for (int j = 0; j < tile.size(); j++) {
                char tmp = tile.at(j).back();
                tile.at(j).pop_back();
                sec_tile.at(i) += tmp;
            }
        }

        tile = sec_tile;
        go_left_up();
        resize();
    }

    void flipud() {
        int n = tile.size();
        vector<string> sec_tile;
        sec_tile.resize(n);

        for (int i = 0; i < n; i++) {
            sec_tile.at(n - 1 - i) = tile.at(i);
        }

        tile = sec_tile;
        go_left_up();
        resize();
    }


    void fliplr(){
        int n = tile.size();

        for (int i = 0; i < n; i++)
            std::reverse(tile.at(i).begin(), tile.at(i).end());
        
        go_left_up();
        resize();
    }
};

class Blokus {
    public:
    vector<Tile> collection;
    vector<string> board;
    vector<vector<pair<int, int> > > played_tiles;

    Tile* find_tile(TileID find){

        for(int i = 0; i < collection.size(); i++) {
            if(collection.at(i).id == find) {
                return &collection.at(i);
            }
        }
        return NULL;
    }

    void create_piece() {
        int size;
        cin >> size;
        int id = 100 + collection.size();
        Tile t(id, size);

        if (t.is_valid()) {

            vector<string> newtile_dis;
            newtile_dis = t.tile;
            bool find = false;

            // find the first *, change it to #
            for (int i = 0; i < newtile_dis.size() && !find; i++) {
                for (int j = 0; j < newtile_dis.size() && !find; j++) {
                    if (newtile_dis.at(i).at(j) == '*') {
                        newtile_dis.at(i).at(j) = '#';
                        find = true;
                    }
                }
            }
            // for each # change its adjacent to # till the end
            bool end = false;
            while (!end) {
                end = true;

                for (int row = 0; row < newtile_dis.size(); row++) {
                    for (int column = 0; column < newtile_dis.size(); column++) {

                        if (newtile_dis.at(row).at(column) == '#') {
                            if (row - 1 >= 0
                                && newtile_dis.at(row - 1).at(column) == '*') {
                                newtile_dis.at(row - 1).at(column) = '#';
                                end = false;
                            }

                            if (column - 1 >= 0
                                && newtile_dis.at(row).at(column - 1) == '*') {
                                newtile_dis.at(row).at(column - 1) = '#';
                                end = false;
                            }

                            if (row + 1 < newtile_dis.size()
                                && newtile_dis.at(row + 1).at(column) == '*') {
                                newtile_dis.at(row + 1).at(column) = '#';
                                end = false;
                            }
                            
                            if (column + 1 < newtile_dis.size()
                                && newtile_dis.at(row).at(column + 1) == '*') {
                                newtile_dis.at(row).at(column + 1) = '#';
                                end = false;
                            }
                        }
                    }
                }
            }

            // if there has * left, discard it.
            for (int i = 0; i < newtile_dis.size(); i++) {
                for (int j = 0; j < newtile_dis.size(); j++) {
                    if (newtile_dis.at(i).at(j) == '*') {
                        cout << "disconnected tile discarded" << endl;
                        return;
                    }
                }
            }

            // duplicate error handling
            t.resize();
            Tile t_flip = t;
            t_flip.fliplr();

            // ROTATE t and flipped t for 4 times
            for (int i = 0; i < 4; i++) {
                int count = 0, count_flip = 0;
                t.rotate();
                t_flip.rotate();

                // COMPARE EACH TILE IN COLLECTION
                for (int i = 0; i < collection.size(); i++) {

                    // IF SIZE IS SAME
                    if (t.tile.size() == collection.at(i).tile.size()) {

                        // COMPARE EACH STRING INSIDE
                        for (int j = 0; j < t.tile.size(); j++) {
                            if (t.tile.at(j) == collection.at(i).tile.at(j))
                                count++;
                            if (t_flip.tile.at(j) == collection.at(i).tile.at(j))
                                count_flip++;
                        }
                        if (count == t.tile.size() or count_flip == t.tile.size()) {
                            cout << "duplicate of " <<
                                collection.at(i).id << " discarded" << '\n';
                            return;
                        }
                    }
                    count = 0;
                    count_flip = 0;
                }
            }
            collection.push_back(t);
            cout << "created tile " << t.id << endl;
        } 
        else {
            cout << "invalid tile" << endl;
        }
    }

    void reset() {
        cout << "game reset" << endl;
        board.clear();
        collection.clear();
        played_tiles.clear();
    }

    void show_tiles() const {
        cout << "tile inventory" << endl;
        for (auto tile = collection.rbegin(); tile != collection.rend(); ++tile) {
            cout << tile -> id << endl;
            tile -> show();
        }
    }

    void show_board() const {
        for(int i = 0; i < board.size(); i++) {
            cout << board.at(i) << "\n";
        }
    }

    void play_tile(TileID find, int x, int y) {

        int board_size = board.size();
        if (find_tile(find) != NULL) {
            if (x < 0 or y < 0 or x >= board_size or y >= board_size) {
                cout << find << " not played" << endl;
                return;
            }

            Tile t = *(find_tile(find));
            int tile_size = t.tile.size();
            vector<pair<int, int> > position;

            for (int i = tile_size - 1; i >= 0; i--) {
                for (int j = tile_size - 1; j >= 0; j--) {
                    if (t.tile.at(i).at(j) == '*') {
                        int a = x + i;
                        int b = y + j;

                        if (a >= board_size or b >= board_size) {
                            cout << find << " not played" << endl;
                            return;
                        } else if (board.at(a).at(b) != '.') {
                            cout << find << " not played" << endl;
                            return;
                        } else {
                            position.push_back(pair<int, int>(a, b));
                        }
                    }
                }
            }

            for (int i = 0; i < position.size(); i++) {
                board.at(position.at(i).first).at(position.at(i).second) = '*';
            }

            played_tiles.push_back(position);
            cout << "played " << find << endl;
        } 
        else {
            cout << find << " not played" << endl;
        }
    }

    void set_size(int n) {

        int m = board.size();

        if (m == 0) {
            board.resize(n);
            for (int i = 0; i < n; i++) {
                string temp(n, '.');
                board.at(i) = temp;
            }
        } 
        else if (n >= m) {
            for (int j = 0; j < m; j++) {
                string temp(n - m, '.');
                board.at(j) += temp;
            }

            for (int k = 0; k < n - m; k++) {
                string temp(n, '.');
                board.push_back(temp);
            }
        } 
        else {

            // resize_smaller here: n < m
            // find all the outside position
            // check if any outside positions matches played postitions
            // if matches, change that played position to '.'
            // resize the board smaller by 1

            for (int i = 0; i < m - n; i++) {
                vector<pair<int, int> > outside_position;

                for (int i = 0; i < board.size(); i++) {
                    outside_position.push_back(pair<int, int>((board.size() - 1), i));
                    outside_position.push_back(pair<int, int>(i, (board.size() - 1)));
                }

                for (int i = 0; i < played_tiles.size(); i++) {

                    for (int j = 0; j < played_tiles.at(i).size(); j++) {

                        for (int k = 0; k < outside_position.size(); k++) {

                            if (outside_position.at(k) == played_tiles.at(i).at(j)) {
                                for (int l = 0; l < played_tiles.at(i).size(); l++) {
                                    if (played_tiles.at(i).at(l).first < board.size()
                                        && played_tiles.at(i).at(l).second < board.size())
                                        board.at(played_tiles.at(i).at(l).first)
                                        .at(played_tiles.at(i).at(l).second) = '.';
                                }
                            }
                        }
                    }
                }
                board.pop_back();
                for (int i = 0; i < board.size(); i++)
                board.at(i).pop_back();
            }
        }
    }
};



//Given Code
int main() {

    string command;
    Blokus b;

    while (true) {
        cin >> command;

        if (command == "quit")  {
        break;
        } 
        else if (command == "//") {
            getline(cin, command);
        } 
        else if (command == "board") {
            b.show_board();
        } 
        else if (command == "create") {
            b.create_piece();
        } 
        else if (command == "reset") {
            b.reset();
        } 
        else if (command == "show") {
            string arg;
            cin >> arg;
            if (arg == "tiles") {
                b.show_tiles();
            } 
            else {
                auto g = b.find_tile(std::stoi(arg));
                g->show();
            }
        } 
        else if (command == "resize") {
            int newsize;
            cin >> newsize;
            b.set_size(newsize);
            b.show_board();
        } 
        else if (command == "play") {
            TileID id;
            int row, col;
            cin >> id >> row >> col;
            b.play_tile(id, row, col);
        } 
        else if (command == "rotate") {
            TileID id;
            cin >> id;
            auto g = b.find_tile(id);
            g->rotate();
            cout << "rotated " << id << "\n";
            g->show();
        } 
        else if (command == "fliplr") {
            TileID id;
            cin >> id;
            auto g = b.find_tile(id);
            g->fliplr();
            cout << "fliplr " << id << "\n";
            g->show();
        } 
        else if (command == "flipud") {
            TileID id;
            cin >> id;
            auto g = b.find_tile(id);
            g->flipud();
            cout << "flipud " << id << "\n";
            g->show();
        } 
        else {
            cout << "command not understood.\n";
        }
    }
    cout << "Goodbye\n";
    return 0;
}