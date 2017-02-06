#include <stdlib.h>

typedef struct main_struct_s main_struct;
struct main_struct_s
{
  int prvni_int;
  int druhy_int;
  struct vnor_struct_s {
    char a;
    char b;
  } vnorena;
};

void nastav_strukturu(main_struct* ptr)
{
  // predelat na ->
  ptr[0].vnorena.a = (char)'a';
  ptr[0].vnorena.b = (char)'b';
}

int zkontroluj_strukturu(main_struct* ptr)
{
  return (ptr[0].vnorena.a == (char)'a') & (ptr[0].vnorena.b == (char)'b');
}

int main(int argc, char** argv)
{
    int supernumber = -1;
    supernumber = supernumber + argc;

    main_struct struktura_init = { 0 };
    main_struct struktura_neinit;

    main_struct* ptr = &struktura_init;

    nastav_strukturu(ptr);

    if (supernumber == 0)
    {
      ptr = NULL;
      return zkontroluj_strukturu(ptr);
    }
    else if(supernumber == 1)
    {
      ptr = &struktura_neinit;
      return zkontroluj_strukturu(ptr);
    }
    else
    {
      return zkontroluj_strukturu(ptr);
    }
}

