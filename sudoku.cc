#include "sudoku.h"

int main(int argc, char* argv[]){
  initMain(argc, argv);
  if( modeResolve ){
    copyNums(numbers, prob);
    copyNums(numbers, numbers2);
    cout << "Prob." << endl;
    printNumbers(numbers);
    cout << endl;
    findCandidateNumber(numbers , emptySlot , candidate );
    findCandidateNumber(numbers2, emptySlot2, candidate2);
    
    cout << "Initial Number of EmptySlots: " << emptySlot.size() << endl;
    
    resolve(numbers, emptySlot, candidate);
    printNumbers(numbers);
    cout << "Number of EmptySlots after the inteligent solutions: " << emptySlot.size() << endl;
    cout << endl;
    if( resolveSimple(numbers, emptySlot, candidate, 0, false, true) && resolveSimple(numbers2, emptySlot2, candidate2, 0, false, false) && compNumbers() ){
      cout << "Answer" << endl;
      printNumbers(numbers);
    }else{
      printNumbers(numbers);
      printNumbers(numbers2);
      cout << "No solution" << endl;
    }
  }else{
    cout << "generate base" << endl;
    bool isGoodMatrix = false;
    while( ! isGoodMatrix ){
      memset(prob, 0, sizeof(prob));
      findCandidateNumber(prob , emptySlot , candidate );
      isGoodMatrix = resolveSimple(prob , emptySlot, candidate, 0, true);
    }
    cout << "Created base" << endl;
    printNumbers(prob);
    copyNums(prob, generated);
    vector<pair<int, int> > tried_slots;
    makeEmptySlot(tried_slots);
    cout << "Created prob: empty slots = " << getNEmpty(prob) << endl;
    printNumbers(prob);
    cout << "TEST RESOLVE" << endl;
    copyNums(prob, numbers);
    copyNums(prob, numbers2);
    findCandidateNumber(numbers , emptySlot , candidate );
    findCandidateNumber(numbers2, emptySlot2, candidate2);
    resolveSimple(numbers , emptySlot , candidate , 0, false, false);
    resolveSimple(numbers2, emptySlot2, candidate2, 0, false,  true);
    if( ! compNumbers() ) cout << " !!! INCONSISTENT !!!  " << endl;
    cout << "GENERATED" << endl;
    printNumbers(generated);
    cout << "ANSWER1" << endl;
    printNumbers(numbers);
    cout << "ANSWER2" << endl;
    printNumbers(numbers2);
    cout << "PROBLEM" << endl;
    printNumbers(prob);
    
  }
}

///////////////////
//// FUNCTION /////
///////////////////
//// INIT
void initMain(int argc, char* argv[]){
  if( argc == 2 )
    {
      modeResolve = true;
      inputNumber(numbers, argv[1]);
    }
  else if( argc == 1 )
    {
      modeResolve = false;
      clearNums(numbers);
      rndm.SetSeed();
    }
  else
    {
      cout << "Too many arguments" << endl;
    }
}

//// SET
void setNum(int row, int line, int tempNumbers[9][9], int num){
  tempNumbers[row][line] = num;
  return;
}

void copyNums(int originalNumbers[9][9], int copiedNumbers[9][9]){
  for( int irow = 0 ; irow < 9 ; irow++ ){
    for( int iline = 0 ; iline < 9 ; iline++ ){
      setNum(irow, iline, copiedNumbers, originalNumbers[irow][iline]);
    }
  }
  return;
}

void clearNums(int tempNumbers[9][9]){
  for( int irow = 0 ; irow < 9 ; irow++ ){
    for( int iline = 0 ; iline < 9 ; iline++ ){
      tempNumbers[irow][iline] = 0;
    }
  }
  return;
}

void inputNumber(int tempNumbers[9][9], char* fileName){
  ifstream ifs;
  ifs.open(fileName);
  int irow  = 0;
  int iline = 0;
  int source = 0;
  while( ifs >> source ){
    if( source != 0 ) tempNumbers[irow][iline] = (1<<(source-1));
    else tempNumbers[irow][iline] = 0;
    iline++;
    if( iline == 9 ){
      irow++;
      iline = 0;
    }
  }
  return;
}

//// __builtin
int bitCount(int num){
  return __builtin_popcount(num);
}

int getBit(int num){
  return __builtin_ctz(num);
}

//// FIND
void makeBlock(int tempNumbers[9][9], int block[9], int irow, int iline){
  int  brow = (int)( irow/3.) * 3;
  int bline = (int)(iline/3.) * 3;
  int ID = 0;
  for( int iirow = brow ; iirow < brow + 3 ; iirow++ ){
    for( int iiline = bline ; iiline < bline + 3 ; iiline++ ){
      block[ID++] = tempNumbers[iirow][iiline];
    }
  }
  return;
}

void findEmptySlot(int tempNumbers[9][9], vector<pair<int, int> >& tempEmptySlot){
  if( tempEmptySlot.size() != 0 ) tempEmptySlot.clear();

  for( int irow = 0 ; irow < 9 ; irow++ ){
    for( int iline = 0 ; iline < 9 ; iline++ ){
      if( bitCount(tempNumbers[irow][iline]) != 1 ) tempEmptySlot.push_back(make_pair(irow, iline));
    }
  }
  return;
}

void findCandidateNumber(int tempNumbers[9][9], vector<pair<int, int> >& tempEmptySlot, int tempCandidate[9][9]){
  findEmptySlot(tempNumbers, tempEmptySlot);

  for( int iEmpty = 0 ; iEmpty < tempEmptySlot.size() ; iEmpty++ ){
    int  irow = tempEmptySlot[iEmpty].first;
    int iline = tempEmptySlot[iEmpty].second;
    findCandidateNumber(irow, iline, tempNumbers, tempCandidate);
  }

  nakedRowLine(tempNumbers, tempEmptySlot, tempCandidate,  true);
  nakedRowLine(tempNumbers, tempEmptySlot, tempCandidate, false);
  nakedBlock  (tempNumbers, tempEmptySlot, tempCandidate);

  XWingRowLine(tempNumbers, tempEmptySlot, tempCandidate,  true);
  XWingRowLine(tempNumbers, tempEmptySlot, tempCandidate, false);

  return;
}

void findCandidateNumber(int irow, int iline, int tempNumbers[9][9], int tempCandidate[9][9]){
  int tempBlock[9];
  int tempNumber = 0;
  makeBlock(tempNumbers, tempBlock, irow, iline);
  for( int iirow = 0 ; iirow < 9 ; iirow++ ){
    if( bitCount(tempNumbers[iirow][iline]) == 1 ) tempNumber |= tempNumbers[iirow][iline];
  }
  for( int iiline = 0 ; iiline < 9 ; iiline++ ){
    if( bitCount(tempNumbers[irow][iiline]) == 1 ) tempNumber |= tempNumbers[irow][iiline];
  }
  for( int iblock = 0 ; iblock < 9 ; iblock++ ){
    if( bitCount(tempBlock[iblock]) == 1 ) tempNumber |= tempBlock[iblock];
  }

  tempCandidate[irow][iline] = 511 ^ tempNumber;

  return;  
}

void nakedRowLine(int tempNumbers[9][9], vector<pair<int, int> >& tempEmptySlot, int tempCandidate[9][9], bool row){
  int irow;
  int iline;

  for( int ibase = 0 ; ibase < 9 ; ibase++ ){

    for( int i  = 0 ; i < 9 ; i++ ){
      irow  = row ? ibase  : i;
      iline = row ? i : ibase ;
      int pos = 1<<i;
      for( int j = i + 1 ; j < 9 ; j++ ){
	int jrow  = row ? ibase  : j;
	int jline = row ? j : ibase ;
	if( tempCandidate[irow][iline] != tempCandidate[jrow][jline] ) continue;
	pos += 1<<j;
	if( bitCount(pos) != bitCount(tempCandidate[irow][iline]) )continue;
	for( int k = 0 ; k < 9 ; k++ ){
	  int krow  = row ? ibase  : k;
	  int kline = row ? k : ibase ;
	  if( pos & (1<<k) ) continue;
	  tempCandidate[krow][kline] -= (tempCandidate[irow][iline] & tempCandidate[krow][kline]);
	}
	break;
      }
    }

  }
  return;
}

void nakedBlock(int tempNumbers[9][9], vector<pair<int, int> >& tempEmptySlot, int tempCandidate[9][9]){
  int irow;
  int iline;

  for( int row = 0 ; row < 9 ; row += 3 ){
    for( int line = 0 ; line < 9 ; line += 3 ){

      for( int i = 0 ; i < 9 ; i++ ){
	irow  = row  + i / 3;
	iline = line + i % 3;
	int pos = 1<<i;
	for( int j = i + 1 ; j < 9 ; j++ ){
	  int jrow  = row  + j / 3;
	  int jline = line + j % 3;
	  if( tempCandidate[irow][iline] != tempCandidate[jrow][jline] ) continue;
	  pos += 1<<j;
	  if( bitCount(pos) != bitCount(tempCandidate[irow][iline]) )continue;
	  for( int k = 0 ; k < 9 ; k++ ){
	    int krow  = row  + k / 3;
	    int kline = line + k % 3;
	    if( pos & (1<<k) ) continue;
	    tempCandidate[krow][kline] -= (tempCandidate[irow][iline] & tempCandidate[krow][kline]);
	  }
	  break;	  
	}
      }
      
    }
  }
  return;
}

void XWingRowLine(int tempNumbers[9][9], vector<pair<int, int> >& tempEmptySlot, int tempCandidate[9][9], bool row){

  int nNumbers[9][9] = {{0}};
  for( int ibase = 0 ; ibase < 9 ; ibase++ ){
    for( int inum = 0 ; inum < 9 ; inum++ ){
      for( int i  = 0 ; i < 9 ; i++ ){
	int irow  = row ? ibase  : i;
	int iline = row ? i : ibase ;
	if( tempCandidate[irow][iline] & (1<<inum) ) nNumbers[ibase][inum] += (1<<i);
      }
    }
  }

  for( int inum = 0 ; inum < 9 ; inum++ ){
    for( int ibase = 0 ; ibase < 9 ; ibase++ ){
      int nSame = 1<<ibase;
      for( int jbase = ibase + 1 ; jbase < 9 ; jbase++ ){
	if( nNumbers[ibase][inum] != nNumbers[jbase][inum] ) continue;
	nSame += 1<<jbase;
	if( bitCount(nSame) != bitCount(nNumbers[ibase][inum]) ) continue;
	for( int kbase = 0 ; kbase < 9 ; kbase++ ){
	  if( nSame & (1<<kbase) ) continue;
	  int nbit = bitCount(nSame);
	  int numbit = nNumbers[ibase][inum];
	  for( int ibit = 0 ; ibit < nbit ; ibit++ ){
	    int krow  = row ? kbase : getBit(numbit);
	    int kline = row ? getBit(numbit) : kbase;
	    if( tempCandidate[krow][kline] & (1<<inum) ) tempCandidate[krow][kline] -= (1<<inum);
	    numbit -= (1<<getBit(numbit));
	  }
	}
      }
    }
  }
  return;
}

// void XYWing(int tempNumbers[9][9], vector<pair<int, int> >& tempEmptySlot, int tempCandidate[9][9]){

// }

bool isValid(int tempNumbers[9][9]){
  for( int irow = 0 ; irow < 9 ; irow++ ){
    for( int iline = 0 ; iline < 9 ; iline++ ){
      if( bitCount(tempNumbers[irow][iline]) != 1 ) return false;
      if( !isValid(irow, iline, tempNumbers) )return false;
    }
  }
  return true;
}

bool isValid(int irow, int iline, int tempNumbers[9][9]){
  int tempBlock[9];
  makeBlock(tempNumbers, tempBlock, irow, iline);
  int number = tempNumbers[irow][iline];
  for( int iirow = 0 ; iirow < 9 ; iirow++ ){
    if( iirow != irow && tempNumbers[iirow][iline] == number ){ return false; }
  }
  
  for( int iiline = 0 ; iiline < 9 ; iiline++ ){
    if( iiline != iline && tempNumbers[irow][iiline] == number ){ return false; }
  }

  int candNum = 0;
  for( int iblock = 0 ; iblock < 9 ; iblock++ ){
    if( tempBlock[iblock] == number ){
      candNum++;
      if( candNum == 2 ){
	return false;
      }
    }
  }

  return true;
}

bool compNumbers(){
  for( int irow = 0 ; irow < 9 ; irow++ )
    for( int iline = 0 ; iline < 9 ; iline++ )
      if( numbers[irow][iline] != numbers2[irow][iline] ) return false;
  return true;
}

void makeEmptySlot(vector<pair<int, int> >& tried_slots){

  int nEmpties = getNEmpty(prob);
  
  if( tried_slots.size() == 81 - nEmpties ) return;
  
  copyNums(prob, numbers);
  
  int  irow = (int)( rndm.Rndm() * 9 );
  int iline = (int)( rndm.Rndm() * 9 );

  bool tried = false;
  
  while( numbers[irow][iline] == 0 || tried ){
    irow  = (int)( rndm.Rndm() * 9 );
    iline = (int)( rndm.Rndm() * 9 );    
    std::vector<pair<int,int> >::iterator it =  std::find(tried_slots.begin(), tried_slots.end(), make_pair(irow, iline)); 
    if( it != tried_slots.end() ) tried =  true;
    else                          tried = false;
  }

  numbers[irow][iline] = 0;
  copyNums(numbers, numbers2);  
  findCandidateNumber(numbers , emptySlot , candidate );
  findCandidateNumber(numbers2, emptySlot2, candidate2);  
  if( resolveSimple(numbers, emptySlot, candidate, 0, false, true) && resolveSimple(numbers2, emptySlot2, candidate2, 0, false, false) && compNumbers() ){
    prob[irow][iline] = 0;
    tried_slots.clear();
    makeEmptySlot(tried_slots);
  }else{
    tried_slots.push_back(make_pair(irow, iline));
    makeEmptySlot(tried_slots);
  }
}

vector<int> makeCandVector(int nums, bool randCand){
  vector<int> numVector;
  int num = 0;
  while( numVector.size() != nums ){
    num = randCand ? (int)(rndm.Rndm() * nums) : num + 1;
    std::vector<int>::iterator it = std::find(numVector.begin(), numVector.end(), num);
    if( it == numVector.end() ) numVector.push_back(num);
  }
  return numVector;
}

int getNEmpty(int tempNumbers[9][9]){
  int count = 0;
  for( int irow = 0 ; irow < 9 ; irow++ )
    for( int iline = 0 ; iline < 9 ; iline++ )
      if( tempNumbers[irow][iline] == 0 ) count++;
  return count;
}

//// RESOLVE
bool resolveSimple(int tempNumbers[9][9], vector<pair<int, int> >& tempEmptySlot, int tempCandidate[9][9], int iEmpty, bool randCand, bool low2hi){

  if( getNEmpty(tempNumbers) == 0 )
    return isValid(tempNumbers);
     
  int  irow = tempEmptySlot[iEmpty]. first;
  int iline = tempEmptySlot[iEmpty].second;
  int tempNums = bitCount(tempCandidate[irow][iline]);

  int sCand = 0;
  int eCand = tempNums;
  int sign = 1;
  
  if( ! low2hi ){
    eCand =          -1;
    sCand = tempNums -1;
    sign =           -1;
  }

  vector<int> candVector = makeCandVector(tempNums, randCand);
  for( int iCand = sCand; sign * iCand < sign * eCand ; iCand += sign * 1 ){
    int num = 1 << getBit(tempCandidate[irow][iline]);
    int tmp = tempCandidate[irow][iline];
    for( int i = 0 ; i < candVector[iCand] ; i++ ){
      tmp -= num;
      num = 1 << getBit(tmp);
    }
    setNum(irow, iline, tempNumbers, num);

    if( !isValid(irow, iline, tempNumbers) ){ setNum(irow, iline, tempNumbers, 0); continue; }
    if( resolveSimple(tempNumbers, tempEmptySlot, tempCandidate, iEmpty+1, randCand, low2hi) )  return true;
  }
  setNum(irow, iline, tempNumbers, 0);
  return false;
}

void resolve(int tempNumbers[9][9], vector<pair<int, int> >& tempEmptySlot, int tempCandidate[9][9]){
  int b_empty = 100;
  int a_empty = 0;
  while( b_empty - a_empty > 0 ){
    b_empty = tempEmptySlot.size();
    resolveOneCand(tempNumbers, tempEmptySlot, tempCandidate);
    resolveRowLineBlock(tempNumbers, tempEmptySlot, tempCandidate);
    findCandidateNumber(tempNumbers, tempEmptySlot, tempCandidate);
    a_empty = tempEmptySlot.size();
  }
  return;
}

void resolveOneCand(int tempNumbers[9][9], vector<pair<int, int> >& tempEmptySlot, int tempCandidate[9][9]){
  for( int iEmpty = 0 ; iEmpty < tempEmptySlot.size() ; iEmpty++ ){
    int  irow = tempEmptySlot[iEmpty].first ;
    int iline = tempEmptySlot[iEmpty].second;
    if( bitCount(tempCandidate[irow][iline]) == 1 ){
      tempNumbers[irow][iline] = tempCandidate[irow][iline];
    }
  }
  return;
}

void resolveRowLineBlock(int tempNumbers[9][9], vector<pair<int, int> >& tempEmptySlot, int tempCandidate[9][9]){
  int b_empty = 100;
  int a_empty = 0;
  while( b_empty - a_empty > 0 ){
    b_empty = tempEmptySlot.size();
    resolveRowLine(tempNumbers, tempEmptySlot, tempCandidate,  true);
    resolveRowLine(tempNumbers, tempEmptySlot, tempCandidate, false);
    resolveBlock  (tempNumbers, tempEmptySlot, tempCandidate);
    a_empty = tempEmptySlot.size();
  }
  return;
}

void resolveRowLine(int tempNumbers[9][9], vector<pair<int, int> >& tempEmptySlot, int tempCandidate[9][9], bool row){
  int iirow;
  int iiline;

  for( int irow = 0 ; irow < 9 ; irow++ ){
    int nNumbers[9] = {0};

    for( int inum = 0 ; inum < 9 ; inum++ ){
      for( int iline  = 0 ; iline < 9 ; iline++ ){
	iirow  = row ? irow  : iline;
	iiline = row ? iline : irow ;
	if( tempCandidate[iirow][iiline] & (1<<inum) ) nNumbers[inum] += (1<<iline);
      }
    }
    for( int inum = 0 ; inum < 9 ; inum++ ){
      iirow  = row ? irow : getBit(nNumbers[inum]);
      iiline = row ? getBit(nNumbers[inum]) : irow;
      
      if( bitCount(nNumbers[inum]) == 1 ){
	tempNumbers  [iirow][iiline] = (1<<inum);
	findEmptySlot(tempNumbers, tempEmptySlot);
      }
    }
  }
  return;
}

void resolveBlock(int tempNumbers[9][9], vector<pair<int, int> >& tempEmptySlot, int tempCandidate[9][9]){
  int iirow;
  int iiline;

  for( int irow = 0 ; irow < 9 ; irow += 3 ){
    for( int iline  = 0 ; iline < 9 ; iline += 3 ){

      int nNumbers[9] = {0};
      
      for( int inum = 0 ; inum < 9 ; inum++ ){
	for( int iblock = 0 ; iblock < 9 ; iblock++ ){
	  iirow  = irow  + iblock / 3;
	  iiline = iline + iblock % 3;
	  if( tempCandidate[iirow][iiline] & (1<<inum) ) nNumbers[inum] += (1<<iblock);
	}
      }
      for( int inum = 0 ; inum < 9 ; inum++ ){
	if( bitCount(nNumbers[inum]) == 1 ){
	  int iblock = getBit(nNumbers[inum]);
	  iirow  = irow  + iblock / 3;
	  iiline = iline + iblock % 3;
	  tempNumbers  [iirow][iiline] = (1<<inum);
	  findEmptySlot(tempNumbers, tempEmptySlot);
	}
      }

    }
  }
  return;
}

//// PRINT
void printNumbers(int tempNumbers[9][9]){
  cout << endl;
  for( int irow = 0 ; irow < 9 ; irow++ ){
    for( int iline = 0 ; iline < 9 ; iline++ ){
      if( bitCount(tempNumbers[irow][iline]) == 1 )
	cout << getBit(tempNumbers[irow][iline])+1 << " ";
      else cout << 0 << " ";
    }
    cout << endl;
  }
  cout << endl;
  return;
}

void printStatus(int tempNumbers[9][9], int tempCandidate[9][9]){
  for( int irow = 0 ; irow < 9 ; irow++ ){
    for( int iline = 0 ; iline < 9 ; iline++ ){
      if( bitCount(tempNumbers[irow][iline]) == 1 )
	cout << 0 << " ";
      else cout << bitCount(tempCandidate[irow][iline]) << " ";
    }
    cout << endl;
  }
  return;
}

void printCandMulti(int tempNumbers[9][9], int tempCandidate[9][9]){
  double multi = 1.;
  for( int irow = 0 ; irow < 9 ; irow++ ){
    for( int iline = 0 ; iline < 9 ; iline++ ){
      if( bitCount(tempNumbers[irow][iline]) != 1 )
	multi *= bitCount(tempCandidate[irow][iline]);
    }
  }
  cout << "CandMulti: " << multi << endl;
  return;
}

