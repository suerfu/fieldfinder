#ifndef FIELDINTERPOLATOR_H
#define FIELDINTERPOLATOR_H 1

#include <iostream>
#include <vector>
#include <cmath>

#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
//#include "TError.h"
#include <iostream>

using namespace std;

//! Abstract class for electric field templates. 
class ElectricField{

public:
    
    //! Constructor. Currently it's using a 2D Gaussian as a placeholder.
    ElectricField( double a ): center(a){}

    ~ElectricField(){}

    double operator()(){ return center;}
        // For test purpose evaluate a 2D Gaussian.

private:
    
    double center;
};

class FieldInterpolator{

public:

    FieldInterpolator( ){}
        //!< Default constructor.

    ~FieldInterpolator(){}
        //!< Destructor.
    
    void LoadField( ElectricField f){ fields.push_back(f);}
        //!< Add electric field templates.

    unsigned int GetDOF(){ return fields.size();}
        //!< Returns the number of field templates to be used.

    //! Evaluate the interpolated field at location X.
    //! The number of elements pointed to by x should be the same as the number of field templates.
    //! It is a wrapper for ComputeLikelihood. This wrapper is required for ROOT's minimizer template.
    double operator()( double* x ){
        return ComputeLikelihood( x );
    }

    //! Compute the likelihood between observed Kr83 events and the linear combination of electric fields for the given sets of parameters.
    //! For now this is a mock-up function that returns ND Gaussian whose center is the respective center of each fields.
    double ComputeLikelihood( double* x){
        
        double exponent = 0;
        for( unsigned int i=0; i<fields.size(); i++){
            exponent += -0.5*( x[i]-fields[i]() ) * ( x[i]-fields[i]() );
        }
        return exp(exponent);
    }

    //! Actual function that does the minimization.
    void Minimize( const char* minName = "Minuit", const char* algoName = "Simplex"){
        
        // Initialize ROOT minimizer object
        ROOT::Math::Minimizer* minimizer = ROOT::Math::Factory::CreateMinimizer(minName, algoName);
        // Set a number of required parameters
        minimizer->SetMaxIterations( maxIteration );
        minimizer->SetTolerance( tolerance )
        minimizer->SetPrintLevel( printLevel );

        // Create a callable function pointer
        // Argument is C++ object and dimension/dof.
        ROOT::Math::Functor f( *this, fields.size() );

        // Set function to minimize.
        minimizer->SetFunction( f );

        // Initialize variables and their step sizes.
        for( unsigned int i=0; i<fields.size(); i++ ){
            stringstream varName;
            varName << "x" << i;
            minimizer->SetVariable( i, varName.str().c_str(), variables[i], steps[i]);
        }

        // Do the actual minimization.
        minimizer->Minimize();

        cout << "Minimum: f( ";
        const double *xmin = minimizer->X();
        for( unsigned int i=0; i<fields.size(); i++){
            cout << xmin[i] << " ";
        }
        cout << ") : " << minimizer->MinValue() << endl;
    }

private:

    vector<ElectricField> fields;
        //!< Vector object to hold electric field templates.

    vector<double> variables;

    vector<double> steps;

    unsigned int maxIteration;

    float tolerance;

    int printLevel;
};


#endif
