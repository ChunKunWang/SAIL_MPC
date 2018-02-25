// Author: Chun-Kun Wang
// E-mail: wckamos@gmaul.com
// Function: Secure bit Multiplication (Match-making)
#include <iostream>
#include <bitset>
#include <unordered_map>
#include <vector>
#define NUM 2

using namespace std;

void computeShares(bitset<NUM> &v, bool in) {
  if (in) {
    while (v.count() == 0) {
      for (int i = 0; i < NUM; i++)
        v[i] = rand() % 2;
    }
  }
}

void sendShares(unordered_map<int, unordered_map<int, vector<bool>>>& buffers, 
    unordered_map<int, bitset<NUM>>& map) {
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

// act as a Trusted Third Party
void fastCheck(bitset<NUM>& P) 
{
  if (P.count() == P.size())
    cout << "All Yes! : " << P << endl;
  else
    cout << "No Matched : " << P << endl;
}

int main()
{
  bitset<NUM> P;
  unordered_map<int, bitset<NUM>> map;
  srand(time(NULL));

  // read the input and generate shares
  for (int i = 0; i < NUM; i++) {
    int input = 1;
    cout << "Enter the bit of P[" << i << "]: ";
    cin >> input;

    if (input == 0) P[i] = false;
    else P[i] = true;

    computeShares(map[i], P[i]);
    cout << "* Shares: " << map[i] << endl;
  }
  cout << "=== Secret Out: " << P << " ===" << endl;
  fastCheck(P);

  cout << "\n=== Networking ===" << endl;
  unordered_map<int, unordered_map<int, vector<bool>>> buffers;
  sendShares(buffers, map);

  bool YResult = false;
  for (int i = 0; i < NUM; i++) {
    cout << "buffers[" << i << "]" << endl;
    for (int j = 0; j < NUM - 1; j++) {
      bool XResult = true;
      for (auto r : buffers[i][j]) {
        cout << r << " ";
        XResult = XResult & r;
      }
      YResult = YResult | XResult;
      cout << endl;
    }
  }

  cout << "=== Computed Out: ";
  if (YResult) cout << "All Yes!";
  else cout << "No Matched.";
  cout << " ===" << endl;
  
  return 0;
}

