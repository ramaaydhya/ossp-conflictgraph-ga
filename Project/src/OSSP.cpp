#include <iostream>
#include <bits/stdc++.h>
#include <vector>

typedef pair<int, int> ops_t;
typedef vector<vector<int>> int2D_t;
using namespace std;

class Schedule;

class OSSP
{
protected:
  static int n_job;
  static int n_machine;
  static int** processing_time; //adjacency matrix
  static int** conflict_graph; //adjacency list
  static int n_operation;
  
public:
  OSSP(int n, int m, int** P, int** G){
    n_job = n;
    n_machine = m;
    n_operation = n * m;
    processing_time = new int*[n_job];
    conflict_graph = new int*[n_job];
    for (int i = 0; i < n_job; i++) {
      processing_time[i] = new int[n_machine];
      conflict_graph[i] = new int[n_job];
      for (int j = 0; j < n_job ; j++)
        conflict_graph[i][j] = G[i][j];
      for (int j = 0; j < n_machine ; j++)
        processing_time[i][j] = P[i][j];
    }
  };

  int getNumOfJob() {return n_job;}
  int getNumOfMachine() {return n_machine;}
  int getNumOfOperation() {return n_operation;}
  int** getProcessingTime() {return processing_time;}

  bool isConflictJob(int i, int j){
    return conflict_graph[i][j];
  }
  bool isConflictOperation(ops_t i, ops_t j){
    return 
      i.first == j.first || 
      i.second == j.second || 
      conflict_graph[i.first][j.first];
  }

  ~OSSP();
  
};
