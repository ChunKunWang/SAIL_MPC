// Author: Chun-Kun Wang
// E-mail: wckamos@gmaul.com
// Function: Secure addition using Secret Sharing
#include <iostream>
#include <unordered_map>
#include <vector>
#define PRIME 199
#define NUM 3

using namespace std;

vector<int> S(NUM, 0);

void computeShares(vector<int> &v, int num, int input) {
  int sum = 0;
  for (int i = 0; i < num - 1; i++) {
    int x = rand() % PRIME;
    sum += x;
    S[i] += x;
    v.push_back(x);
  }
  int lastx = (input - sum) > 0 ? (input - sum) % PRIME : (input - sum + 2*PRIME) % PRIME;
  S[NUM-1] += lastx;
  v.push_back(lastx);
}

void sendShares(unordered_map<int, unordered_map<int, vector<int>>>& buffers, 
    unordered_map<int, vector<int>>& map) {
  for (int i = 0; i < NUM; i++) {
    //cout << "buffers[" << i << "]" << endl;
    for (int k = 0; k < NUM; k++) {
      int index = 0;
      for (int j = 0; j < NUM; j++) {
        if (i == j) continue;
        buffers[i][index++].push_back(map[k][j]);
        //cout << map[k][j] << " ";
      }
    }
    //cout << endl;
  }
}

void fastCheck() 
{
  int re_secret = 0;
  for (auto s : S) {
    re_secret += (s % PRIME);
    cout << s << " ";
  }
  cout << "=> " << re_secret % PRIME << endl;
}

int main()
{
  vector<int> P(NUM, 1);
  unordered_map<int, vector<int>> map;
  srand(time(NULL));

  cout << "The secret number range is [0," << PRIME << "]." << endl;
  for (int i = 0; i < NUM; i++) {
    int input = 1;
    cout << "Enter the secret of P[" << i << "]: ";
    cin >> input;
    P[i] = input;
    computeShares(map[i], NUM, input);
    cout << "* Shares: ";
    for (auto p : map[i])
      cout << p << " ";
    cout << endl;
  }

  int secret = 0;
  for (auto s : P)
    secret += s;
  cout << "=== Secret Sum: " << secret % PRIME << " ===" << endl;

  cout << "\n=== Networking ===" << endl;
  unordered_map<int, unordered_map<int, vector<int>>> buffers;
  sendShares(buffers, map);

  int total_sum = 0;
  for (int i = 0; i < NUM; i++) {
    cout << "buffers[" << i << "]" << endl;
    for (int j = 0; j < NUM - 1; j++) {
      int s_sum = 0;
      for (auto r : buffers[i][j]) {
        cout << r << " ";
        s_sum += r;
      }
      total_sum += (s_sum % PRIME);
      cout << " : " << (int) s_sum % PRIME << endl;
    }
  }
  cout << "=== Computed Sum: " << (total_sum / 2) % PRIME << " ===" << endl;
  //fastCheck();

  return 0;
}

