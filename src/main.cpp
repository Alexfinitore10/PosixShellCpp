#include <iostream>
#include <sstream>
#include <cstdlib>
#include <filesystem>
#include <vector>


#include <chrono>



enum Type{
  Executable,
  Builtin,
  NotFound,
};


std::vector<std::string> parsingTheCommand(std::string);
std::filesystem::path isInPath(std::string, std::string);
std::pair<Type, std::string> generateFullCommand(std::string);

 


int main(int argc, char* argv[]) {
  auto const t0 = std::chrono::high_resolution_clock::now();
  const char * path = std::getenv("PATH");//mi prendo il path
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  std::string input;

  //REPL
  while (1)
  {
    std::cout << "$ ";

    std::getline(std::cin, input);

    std::vector<std::string> parsedCommand = parsingTheCommand(input);
    
    /* for (auto a : parsedCommand)
    {
      std::cout<<a<<std::endl;
    } */
    
    if(input == "exit 0")
    {
      //auto const t1 = std::chrono::high_resolution_clock::now();
      //std::chrono::duration<double> duration = t1 - t0;
      //std::cout << "implementation1 duration: " << duration.count() << "s\n";
      exit(EXIT_SUCCESS);
    }else if(input.find("echo ") == 0){//repeat
      std::string text = input.substr(5);
      std::cout << text << std::endl;
    }else if (input.substr(0,5) == "type "){//type
      std::string tipo = input.substr(5);
      //Discriminating if it's NOT an executable file (env)
      if(tipo.substr(0,4) == "echo" || tipo.substr(0,4) == "type" || tipo.substr(0,4) == "exit"){
        std::cout << tipo << " is a shell builtin" << std::endl;
      }else{
        std::filesystem::path patto = isInPath(path, tipo);
        if( patto == "")std::cout << tipo << ": not found" << std::endl;else{
          std::cout<<tipo<<" is "<<patto.string()<<std::endl;
        }
      }
    }else{
      std::filesystem::path patto = isInPath(path, input.substr(0, input.find(' ')));

      if(patto != "" && argc > 1)
      {
        for(int i=0; i<argc; i++)
        {
          patto += " ";
          patto += argv[i];
        }
        
        std::cout<<"Ending command : "<< patto.c_str()<<std::endl;

        system(patto.c_str());
      }else if (patto != "")
      {
        std::cout<<"Ending command : "<< patto.c_str()<<std::endl;

        system(patto.c_str());
      }else std::cout << input << ": command not found" << std::endl;
    }
  }
}

std::vector<std::string> parsingTheCommand(std::string input)
{
  std::vector<std::string> vec;
  std::string word_acc;

  for (char c : input)
  {
    if (c == ' ')
    {
      vec.push_back(word_acc);
      word_acc = "";
    }else{
      word_acc += c;
    }
  }
  if (word_acc != "")
  {
    vec.push_back(word_acc);
  }

  return vec;
}

std::filesystem::path isInPath(std::string path, std::string input)
{
  std::istringstream path_stream(path);
  std::string buffer;
        
  while(std::getline(path_stream, buffer, ':')){
    std::filesystem::path patto = std::filesystem::path(buffer) / input;
    if(std::filesystem::exists(patto))//mannaggia guarda
    {
      return patto;
    }
  }
  return "";
}

std::pair<Type, std::string> generateFullCommand(std::string comando)
{
  std::pair<Type, std::string> comandoCoppia;
  //comadno puo essere BUIltin, executable, not found
  if(comando == "echo" || comando == "exit" || comando == "type")
  {
    //TODO
  }
}

