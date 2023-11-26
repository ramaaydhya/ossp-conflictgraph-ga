#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <random>

#include "OSSP.cpp"

using namespace std;
typedef pair<int, int> ops_t;
typedef vector<vector<int>> int2D_t; 
typedef vector<pair<int, int>> opsArr_t;
typedef map<pair<int, int>, bool> usedArr_t;

class Schedule {
  private:
    opsArr_t operationOrder;

  public:
    Schedule(OSSP* ptr) : 
      OSSProblemPointer(ptr)
    {
      this->operationOrder = {};
      for (int i = 0 ; i < OSSProblemPointer->getNumOfJob() ; i++)
        for (int j = 0 ; j < OSSProblemPointer->getNumOfMachine() ; j++)
          this->operationOrder.push_back(make_pair(i, j));
    };

    int2D_t scheduleActive2(){
      int n = OSSProblemPointer->getNumOfJob();
      int m = OSSProblemPointer->getNumOfMachine();
      int nm = n * m;
      int CT[n][m];
      int ST[n][m] = {0};
      pair<int, int> minCTOperation = make_pair(0,0);
      int i0 = minCTOperation.first;
      int j0 = minCTOperation.second;
      opsArr_t t;
      copy((this->operationOrder).begin(), (this->operationOrder).end(), back_inserter(t));
      for(int i = 0 ; i < n ; i++)
        for(int j = 0 ; j < m ; j++){
          CT[i][j] = (OSSProblemPointer->getProcessingTime())[i][j]; 
          if (CT[i][j] < CT[i0][j0]){
             i0 = i;
             j0 = j;
          }
        }      
      while(!t.empty()){
        pair<int, int> firstConflict;
        int i = 0;
        for(i = 0 ; i < t.size() ; i++){
          if(ST[t[i].first][t[i].second] < CT[i0][j0] && OSSProblemPointer->isConflictOperation(t[i],minCTOperation)){
            firstConflict = t[i];
            break;
          }
        }

        t.erase(t.begin() + i);
        for (int i = 0; i < t.size(); i++){
          if (OSSProblemPointer->isConflictOperation(t[i], firstConflict))
            if (ST[t[i].first][t[i].second] < CT[firstConflict.first][firstConflict.second]){
              ST[t[i].first][t[i].second] = CT[firstConflict.first][firstConflict.second];
              CT[t[i].first][t[i].second] = ST[t[i].first][t[i].second] + 
                (OSSProblemPointer->getProcessingTime())[t[i].first][t[i].second];
              if (CT[t[i].first][t[i].second] < CT[i0][j0])
                minCTOperation = t[i];
            }
        }
      }
          


    };
    int2D_t scheduleActive1(){

    };

    int2D_t scheduleNonDelay(){
      int n = OSSProblemPointer->getNumOfJob();
      int m = OSSProblemPointer->getNumOfMachine();
      int CT[n][m];
      int ST[n][m] = {0};
      pair<int, int> minSTOperation = make_pair(0,0);
      int i0 = minSTOperation.first;
      int j0 = minSTOperation.second;
      opsArr_t t;
      copy((this->operationOrder).begin(), (this->operationOrder).end(), back_inserter(t));
      //in progress
      CT[i0][j0] = ST[i0][j0] + (OSSProblemPointer->getProcessingTime())[i0][j0];        
      while(!t.empty()){    
      }
    };


    static void X1(Schedule& parent1, Schedule& parent2, Schedule& child1, Schedule& child2){
      random_device rd;
      mt19937 gen(rd());
      int n_ops = parent1.OSSProblemPointer->getNumOfOperation();
      uniform_int_distribution<> distrib(1, n_ops - 1);
      int crossoverPoint = distrib(gen);

      opsArr_t cmsChild1 = child1.operationOrder;
      opsArr_t cmsChild2 = child2.operationOrder;
      opsArr_t cmsParent1 = parent1.operationOrder;
      opsArr_t cmsParent2 = parent2.operationOrder;

      cmsChild1.insert(
        cmsChild1.end(),
        cmsParent1.begin(),
        cmsParent1.begin() + crossoverPoint);

      cmsChild1.insert(
        cmsChild1.end(),
        cmsParent2.begin() + crossoverPoint,
        cmsParent2.end());

      cmsChild2.insert(
        cmsChild2.end(),
        cmsParent2.begin(),
        cmsParent2.begin() + crossoverPoint);

      cmsChild2.insert(
        cmsChild2.end(),
        cmsParent1.begin() + crossoverPoint,
        cmsParent1.end() );

    };
    static void OX(Schedule& parent1, Schedule& parent2, Schedule& child1, Schedule& child2){
      random_device rd;
      mt19937 gen(rd());
      int n_ops = parent1.OSSProblemPointer->getNumOfOperation();
      uniform_int_distribution<> distrib(1, n_ops - 1);
      int crossoverPoint1 = distrib(gen);
      int crossoverPoint2 = distrib(gen);
      while (crossoverPoint2 == crossoverPoint1) 
        crossoverPoint2 = distrib(gen);

      opsArr_t* cmsChild1 = &(child1.operationOrder);
      opsArr_t* cmsChild2 = &(child2.operationOrder);
      opsArr_t* cmsParent1 = &(parent1.operationOrder);
      opsArr_t* cmsParent2 = &(parent2.operationOrder);

    
      usedArr_t usedInChild1;
      usedArr_t usedInChild2;

      
      cmsChild1->assign(n_ops, make_pair(-1,-1));
      cmsChild2->assign(n_ops, make_pair(-1,-1));

      for(int i = crossoverPoint1 ; i < crossoverPoint2 ; i++){
        cmsChild1->at(i) = cmsParent1->at(i);
        usedInChild1[cmsParent1->at(i)] = true;
        cmsChild2->at(i) = cmsParent2->at(i);
        usedInChild2[cmsParent2->at(i)] = true;
      }

      int i = crossoverPoint2;
      for(int j = 0 ; j < cmsParent1->size(); j++){
        if(!usedInChild1[cmsParent1->at(j)]){
          cmsChild2->at(i++) = cmsParent1->at(j);
          if(i == cmsChild2->size()) i = 0;
        }
          
      }
      i = crossoverPoint2;
      for(int j = 0 ; j < cmsParent2->size(); j++){
        if(!usedInChild1[cmsParent2->at(j)]){
          cmsChild1->at(i++) = cmsParent2->at(j);
          if(i == cmsChild2->size()) i = 0;
        }
      }  
    };
    static void LOX(Schedule& parent1, Schedule& parent2, Schedule& child1, Schedule& child2){
      random_device rd;
      mt19937 gen(rd());
      int n_ops = parent1.OSSProblemPointer->getNumOfOperation();
      uniform_int_distribution<> distrib(1, n_ops - 1);
      int crossoverPoint1 = distrib(gen);
      int crossoverPoint2 = distrib(gen);
      while (crossoverPoint2 == crossoverPoint1) 
        crossoverPoint2 = distrib(gen);

      opsArr_t* cmsChild1 = &(child1.operationOrder);
      opsArr_t* cmsChild2 = &(child2.operationOrder);
      opsArr_t* cmsParent1 = &(parent1.operationOrder);
      opsArr_t* cmsParent2 = &(parent2.operationOrder);

    
      usedArr_t usedInChild1;
      usedArr_t usedInChild2;

      cmsChild1->assign(n_ops, make_pair(-1,-1));
      cmsChild2->assign(n_ops, make_pair(-1,-1));

      for(int i = crossoverPoint1 ; i < crossoverPoint2 ; i++){
        cmsChild1->at(i) = cmsParent1->at(i);
        usedInChild1[cmsParent1->at(i)] = true;
        cmsChild2->at(i) = cmsParent2->at(i);
        usedInChild2[cmsParent2->at(i)] = true;
      }

      int i = 0;
      for(int j = 0 ; j < cmsParent1->size(); j++){
        if(!usedInChild1[cmsParent1->at(j)]){
          cmsChild2->at(i++) = cmsParent1->at(j);
          if(i == crossoverPoint1) i = crossoverPoint2;
        }
          
      }
      i = 0;
      for(int j = 0 ; j < cmsParent2->size(); j++){
        if(!usedInChild1[cmsParent2->at(j)]){
          cmsChild1->at(i++) = cmsParent2->at(j);
          if(i == crossoverPoint1) i = crossoverPoint2;
        }
      }
    };

    void swapMutation(){
      random_device rd;
      mt19937 gen(rd());
      int n_ops = OSSProblemPointer->getNumOfOperation();
      uniform_int_distribution<> distrib(0, n_ops - 1);
      int index1 = distrib(gen);
      int index2 = distrib(gen);
      while(index1 == index2)
        int index2 = distrib(gen);
      pair<int, int> temp = (this->operationOrder)[index1];
      (this->operationOrder)[index1] = (this->operationOrder)[index2];
      (this->operationOrder)[index2] = temp; 
    };

    void moveMutation(){
      random_device rd;
      mt19937 gen(rd());
      int n_ops = OSSProblemPointer->getNumOfOperation();
      uniform_int_distribution<> distrib(0, n_ops - 1);
      int index_from = distrib(gen);
      int index_to = distrib(gen);
      while(index_from == index_to)
        int index_to = distrib(gen); 
      (this->operationOrder).insert(
        (this->operationOrder).begin() + index_to, 
        (this->operationOrder)[index_from]);
      if(index_from < index_to)
        (this->operationOrder).erase((this->operationOrder).begin() + index_from); 
      else
        (this->operationOrder).erase((this->operationOrder).begin() + (index_from + 1));
    };

    OSSP* OSSProblemPointer;    
};
