#include <vector>

void fce2(){
    int x;
}

int main(int argc, char** argv){
    int supernumber = 1;
    supernumber = supernumber + argc;
    if(supernumber <= 2)
        supernumber++;
    else
        fce2();
    return supernumber;
}

