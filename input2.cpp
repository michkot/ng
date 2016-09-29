
int main(int argc, char** argv)
{
  int* supernumber = new int();
  *supernumber = 64;
  if (argc <= 2)
    *supernumber = *supernumber + argc;
  else
    *supernumber = *supernumber - argc;

  return *supernumber;
}

