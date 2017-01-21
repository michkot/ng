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

// fronta stavů ke zpracování
ref_queue<IState> toProcess{};

void VerificationLoop()
{
  // smyčka
  while (!StateManger::IsWorklistEmpty())
  {
    // načti stav
    IState& state = StateManger::WorklistDequeue();

    // zpracuj stav
    {
      // vypočet nových stavů
      if (!state.IsNew())
        continue;

      state.nextCfgNode.Execute(state);
    }

  }
}

#include "ValuesZ3.hh"

Z3ValueContainer vc;
void Verify()
{
  auto f = DummyOperationFactory{};
  LlvmCfgParser parser{f, vc};
  auto& firstNode = parser.ParseAndOpenIrFile("input-int-conv.ll");

  auto emptyStateUPtr = make_unique<DummyState>(firstNode.GetPrevs()[0], firstNode);

  //emptyStateUPtr->get

  VerificationLoop();
}


#ifdef _WIN32
#include <Windows.h>
#endif

int main()
{
#ifdef _WIN32
  SetConsoleTitle("NextGen");
#endif

  //this is a debugging code for experimenting
  //vc.CreateVal(Type{0});
  for (int i = 0; i < 32; i++)
  {
    vc.CreateConstIntVal(i);
  }
  for (int i = 1; i < 33; i++)
  {
    vc.CreateConstIntVal(-i);
  }

  Verify();

  vc.PrintValues();

  getchar();
  return 0;
}