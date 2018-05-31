#include <iostream>
#include "proxy.hpp"

int main()
{
  if(!initialize()){
    std::cout << "error initialize"<< std::endl;
    return 1;
  }

  proxy_core();

  if(finalize()){
    std::cout << "success" << std::endl;
  }else{
    std::cout << "error finalize" << std::endl;
    return 1;
}
  return 0;
}
