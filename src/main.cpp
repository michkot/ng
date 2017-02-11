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
#include <boost/utility/string_view.hpp>
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
#include "ForwardNullAnalysis.hh"
#include "FrontendLlvm.hh"

// fronta stavů ke zpracování
ref_queue<IState> toProcess{};

void VerificationLoop()
{
  // smyčka
  while (!StatesManger::IsWorklistEmpty())
  {
    // načti stav
    IState& state = StatesManger::WorklistDequeue();

    // zpracuj stav
    {
      // vypočet nových stavů
      if (!state.IsNew())
        continue;

      state.nextCfgNode.GetDebugInfo();
      state.nextCfgNode.Execute(state);
    }

  }
}

//#define USE_Z3
#if defined(USE_Z3)
#include "ValuesZ3.hh"

Z3ValueContainer vc;
#else
#include "ValueRepImplementation.hh"

ValueContainer vc;
#endif

#include "FrontedValueMapper.hh"
Mapper mapper{vc};

void Verify()
{
  auto f = FnaOperationFactory{};
  LlvmCfgParser parser{f, vc, mapper};
  auto& firstNode = parser.ParseAndOpenIrFile("examples/01_mincase_01_nullptr_dereference[dead].ll");//("input-int-conv.ll");

  auto emptyStateUPtr = make_unique<ForwardNullAnalysisState>(firstNode.GetPrevs()[0], firstNode, vc, mapper);

  firstNode.GetStatesManager().InsertAndEnqueue(move(emptyStateUPtr));

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
  //this is end of experimental code

  Verify();

  vc.PrintDebug();

  getchar();
  return 0;
}