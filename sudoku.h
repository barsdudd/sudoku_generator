#include <stdio.h>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <istream>
#include <fstream>
#include <math.h>
#include <map>
#include <string>

#include <TH1D.h>
#include <TH2D.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TF1.h>
#include <TFile.h>
#include <TRandom.h>
#include <TCanvas.h>
#include <TGaxis.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TTree.h>
#include <TVector3.h>
#include <TAxis.h>
#include <TSystem.h>


using namespace std;

///////////////////
//// VARIABLE /////
///////////////////
int numbers[9][9];
int numbers2[9][9];
int prob[9][9];
int generated[9][9];
vector<pair<int, int> > emptySlot;
vector<pair<int, int> > emptySlot2;
int candidate[9][9];
int candidate2[9][9];
int itry = 0;
bool modeResolve;
TRandom rndm;
/////////////////////////////

///////////////////
//// FUNCTION /////
///////////////////
//// INIT
void initMain(int argc, char* argv[]);

//// SET
void setNum(int row, int line, int tempNumbers[9][9], int num);
void copyNums(int originalNumbers[9][9], int copiedNumbers[9][9]);
void clearNums(int tempNumbers[9][9]);
void inputNumber(int tempNumbers[9][9], char* fileName);

//// __builin
int bitCount(int num);
int getBit(int num);

//// FIND
void makeBlock(int tempNumbers[9][9], int block[9], int irow, int iline);
void findEmptySlot(int tempNumbers[9][9], vector<pair<int, int> >& tempEmptySlot);
void findCandidateNumber(int tempNumbers[9][9], vector<pair<int, int> >& tempEmptySlot, int tempCandidate[9][9]);
void findCandidateNumber(int irow, int iline, int tempNumbers[9][9], int tempCandidate[9][9]);
void nakedRowLine(int tempNumbers[9][9], vector<pair<int, int> >& tempEmptySlot, int tempCandidate[9][9], bool row);
void nakedBlock(int tempNumbers[9][9], vector<pair<int, int> >& tempEmptySlot, int tempCandidate[9][9]);
void XWingRowLine(int tempNumbers[9][9], vector<pair<int, int> >& tempEmptySlot, int tempCandidate[9][9], bool row);
bool isValid(int tempNumbers[9][9]);
bool isValid(int irow, int iline, int tempNumbers[9][9]);
bool compNumbers();
//// GENERATOR
vector<int> makeCandVector(int nums, bool randCand);
void makeEmptySlot(vector< pair<int, int > >& tried_slots);
int getNEmpty(int tempNumbers[9][9]);
//// RESOLVE
bool resolveSimple(int tempNumbers[9][9], vector<pair<int, int> >& tempEmptySlot, int tempCandidate[9][9], int iEmpty, bool randCand = false, bool low2hi = true);
void resolve(int tempNumbers[9][9], vector<pair<int, int> >& tempEmptySlot, int tempCandidate[9][9]);
void resolveOneCand(int tempNumbers[9][9], vector<pair<int, int> >& tempEmptySlot, int tempCandidate[9][9]);
void resolveRowLineBlock(int tempNumbers[9][9], vector<pair<int, int> >& tempEmptySlot, int tempCandidate[9][9]);
void resolveRowLine(int tempNumbers[9][9], vector<pair<int, int> >& tempEmptySlot, int tempCandidate[9][9], bool row);
void resolveBlock  (int tempNumbers[9][9], vector<pair<int, int> >& tempEmptySlot, int tempCandidate[9][9]);
//// PRINT
void printNumbers(int tempNumbers[9][9]);
void printStatus(int tempNumbers[9][9], int tempCandidate[9][9]);
void printCandMulti(int tempNumbers[9][9], int tempCandidate[9][9]);
