#include <iostream>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;


  //REPL
  while (1)
  {
    std::cout << "$ ";
  
    std::string input;

    std::getline(std::cin, input);

    if(input == "exit 0")
    {
      exit(EXIT_SUCCESS);
    }else if(input.find("echo ") == 0){
      std::string text = input.substr(5);
      std::cout << text << std::endl;
    }else if (input.find("type echo") == 0){
      std::cout << input.substr(5) << " is a shell builtin" << std::endl;
    }else if (input.find("type exit") == 0){
      std::cout << input.substr(5) << " is a shell builtin" << std::endl;
    }else if (input.find("type type") == 0){
      std::cout << input.substr(5) << " is a shell builtin" << std::endl;
    }else{
      std::cout << input << ": command not found" << std::endl;
    }
  }
}
