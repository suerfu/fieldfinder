
#include <iostream>
#include <cmath>

#include "FieldInterpolator.h"

#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"

using namespace std;


FieldInterpolator::FieldInterpolator( ){
    // Set the parameters to default values.
    SetMaxIterations();
    SetTolerance();
    SetPrintLevel();
}


FieldInterpolator::~FieldInterpolator(){}


void FieldInterpolator::LoadField( FieldTemplate f){
    fields.push_back(f);
}


void FieldInterpolator::LoadField(/*Actual field object goes here*/){}

int FieldInterpolator::LoadKrData( string filename, string histname){
    
    TFile file( filename.c_str(), "READ");
    if( !file.IsOpen() ){
        cerr << "Could not open " << filename << endl;
        return -1;
    }
    
    krData = reinterpret_cast<TH2F*>( file.Get( histname.c_str() ) );
    if( !krData){
        cerr << "Could not find TH2F object named " << histname << endl;
        return -2;
    }

    return 0;
}


double FieldInterpolator::ComputeLikelihood( const double* x){
        
    double exponent = 0;
    for( unsigned int i=0; i<GetDOF(); i++){
        exponent += -0.5*( x[i]-fields[i]() ) * ( x[i]-fields[i]() );
    }
    return -exp(exponent);

    /* actual code should be something like
    finfield = 0;
    for( unsigned int i=0; i<GetDOF(); i++){
        finfield += x[i] * fieldtemplate[i] );
    }
    return GetLikelihood( finfield, krData);
    */
}

    
void FieldInterpolator::Minimize( string minName, string algoName){
        
    // Initialize ROOT minimizer object
    ROOT::Math::Minimizer* minimizer = ROOT::Math::Factory::CreateMinimizer(minName.c_str(), algoName.c_str());
    // Set a number of required parameters
    minimizer->SetMaxFunctionCalls( maxIteration );
    minimizer->SetMaxIterations( maxIteration );
    minimizer->SetTolerance( tolerance );
    minimizer->SetPrintLevel( printLevel );

    // Create a callable function pointer
    // Argument is C++ object and dimension/dof.
    ROOT::Math::Functor f( *this, GetDOF() );

    // Set function to minimize.
    minimizer->SetFunction( f );

    // Initialize variables and their step sizes.
    for( unsigned int i=0; i<GetDOF(); i++ ){
        stringstream varName;
        varName << "x" << i;
        minimizer->SetVariable( i, varName.str().c_str(), variables[i], steps[i]);
    }

    // Do the actual minimization.
    minimizer->Minimize();

    /*
    cout << "Minimum: f( ";
    const double *xmin = minimizer->X();
    for( unsigned int i=0; i<GetDOF(); i++){
        cout << xmin[i] << " ";
    }
    cout << ") : " << minimizer->MinValue() << endl;
    */

    // Store the information about the minimum 
    minValue = minimizer->MinValue();
    const double *xmin = minimizer->X();
    for( unsigned int i=0; i<GetDOF(); i++){
        variables[i] = xmin[i];
    }
}

