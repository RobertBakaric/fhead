/*
 * fhead.cpp
 * 
 * Copyright 2015 Robert Bakaric <rbakaric@irb.hr>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <fcntl.h>
#include <boost/program_options.hpp>


namespace po = boost::program_options;
using namespace std;


void PrintAcknowledgements(){

cout << "**************************************************************************************"<<"\n";
cout <<"                       fhead - head function for fasta files                           "<<"\n";
cout <<"                                          by                                           "<<"\n";
cout <<"                                    Robert Bakaric                                     "<<"\n\n";
cout <<"CONTACT:                                                                               "<<"\n";
cout <<" Code written and maintained by Robert Bakaric,                                        "<<"\n";
cout <<" email: rbakaric@irb.hr , bakaric@evolbio.mpg.de                                       "<<"\n\n";
cout <<"ACKNOWLEDGEMENT:                                                                       "<<"\n";
cout <<"                                                                                       "<<"\n\n";
cout <<"LICENSE:                                                                               "<<"\n";
cout <<" The program is distributed under the GNU General Public License. You should have      "<< "\n";
cout <<" received a copy of the licence together  with this software. If not, see              "<< "\n";
cout <<" http://www.gnu.org/licenses/                                                          "<< "\n";
cout <<"***************************************************************************************"<< "\n\n\n";
}


template <typename INT, typename CHARA>
po::variables_map SetOptions(INT& argc, CHARA& argv){

    try {
        int opt;
        string version = "1.0";
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help,h", "produce help message")
            ("version,v", "print version information")
            ("input,i",  po::value< string >()->default_value("-"), "Input fasta file")
            ("fasta,f", po::value< int >()->default_value(10),"print the first K fasta entries of each fasta file")
        ;
        
        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).
                  options(desc).run(), vm);
        po::notify(vm);
    
        if (vm.count("help")) {
            cout << "Usage: ./program [options]\n\n";
            PrintAcknowledgements();
            cout << desc;
            exit(0);
        }else if (vm.count("version")) {
            cout << "Program version:  " << version << "\n";
            exit(0);
        }
        return vm;    
    }
    catch(std::exception& e)
    {
        cout << e.what() << "\n";
        exit(0);
    }    
}

struct noop {
    void operator()(...) const {}
};




int main(int argc, char **argv){

  po::variables_map arg;
  shared_ptr<istream> input; 
  long long entries = 0, cnt =1;
  char c, t;
  

/* Parse input options */
  arg = SetOptions(argc,argv);
  string file = arg["input"].as<string>();
  entries = static_cast<long long>(arg["fasta"].as<int>());


  try{
    if ( file.compare("-") == 0 || file.compare("") == 0 ){
      input.reset(&cin, noop());
    }else{
      input.reset(new ifstream(file.c_str()));
    }

    while ((c=(*input).get())>0){
      if (c == '>' && t == '\n')
        cnt++;
      if(cnt > entries)
        break;
      cout << c;
      t = c;
    }
    cout << endl;


  }catch(runtime_error&e){
    cerr << e.what() << "\n";
  } 

  return 0;
}

