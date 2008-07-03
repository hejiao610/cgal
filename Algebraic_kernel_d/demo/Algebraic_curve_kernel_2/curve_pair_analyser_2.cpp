// TODO: Add licence
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL:$
// $Id: $
// 
//
// Author(s)     : Michael Kerber <mkerber@mpi-inf.mpg.de>
//
// ============================================================================

// Allow only coprime curve pairs for that demo!
#define CGAL_ACK_DONT_CHECK_POLYNOMIALS_FOR_COPRIMALITY 1

#include<CGAL/Algebraic_curve_kernel_2/flags.h>

#include <CGAL/basic.h>

// demo-specific flags

// Bitsize of coefficients for random polynomials
#define COEFFICIENT_BITSIZE 8

// What is the coefficient type of the input?
#ifndef CGAL_ACK_COEFFICIENT
#define CGAL_ACK_COEFFICIENT CGAL::CORE_arithmetic_kernel::Integer
#define CGAL_ACK_COEFFICIENT_IS_INTEGER 1
#endif

#include <sstream>

#include <CGAL/Timer.h>
CGAL::Timer overall_timer;
CGAL::Timer first_curve_timer, second_curve_timer,
  pair_timer;

#include <CGAL/Arithmetic_kernel.h>
#include <CGAL/Preferred_algebraic_curve_kernels_2.h>

void print_help(char* execname) {
  std::cout << "Usage: " << execname 
	    << " (RANDOM n m)|(FILE filename)|(INPUT input1 input2)"
	    << std::endl << std::endl
	    << "if RANDOM is used, random bivariate polynomials of bidegree "
	    << "n an m are generated and the curve pair is analyzed." 
            << std::endl
	    << "if FILE is used, the file filename is taken as input. It is "
	    << "expected that the file contains two curve as its head, "
            << "and the remainder of the file is ignored"
	    << std::endl
	    << "if INPUT is used, the next two argument are taken "
            << "as definition of the polynomial" 
            << std::endl << std::endl
	    << "For the options FILE and INPUT, the program expects the "
	    << "polynomials to be of the pattern P[...], according to the "
	    << "EXACUS format" << std::endl << std::endl
            << "Example: " << std::endl << "\t" <<  execname << " RANDOM 4 5 "
	    << "(analyses random curve pair of bidegrees 4 and 5)" << std::endl
	    << "\t" << execname 
	    << " INPUT \"P[2(0,P[2(0,-1)(2,1)])(2,P[0(0,1)])]\" "
            << "\"P[1(0,P[2(2,-1)])(1,P[0(0,1)])]\" "
	    << "(analyses unit circle with parabola)" << std::endl
	    << "\t" << execname << " FILE poly_pair  (analyses the polynomial "
	    << "defined in the textfile poly_pair)" << std::endl <<std::endl;

}

void reset_timers() {
  overall_timer.reset();
  first_curve_timer.reset();
  second_curve_timer.reset();
  pair_timer.reset();
}

template<class NT>
std::vector<NT> randvector(int degree,long bitsize) {
  std::vector<NT> coeffs(degree+1);
  for(int i=0;i<=degree;i++) {
    // Creating the coefficients VERY elementary...
    NT coeff=0;
    for(int j=0;j<bitsize-1;j++) {
      coeff = 2*coeff + (lrand48()%2);
    }
    // The last bit determines the sign
    if(lrand48()%2==0) {
      coeff=-coeff;
    }    
    coeffs[i]=coeff;
  }
  return coeffs;
}

int main(int argc,char** argv) {
  
  if(argc<3) {
    print_help(argv[0]);
    exit(-1);
  }

  ::CGAL::set_pretty_mode(std::cout);

  typedef CGAL_ACK_COEFFICIENT Coefficient;

  typedef CGAL::Get_algebraic_curve_kernel_2<Coefficient>
      ::Algebraic_curve_kernel_with_qir_and_bitstream_2
      Algebraic_curve_kernel_2;
  
  typedef Algebraic_curve_kernel_2::Curve_analysis_2 Curve_analysis_2;
  
  typedef Algebraic_curve_kernel_2::Curve_pair_analysis_2 
      Curve_pair_analysis_2;

  typedef Algebraic_curve_kernel_2::Polynomial_1 Polynomial_1;
  typedef Algebraic_curve_kernel_2::Polynomial_2 Polynomial_2;
  
  std::vector<std::pair<Polynomial_2,Polynomial_2> > curve_pairs;
  Polynomial_2 f,g;
  
  if(strcmp(argv[1],"FILE")!=0 
     && strcmp(argv[1],"RANDOM")!=0  
     && strcmp(argv[1],"INPUT")!=0 
     && strcmp(argv[1],"FILESEQ")!=0) {
    print_help(argv[0]);
    exit(-1);
  }

   if(strcmp(argv[1],"FILE")==0) {
    std::ifstream input(argv[2]);
    input >> f; 
    input >> g;
    curve_pairs.push_back(std::make_pair(f,g));
   }
   if(strcmp(argv[1],"RANDOM")==0) {
#if CGAL_ACK_COEFFICIENT_IS_INTEGER
     if(argc<4) {
       print_help(argv[0]);
       exit(-1);
     }
     srand48(time(NULL));
     // Create random polynomial of given degree
     int degree1 = atoi(argv[2]);
     int degree2 = atoi(argv[3]);
     std::vector<Polynomial_1> coeffs;
     for(int i=0;i<=degree1;i++) {
       std::vector<Coefficient> curr_coeffs 
	 = randvector<Coefficient>(degree1-i,COEFFICIENT_BITSIZE);
       coeffs.push_back(Polynomial_1(curr_coeffs.begin(),curr_coeffs.end()));
     }
     f=Polynomial_2(coeffs.begin(),coeffs.end());
     coeffs.clear();
     for(int i=0;i<=degree2;i++) {
       std::vector<Coefficient> curr_coeffs 
	 = randvector<Coefficient>(degree2-i,COEFFICIENT_BITSIZE);
       coeffs.push_back(Polynomial_1(curr_coeffs.begin(),curr_coeffs.end()));
     }
     g=Polynomial_2(coeffs.begin(),coeffs.end());
     curve_pairs.push_back(std::make_pair(f,g));
#else
     std::cerr << "Choosen coefficient type does not allow "
               << " creation of random numbers" << std::endl;
     std::exit(1);
#endif
   }
   if(strcmp(argv[1],"INPUT")==0) {
     if(argc<4) {
       print_help(argv[0]);
       exit(-1);
     }
     std::stringstream ss1(argv[2]);
     ss1 >> f;
     std::stringstream ss2(argv[3]);
     ss2 >> g;
     curve_pairs.push_back(std::make_pair(f,g));
   }
   for(int i=0;i<static_cast<int>(curve_pairs.size());i++) {
     f = curve_pairs[i].first;
     g = curve_pairs[i].second;
     std::cout << "Input polynomials:\n" << "f=" << f << "\ng=" << g 
               << std::endl;
     reset_timers();
     overall_timer.start();
     std::cout << "+++++++++++++++ One curve analysis starts +++++++++++" << std::endl;
     first_curve_timer.start();
     Curve_analysis_2 F(f);
     first_curve_timer.stop();
     second_curve_timer.start();
     Curve_analysis_2 G(g);
     second_curve_timer.stop();
     std::cout << "+++++++++++++++ One curve analysis ends +++++++++++" << std::endl;
     std::cout << "*************** Two  curve analysis starts ***************" << std::endl;
     pair_timer.start();
     Curve_pair_analysis_2 algebraic_curve_pair(F,G);
     pair_timer.stop();
     overall_timer.stop();   
     std::cout << "*************** Two curve analysis ends ***************" << std::endl;

     std::cout << algebraic_curve_pair << std::endl;

     std::cout << "TIMINGS: " << std::endl;
     std::cout << "Analysing 1st curve:  " << first_curve_timer.time() 
               << std::endl; 
     std::cout << "Analysing 2nd curve:  " << second_curve_timer.time() 
               << std::endl;
     std::cout << "Analysing curve pair: " << pair_timer.time() << std::endl; 
     std::cout << "----" << std::endl;
     std::cout << "Overall timer:        " << overall_timer.time() << std::endl; 

   }
   return 0;
      
}

