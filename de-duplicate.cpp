#include <fstream>
#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <exception>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <cstdlib>
#include <map>
#include <set>
#include <tr1/functional>
using namespace std;
//hash function, give differnet path the hush value                             
size_t hashFunction(string path){//here the path is the eltimate one            
  //open the file                                                               
  ifstream  file;
  file.open(path.c_str());
  //use the object : hash                                                       
  tr1::hash<string> hash;
  string temp;
  size_t hashResult = 0;
  //read eachline                                                               
  while(getline(file, temp)){
    hashResult += hash(temp);
  }
  file.close();
  return hashResult;
}
//transverse every table                                                        
void traverseDirectory(string path, map<size_t, string> * table, set<string> * \
		       record){
  DIR * dir = NULL;//pointer to the directory                                   
  dirent * entry;//pointer to the struct that is about the content of the direc\
tory                                                                            
  //deal with "/"                                                               
  if(path[path.size()-1]!= '/') path = path + "/";//path is a string            
//check if it's deplicated                                                    
 if(record->find(path)!= record->end())return;//yes it is deplicated           
 record->insert(path);
 //open                                                                        
 dir = opendir(path.c_str());
 if(dir == NULL){
   cout <<"Can not open the directory"<<path<<endl;
   return;
 }
 //read                                                                        
 while((entry = readdir(dir))){
   if(strcmp(entry->d_name,".") == 0 || strcmp(entry->d_name, "..")==0) contin\
									  ue;
   struct stat buff;
   if(lstat((path + entry->d_name).c_str(), &buff) == -1){
     cout << "Error" <<endl;
     return;
   }
   //recursively                                                               
   // if(S_ISDIR(buff.st_mode)){                                               
   if(entry->d_type == DT_DIR){
     traverseDirectory(path + entry->d_name, table, record);
   }
   else if(entry->d_type == DT_REG){
     size_t fileHash = hashFunction(path + entry->d_name);
     if(table->find(fileHash) == table->end()){
       const pair<size_t, string> newFile(fileHash,path + entry->d_name);
       table->insert(newFile);
     }
     else if(table->find(fileHash)->second.compare(path + entry->d_name)!= 0)
       {
	 string originFile = table->find(fileHash)->second;
	 cout<<"#Removing"<<(path + entry->d_name)<<"(duplicate of)"<<originFi\
	   le << ")."<<endl;
	 cout<<"rm" << (path + entry->d_name) <<endl;
       }
   }
 }
 closedir(dir);
}//main                                                                          

int main(int argc, char ** argv) {
  if (argc < 2) {
    std::cerr << "Usage: deletedup directory" << std::endl;
    return EXIT_FAILURE;
  }
  cout<<"#!/bin/bash"<<endl;
  //create a map key:hashvalue, value: path(string)                             
  map<size_t, string> * table = new map<size_t, string>();
  //create a set to store the opened directory                                  
  set<string> * record = new set<string>();
  //loop through every argument                                                 
  for(int i = 1; i < argc; i++){
    string path = string(argv[i]);
    traverseDirectory(path, table, record);
  }
  delete record;
  delete table;
  return EXIT_SUCCESS;
}



