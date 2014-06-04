#ifdef _OPENMP
 #include "omp.h"
#endif

#include <iostream>
#include <sstream>
#include <iomanip> // for setprecision
#include <limits> // for min/max values
#include <assert.h>
#include <math.h>
#include <time.h>
#include <vector>

#include "MBCore.hpp"
#include "MBTagConventions.hpp"
#include "MBRange.hpp"
#include "moab/Skinner.hpp"
#include "moab/GeomTopoTool.hpp"

MBInterface *MBI();

int main(int argc, char* argv[])
{
  // create empty meshset                                                                                                                
  MBErrorCode result, rval;
  MBEntityHandle input_set;
  rval = MBI()->create_meshset( MESHSET_SET, input_set );

  // load the input file
  std::string input_name = argv[1];
  rval = MBI()->load_file( input_name.c_str(), &input_set );
  if(MB_SUCCESS == rval ) {
    std::cout << "Loading input file..." << std::endl;
  } else {
    std::cout << "failed to load input file" << std::endl;
    return 1;
  }

  int num_ents;
  #pragma omp parallel
  {
    #ifdef _OPENMP
    if(omp_get_thread_num() < 11)
      {
        rval = MBI()->get_number_entities_by_dimension(input_set,1,num_ents);
	std::cout << "Thread " << omp_get_thread_num() << "Number of 1d entities " << num_ents << std::endl;  
      }
    else
      {
        rval = MBI()->get_number_entities_by_dimension(input_set,2,num_ents);
	std::cout << "Thread " << omp_get_thread_num() << "Number of 2d entities " << num_ents << std::endl;  
      }
    #else
      rval = MBI()->get_number_entities_by_dimension(input_set,1,num_ents);
      std::cout << "Number of 1d entities " << num_ents << std::endl;  
    #endif
  } 

  return 0;
}

MBInterface *MBI() 
{
  static MBCore instance;
  return &instance;
}
