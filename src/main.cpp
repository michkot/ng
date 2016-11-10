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
//je možné, že po naplánování stavu k dalšímu kroku dojde k přidání dalšího kontextu do stejné lokace
//a k abstrakci těcho SMG?
//předpoklad - ne?

//logické ohodnocení parametrů stavu
//je dosažitelný? -> hodnotový systém při přidání podmínek, říká nám že nelze
//došlo k chybě paměti (predator works!!!)

//do budoucna - změnit flow nových stavů tak,
//aby tvůrce domény mohl mít plnou kontrolu na způsobem přidávání do worklistu

//indirect call of malloc - jak se o nej postarat aby se dostal do analyzátoru tam kam patří

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

//fronta stavů ke zpracování
ref_queue<IState> toProcess{};

void VerificationLoop()
{
  //smyčka
  while (!toProcess.empty())
  {
    //načti stav
    IState& state = toProcess.front();
    toProcess.pop();

    //zpracuj stav
    {
      //vypočet nových stavů
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
  auto& firstNode = parser.ParseAndOpenIrFile("input-int-conv.ll");

  auto emptyStateUPtr = make_unique<DummyState>(firstNode.GetPrevs()[0], firstNode);

  toProcess.push(*emptyStateUPtr);

  VerificationLoop();
}

#include "ValuesZ3.hh"

#ifdef _WIN32
#include <Windows.h>
#endif

int main()
{
#ifdef _WIN32
  SetConsoleTitle("NextGen");
#endif

  //this is a debugging code for experimenting
  Z3ValueContainer vc;
  vc.CreateVal(Type{0});
  vc.CreateConstIntVal(0);
  vc.CreateConstIntVal(1);
  vc.CreateConstIntVal(2);
  //this is end of experimental code

  Verify();

  getchar();
  return 0;
}