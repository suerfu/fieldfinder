#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "TRandom2.h"
#include "TError.h"
#include <iostream>
 
class RosenBrock
 {
public:
    double operator()(const double *xx){
   const Double_t x = xx[0];
   const Double_t y = xx[1];
   const Double_t tmp1 = y-x*x;
   const Double_t tmp2 = 1-x;
   return 100*tmp1*tmp1+tmp2*tmp2;
   }
 };

int main(){
    const char *minName = "Minuit";
    const char *algoName = "Simplex";
    //const char *algoName = "";
    int randomSeed = -1;
    
    // create minimizer giving a name and a name (optionally) for the specific
    // algorithm
    // possible choices are:
    //     minName                  algoName
    // Minuit /Minuit2             Migrad, Simplex,Combined,Scan  (default is Migrad)
    //  Minuit2                     Fumili2
    //  Fumili
    //  GSLMultiMin                ConjugateFR, ConjugatePR, BFGS,
    //                              BFGS2, SteepestDescent
    //  GSLMultiFit
    //   GSLSimAn
    //   Genetic
    ROOT::Math::Minimizer* minimum =
       ROOT::Math::Factory::CreateMinimizer(minName, algoName);
 
    // set tolerance , etc...
    minimum->SetMaxFunctionCalls(100000000); // for Minuit/Minuit2
    minimum->SetMaxIterations(10000);  // for GSL
    minimum->SetTolerance(0.001);
    minimum->SetPrintLevel(2);
 
    RosenBrock rb;
    // create function wrapper for minimizer
    // a IMultiGenFunction type
    ROOT::Math::Functor f( rb,2);
    double step[2] = {0.01,0.01};
    // starting point
 
    double variable[2] = { -1.,1.2};
    if (randomSeed >= 0) {
       TRandom2 r(randomSeed);
       variable[0] = r.Uniform(-20,20);
       variable[1] = r.Uniform(-20,20);
    }
 
    minimum->SetFunction(f);
 
    // Set the free variables to be minimized !
    minimum->SetVariable(0,"x",variable[0], step[0]);
    minimum->SetVariable(1,"y",variable[1], step[1]);
 
    // do the minimization
    minimum->Minimize();
 
    const double *xs = minimum->X();
    std::cout << "Minimum: f(" << xs[0] << "," << xs[1] << "): "
              << minimum->MinValue()  << std::endl;
 
    // expected minimum is 0
    if ( minimum->MinValue()  < 1.E-4  && f(xs) < 1.E-4)
       std::cout << "Minimizer " << minName << " - " << algoName
                 << "   converged to the right minimum" << std::endl;
    else {
       std::cout << "Minimizer " << minName << " - " << algoName
                 << "   failed to converge !!!" << std::endl;
       Error("NumericalMinimization","fail to converge");
    }
 
    return 0;
 }
