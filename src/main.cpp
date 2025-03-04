#include <iostream>
#include <sstream>


int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  std::string input;

  //REPL
  while (1)
  {
    std::cout << "$ ";

    std::getline(std::cin, input);

    if(input == "exit 0")
    {
      exit(EXIT_SUCCESS);
    }else if(input.find("echo ") == 0){
      std::string text = input.substr(5);
      std::cout << text << std::endl;
    }else if (input.substr(0,5) == "type "){
      std::string tipo = input.substr(5);
      //Discriminating if it's NOT an executable file (env)
      if(tipo.substr(0,4) == "echo" || tipo.substr(0,4) == "type" || tipo.substr(0,4) == "exit"){
        std::cout << tipo << " is a shell builtin" << std::endl;
      }else{
        bool found = false;
        const char * path = std::getenv("PATH");//mi prendo il path
        std::istringstream path_stream(path);
        std::string buffer;
        while(std::getline(path_stream, buffer, ':')){
          if(buffer.find(tipo) != std::string::npos)
          {
            std::cout<<tipo<<" is "<<buffer<<std::endl;
            found = true;
            break;
          }
        }
        if(found == false)std::cout << tipo << ": not found" << std::endl;
      }
    }else{
      std::cout << input << ": command not found" << std::endl;
    }
  }
}
