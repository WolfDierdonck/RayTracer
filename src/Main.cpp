// Termm--Fall 2024

#include <iostream>
#include "./Lua/scene_lua.hpp"

int main(int argc, char **argv)
{
  std::string filename = "simple.lua";
  if (argc >= 2)
  {
    filename = argv[1];
  }

  try
  {
    if (!run_lua(filename))
    {
      std::cerr << "Could not open " << filename << ". Try running the executable from inside of" << " the Assets/ directory" << std::endl;
      return 1;
    }
  }
  catch (...) // catch all exceptions
  {
    std::cerr << "An exception occurred" << std::endl;
    return 1;
  }
}
