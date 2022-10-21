/*
 * The branch perdiction module with the following strategies
 *   Always Taken
 *   Always Not Taken
 *   Backward Taken, Forward Not Taken
 *   Branch Prediction Buffer with 2bit history information
 * 
 * Created by He, Hao on 2019-3-25
 */

#ifndef BRANCH_PREDICTOR_H
#define BRANCH_PREDICTOR_H

#include <cstdint>
#include <string>
#include <vector>

const int PRED_BUF_SIZE = 4096;

class BranchPredictor {
public:
  enum Strategy {
    AT, // Always Taken
    NT, // Always Not Taken
    BTFNT, // Backward Taken, Forward Not Taken
    BPB, // Branch Prediction Buffer with 2bit history information
    PERCEPTRON // Branch Prediction with perceptrons
  } strategy;

  struct Perceptron{
    Perceptron(){
      this->perceTable.resize(0);
    }
    // build perceptron with the given parameters
    Perceptron(uint32_t numWeight,uint32_t numPerceptrons,int32_t threshold){
      this->numPerceptrons = numPerceptrons;
      this->numWeight = numWeight;
      this->threshold = threshold;
      // init percetable
      this->perceTable.resize(numPerceptrons);
      // init each perceptrons
      for(uint32_t i=0;i<this->numPerceptrons;i++)
        this->perceTable[i].resize(numWeight);
    }
    void trainning();
    uint32_t numPerceptrons;
    uint32_t numWeight;
    int32_t threshold;
    // percetrons table
    std::vector<std::vector<int32_t>> perceTable;
  };
  // update 
  BranchPredictor(int32_t numHistory=0,int32_t numPerce=0,int32_t threshold = 0);
  ~BranchPredictor();

  bool predict(uint32_t pc, uint32_t insttype, int64_t op1, int64_t op2,
               int64_t offset);

  // For Branch Prediction Buffer 
  void update(uint32_t pc, bool branch);

  std::string strategyName();
  
private:
  enum PredictorState {
    STRONG_TAKEN = 0, WEAK_TAKEN = 1,
    STRONG_NOT_TAKEN = 3, WEAK_NOT_TAKEN = 2,
  } predbuf[PRED_BUF_SIZE]; // initial state: WEAK_TAKEN
  // history table to record the result of the last k branch
  std::vector<int8_t> historyTable;
  int32_t historySize;
  Perceptron perceptron;
};

#endif