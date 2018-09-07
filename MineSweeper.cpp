#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <queue>
#include <tuple>
#include <array>
using std::cout;
using std::endl;
using std::string;
using std::cin;
using std::flush;
using std::stoi;
using std::iota;
using std::to_string;
using std::random_device;
using std::mt19937;
using std::shuffle;
using std::make_tuple;
using std::tie;
using std::queue;
using std::tuple;
using std::vector;

constexpr int lines = 9;        // line
constexpr int number = 9;       // list
constexpr int mineCount = 9;    // mine counts
string block[lines + 1][number + 1]; // save pos
int mine[mineCount];            // mine pos，x + (y - 1) * 9
int state = 0;                  // state

void ready();
void prepare();
void display();
void play();
void flush();
bool isNoMine();
bool blockIsMine(int, int);
void update(int, int);

void ready() {
  cout << "*MineSweeper*" << endl;
  cout << "Size:9x9" << endl;
  cout << "Mine:9" << endl;
  cout << "GameRule:" << endl;
  cout << "Enter[(x) (y) \"q\"/\"w\"]to sweep mines" << endl;
  cout << "Example:1 2 q" << endl;
  cout << "----------" << endl;
  cout << "[n] -> n mines around here" << endl;
  cout << "[ ] -> No Explore" << endl;
  cout << "q -> Exploring" << endl;
  cout << "w -> Is Mine" << endl;
  cout << "♪ ->Mine" << endl;
  cout << "◎ ->It's your minefield" << endl;
  cout << endl;
  cout << "Ready to enter any chars to start the game." << endl;

  getchar();
  while (true) {
    system("clear");
    prepare();
    break;
  }
}

void prepare() {
  vector < int >v(81);
  iota(v.begin(), v.end(), 1);
  random_device rd;
  mt19937 g(rd());
  shuffle(v.begin(), v.end(), g);
  for (int i = 0; i < 10; i++) {
    mine[i] = v[i];
  }
  display();
}

void display() {
  system("clear");
  cout << "  ";
  for (int i = 1; i <= 9; i++) {
    cout << " " + to_string(i) + " " << flush;
  }
  cout << endl;
  for (int j = 1; j <= 9; j++) {
    cout << to_string(j) << " ";
    for (int k = 1; k <= 9; k++) {
      block[k][j] = "[ ]";
      cout << block[k][j];
    }
    cout << endl;
  }
  cout << "-----------------------------" << endl;
  play();
}

void flush() {
  system("clear");
  cout << "  ";
  for (int i = 1; i <= 9; i++) {
    cout << " " + to_string(i) + " " << flush;
  }
  cout << endl;
  for (int j = 1; j <= 9; j++) {
    cout << to_string(j) << " ";
    for (int k = 1; k <= 9; k++) {
      cout << block[k][j];
    }
    cout << endl;
  }
  cout << "-----------------------------" << endl;

  if (isNoMine()) {
    state = 2;
  }

  if (state == 1) {
    cout << "You Lose" << endl;
    cout << "Enter any chars to restart the game" << endl;
    getchar();
    while (true) {
      system("clear");
      state = 0;
      prepare();
      break;
    }
  }

  if (state == 2) {
    cout << "You Win" << endl;
    cout << "Enter any chars to restart the game" << endl;
    getchar();
    while (true) {
      system("clear");
      state = 0;
      prepare();
      break;
    }
  }
}

void play() {
  string input;
  int check1 = 0;
  int check2 = 0;
  string check3 = "";
  while (getline(cin, input)) {
    if (state == 1 || state == 2) {
      continue;
    }
    if (input.length() == 5) {
      check1 = stoi(input.substr(0), nullptr, 0);
      check2 = stoi(input.substr(2), nullptr, 0);
      check3 = input.substr(4);
      if (check1 >= 1 && check1 <= 9 && check2 >= 1 && check2 <= 9) {
        if (check3 == "q") {
          int x = stoi(input.substr(0), nullptr, 0);
          int y = stoi(input.substr(2), nullptr, 0);
          update(x, y);
          flush();
        } else if (check3 == "w") {
          int x, y = 0;
          x = stoi(input.substr(0), nullptr, 0);
          y = stoi(input.substr(2), nullptr, 0);
          if (block[x][y] == "[ ]") {
            block[x][y] = "[◎]";
          } else if (block[x][y] == "[◎]") {
            block[x][y] = "[ ]";
          }
          flush();
        } else {
          cout << "Illegality" << endl;
        }
      } else {
        cout << "Illegality" << endl;
      }
    } else {
      cout << "Illegality" << endl;
    }
  }
}

bool blockIsMine(int x, int y) {
for (int i:mine) {
    if (x + (y - 1) * 9 == i) {
      return true;
    }
  }
  return false;
}

bool isNoMine() {
for (int i:mine) {
    int x = (i - 1) % 9 + 1;
    int y = (i - 1) / 9 + 1;
    for (int i = 0; i <= 10; i++) {
    for(int j = 0;j <= 10;j ++){
      if (block[x][y] != "[◎]") {
        return false;
      }
      if (block[x][y] == "[◎]" && x + (y - 1) * 9 != i) {
        return false;
      }
    }
    }
  }
  return true;
}

void update(int tmpX, int tmpY) {
  queue < tuple < int, int >>q;
  q.push(make_tuple(tmpX, tmpY));
  bool visited[lines + 1][number + 1] = { false };
  int x = 0;
  int y = 0;
  while (!q.empty()) {
    tie(x, y) = q.front();
    q.pop();

    if (blockIsMine(x, y)) {
      block[x][y].assign("[♪]");
      state = 1;
    } else {
      int count = 0;
      for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
          if (i == 0 && j == 0) {
            continue;
          }

          int newX = x + i;
          int newY = y + j;
          if (newX < 1 || newX > 9 || newY < 1 || newY > 9) {
            continue;
          }

          if (blockIsMine(newX, newY)) {
            ++count;
          }
        }
      }

      block[x][y] = "[" + to_string(count) + "]";
      if (count == 0) {
        for (int i = -1; i < 2; ++i) {
          for (int j = -1; j < 2; ++j) {
            if (i == 0 && j == 0) {
              continue;
            }

            int newX = x + i;
            int newY = y + j;
            if (newX < 1 || newX > 9 || newY < 1 || newY > 9) {
              continue;
            }
            if (!visited[newX][newY]) {
              visited[newX][newY] = true;
              q.push(make_tuple(newX, newY));
            }
          }
        }
      }
    }
  }
}

int main() {
  ready();
}