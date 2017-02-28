#include <llvm/Support/CommandLine.h>
#include <string>

using namespace ::std;
using namespace ::llvm;

cl::opt<string> OutputFilename("o", cl::desc("Specify output filename"), cl::value_desc("filename"), cl::init("-"));
cl::opt<string> InputFilename(cl::Positional, cl::desc("<input file>"), cl::init(""));
cl::opt<bool>   Analysis("a", cl::desc("Enable analysis"));

extern int main_old(void);
extern void playground();

int main(int argc, char** argv)
{
  cl::ParseCommandLineOptions(argc, argv);

  if (!Analysis)
  {
    playground();
    return 0;
  }
  else
  {
#if 0
    return main_old();
#endif
  }
}

