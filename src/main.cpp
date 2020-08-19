#include <iostream>
#include "ElfReader.hpp"

void read(std::string filename)
{
  std::cout << "\n\nReading executable '" << filename << "'\n";
  auto s = ElfReader(filename);
  std::cout << s << std::endl;
}

int main(int argc, char* argv[])
{
  if ( argc < 2 ) {
    std::cout << "requires a filename as first argument" << std::endl;
    return 2;
  }
  read(argv[argc-1]);
  return 0;
}