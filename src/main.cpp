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
//je možné, že po naplánování stavu k dalšímu kroku dojde k pøidání dalšího kontextu do stejné lokace
//a k abstrakci tìcho SMG?
//pøedpoklad - ne?

//logické ohodnocení parametrù stavu
//je dosažitelný? -> hodnotový systém pøi pøidání podmínek, øíká nám že nelze
//došlo k chybì pamìti (predator works!!!)

//do budoucna - zmìnit flow nových stavù tak,
//aby tvùrce domény mohl mít plnou kontrolu na zpùsobem pøidávání do worklistu

//indirect call of malloc - jak se o nej postarat aby se dostal do analyzátoru tam kam patøí

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

//fronta stavù ke zpracování
ref_queue<IState> toProcess{};

void VerificationLoop()
{
  //smyèka
  while (!toProcess.empty())
  {
    //naèti stav
    IState& state = toProcess.front();
    toProcess.pop();

    //zpracuj stav
    {
      //vypoèet nových stavù
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