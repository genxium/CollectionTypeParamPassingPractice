#include <map>
#include <stdio.h>
#include <stdexcept>
using namespace std;

bool isOnStack(void* addr, void* pStackAddrPile0, int stackAddrGrowthDir) {
  if (-1 == stackAddrGrowthDir) {
    // In this case "any addr in heap" is larger than "pStackAddrPile0 of the current LinuxProcess".
    return (addr < pStackAddrPile0);
  } else {
    return (addr > pStackAddrPile0);
  }
  return false;
}

void func1(std::map<int, int> param, void* pStackAddrPile0, int stackAddrGrowthDir) {
  void* pParamInFunc1 = (void*)(&param);
  printf("\n------------------------------------------\nExecuting func1\n\t&param == %p, isOnStack == %d\n\n", pParamInFunc1, isOnStack(pParamInFunc1, pStackAddrPile0, stackAddrGrowthDir));
      
  for (map<int, int>::iterator it = param.begin(); it != param.end(); ++it) {
    void* elementAddr = (void*)(&(param[it->first]));
    printf("\tparam[%d] == %d, &(param[%d]) == %p, isOnStack == %d\n", it->first, it->second, it->first, elementAddr, isOnStack(elementAddr, pStackAddrPile0, stackAddrGrowthDir));
  }
  param[0] = 9999;
}

void func2(std::map<int, int> *pParam, void* pStackAddrPile0, int stackAddrGrowthDir) {
  printf("\n------------------------------------------\nExecuting func2\n\tpParam == %p, isOnStack == %d\n", pParam, isOnStack((void*)pParam, pStackAddrPile0, stackAddrGrowthDir));
  printf("\t&pParam == %p, isOnStack == %d\n", (&pParam), isOnStack((void*)(&pParam), pStackAddrPile0, stackAddrGrowthDir));
  for (map<int, int>::iterator it = pParam->begin(); it != pParam->end(); ++it) {
    void* elementAddr = (void*)(&((*pParam)[it->first]));
    printf("\t(*pParam)[%d] == %d, &((*pParam)[%d]) == %p, isOnStack == %d\n", it->first, it->second, it->first, elementAddr, isOnStack(elementAddr, pStackAddrPile0, stackAddrGrowthDir));
  }

  map<int, int> inFunc2MapVar = (*pParam);
  printf("\n\t(&inFunc2MapVar) == %p, isOnStack == %d\n", (&inFunc2MapVar), isOnStack((&inFunc2MapVar), pStackAddrPile0, stackAddrGrowthDir));
  for (map<int, int>::iterator it = inFunc2MapVar.begin(); it != inFunc2MapVar.end(); ++it) {
    void* elementAddr = (void*)(&(inFunc2MapVar[it->first]));
    printf("\tinFunc2MapVar[%d] == %d, &(inFunc2MapVar[%d]) == %p, isOnStack == %d\n", it->first, it->second, it->first, elementAddr, isOnStack(elementAddr, pStackAddrPile0, stackAddrGrowthDir));
  }

  (*pParam)[0] = 9999;
}

int main() {
  int stackAddrPile0;
  int stackAddrPile1;
  void* pStackAddrPile0 = (void*)(&stackAddrPile0);
  void* pStackAddrPile1 = (void*)(&stackAddrPile1);
  int stackAddrGrowthDir = (pStackAddrPile0 > pStackAddrPile1 ? -1 : +1);

  char const * const s = "This program only works if the followings are ALL TRUE.\n\t- Address growth direction is unanimous in both `main` and other `function`s.\n\t- Address in heap is always in the opposite direction of `stackAddrGrowthDir`."; 

  map<int, int> tmp;
  tmp[0] = 0;
  tmp[1] = 1;
  tmp[2] = 2;
  printf("pStackAddrPile0 == %p\n&tmp == %p, isOnStack == %d\n------------------------------------------\nBefore any func[x]\n", pStackAddrPile0, &tmp, isOnStack(&tmp, pStackAddrPile0, stackAddrGrowthDir));
  for (map<int, int>::iterator it = tmp.begin(); it != tmp.end(); ++it) {
    void* elementAddr = (void*)(&(tmp[it->first])); 
    bool elementAddrIsOnStack = isOnStack(elementAddr, pStackAddrPile0, stackAddrGrowthDir);
    if (true == elementAddrIsOnStack) {
      throw invalid_argument(s);
    } 
    printf("\ttmp[%d] == %d, &(tmp[%d]) == %p, isOnStack == %d\n", it->first, it->second, it->first, elementAddr, elementAddrIsOnStack);
  }

  func1(tmp, &stackAddrPile0, stackAddrGrowthDir);
  printf("\n------------------------------------------\nAfter func1\n");
  for (map<int, int>::iterator it = tmp.begin(); it != tmp.end(); ++it) {
    printf("\ttmp[%d] == %d\n", it->first, it->second);
  }

  func2(&tmp, &stackAddrPile0, stackAddrGrowthDir);
  printf("\n------------------------------------------\nAfter func2\n");
  for (map<int, int>::iterator it = tmp.begin(); it != tmp.end(); ++it) {
    printf("\ttmp[%d] == %d\n", it->first, it->second);
  }
  printf("\n");

  return 0;
}
