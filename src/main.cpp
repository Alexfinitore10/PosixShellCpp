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

struct CommandType
{
  std::string command;
  std::filesystem::path command_path;
  Type type;
  int argn;
};



std::vector<std::string> parsingTheCommand(std::string);
std::filesystem::path findPath(std::string);
CommandType checkCommand(std::string);
void doBuiltin(CommandType, std::vector<std::string>);
void doExecutable(CommandType, std::vector<std::string>);
void TypeCheck(CommandType);

 


int main(int argc, char* argv[]) {
  auto const t0 = std::chrono::high_resolution_clock::now();
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  std::string input;

  //REPL
  while (1)
  {
    CommandType commandType;
    std::cout << "$ ";

    std::getline(std::cin, input);

    std::vector<std::string> parsedCommand = parsingTheCommand(input);//divide tutto in array
    
   /*  for (auto a : parsedCommand)
    {
      std::cout<<a<<std::endl;
    } */

    //devo mandare il primo elemento dell'array a far capire se è executable, builtin o non trovato
    commandType = checkCommand(parsedCommand[0]);

    switch (commandType.type)
    {
    case Type::Builtin : 
      if(commandType.command == "exit")
      {
        exit(0);
      }
      doBuiltin(commandType, parsedCommand);
      break;
    case Type::Executable :
      doExecutable(commandType, parsedCommand);
      break;
    case Type::NotFound :
      std::cout<<commandType.command<<" not found"<<std::endl;
    default:
      break;
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

std::filesystem::path findPath(std::string input)
{
  const char * path = std::getenv("PATH");//mi prendo il path
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

CommandType checkCommand(std::string comando)
{
  CommandType commandType;
  //comadno puo essere BUIltin, executable, not found
  if(comando == "echo" || comando == "exit"|| comando == "type")
  {
    commandType.type = Type::Builtin;
    commandType.command_path = findPath(comando);
    commandType.command = comando;
    return commandType;
  }
  std::string path = findPath(comando);
  if (path != "")
  {
    commandType.type = Type::Executable;
    commandType.command_path = path;
    commandType.command = comando;
    return commandType;
  }

  commandType.type = Type::NotFound;
  commandType.command = comando;
  return commandType;
  
}

void doBuiltin(CommandType cmt, std::vector<std::string> vec)
{
  //devo trovare quale builtin si riferisce e dare il risultato
  if (cmt.command == "echo")
  {
    std::cout<<"\n";
    for (int i=1; i<vec.size(); i++)
    {
      std::cout<<vec[i]<<" "<<std::endl;
    }
  }else if (cmt.command == "type")
  {
    //ci vediamo domani mattina...
    if (vec.size() < 2)
    {
      std::cout<<"Type : Missing Arguments"<<std::endl;
      return;
    }else{
      CommandType cmt;
      for (int a=1; a<vec.size(); a++)
      {
        //teoricamente dovrei fare un check progressivo del tipo e del path
        cmt = checkCommand(vec[a]);//mi parsa il comando incoming
        TypeCheck(cmt);
      }
    }
  }
}

void TypeCheck(CommandType cmt)
{
  switch (cmt.type)
  {
  case Type::Builtin :
    std::cout<<cmt.command<<" is a shell builtin"<<std::endl;
    break;
  case Type::Executable:
    std::cout<<cmt.command<<" is "<<cmt.command<<std::endl;
  case Type::NotFound:
    std::cout<<cmt.command<<" not found"<<std::endl;
  default:
    break;
  }
}


void doExecutable(CommandType cmt, std::vector<std::string> vec)
{
  std::string fullCommand = cmt.command_path;
  //se è eseguibile... ha il path e ha gli argomenti
  for(int i = 1 ; i<vec.size(); i++)
  {
    fullCommand += " ";
    fullCommand += vec[i];
  }
  const char *command_path = fullCommand.c_str();
  system(command_path);
}

