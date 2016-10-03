#include <cstdio>
#include <memory>

#include <functional>
#include <vector>
#include <queue>
#include <stack>
#include <tuple>

#include <boost/range.hpp>

#include <boost/range.hpp>
#include <boost/logic/tribool.hpp>
//KISS!!!

//Q:
//je mo�n�, �e po napl�nov�n� stavu k dal��mu kroku dojde k p�id�n� dal��ho kontextu do stejn� lokace
//a k abstrakci t�cho SMG?
//p�edpoklad - ne?

//logick� ohodnocen� parametr� stavu
//je dosa�iteln�? -> hodnotov� syst�m p�i p�id�n� podm�nek, ��k� n�m �e nelze
//do�lo k chyb� pam�ti (predator works!!!)

//do budoucna - zm�nit flow nov�ch stav� tak,
//aby tv�rce dom�ny mohl m�t plnou kontrolu na zp�sobem p�id�v�n� do worklistu

//indirect call of malloc - jak se o nej postarat aby se dostal do analyz�toru tam kam pat��

//#include "core\LlvmLoader.hh"
//#include "core\LlvmSourceParser.hh"
//#include "domains\smg\SpcOperationsFactory.hh"


using namespace ::std;
//using namespace ::boost;

#include "Definitions.hh"
#include "General.hh"
#include "Values.hh"
#include "IState.hh"
#include "IOperation.hh"
#include "ICfgNode.hh"
#include "DummyOperations.hh"
#include "FrontendLlvm.hh"

//fronta stav� ke zpracov�n�
ref_queue<IState> toProcess{};

void VerificationLoop()
{
  //smy�ka
  while (!toProcess.empty())
  {
    //na�ti stav
    IState& state = toProcess.front();
    toProcess.pop();

    //zpracuj stav
    {
      //vypo�et nov�ch stav�
      if (!state.IsNew())
        continue;

      state.nextCfgNode.Execute(state);
    }

  }
}


void Verify()
{
  auto f = DummyOperationFactory{};
  LlvmCfgParser parser{f};
  auto& firstNode = parser.ParseAndOpenIrFile("input2.bc");

  auto emptyStateUPtr = make_unique<DummyState>(firstNode.GetPrevs()[0], firstNode);

  toProcess.push(*emptyStateUPtr);

  VerificationLoop();
}

int main()
{
  Verify();
  return 0;
}