#ifndef FIELDINTERPOLATOR_H
#define FIELDINTERPOLATOR_H 1

#include <iostream>
#include <vector>

#include "FieldTemplate.h"

#include "TFile.h"
#include "TH2F.h"

#include <string>

using namespace std;


class FieldInterpolator{

public:

    FieldInterpolator( );
        //!< Default constructor.

    ~FieldInterpolator();
        //!< Destructor.
    
    void LoadField( FieldTemplate f);
        //!< Add electric field templates.
        //!< This method is used for demo.

    void LoadField(/*Actual field object goes here*/);
        //!< Method to load field template.

    int LoadKrData( string filename, string histname);
        //!< This method is used to load the actual calibration data.
        //!< Let's assume it's a root file and the data is accessible as a TH2F object.
        //!< Returns 0 upon success. Negative number upon failure. This can also be handled with exceptions.

    unsigned int GetDOF(){ return fields.size();}
        //!< Returns the number of field templates to be used.


    //! Most crucial operator for the minimizer routine. This wrapper is required for ROOT's minimizer template.
    //! It is a wrapper for ComputeLikelihood. 
    //! In the current mock-up example, number of x should be equal to number of templates.
    double operator()( const double* x ){
        return ComputeLikelihood( x );
    }


    //! Compute the likelihood between observed Kr83 events and the linear combination of electric fields for the given sets of parameters.
    //! For now this is a mock-up function that returns ND Gaussian whose center is the respective center of each fields.
    double ComputeLikelihood( const double* x);


    //! Actual minimization routine.
    //! It takes as argument the Minimization library (default Minuit) and algorithm.
    //! Possible choices are:
    //!   minName             algoName
    //! Minuit /Minuit2    Migrad,Simplex,Combined,Scan(default is Migrad if not specified.)
    //! Minuit2            Fumili2
    //! Fumili
    //! GSLMultiMin        ConjugateFR, ConjugatePR, BFGS, BFGS2, SteepestDescent
    //! GSLMultiFit
    //! GSLSimAn
    //! Genetic
    void Minimize( string minName = "Minuit", string algoName = "Simplex");


    //! Initialize variables.
    void SetVariables( vector<double> v){ variables = v; }

    //! Get the current variable.
    //! After minimization, this method can be called to check the coordinate of minimum.
    vector<double> GetVariables(){ return variables; }


    //! Set the current stepsize.
    void SetStepSizes( vector<double> v){ steps = v; }

    //! Get the current stepsize.
    vector<double> GetStepSizes(){ return steps; }


    //! Set the maximum number of iterations.
    //! For certain algorithms this will be the max number of function calls (i.e. Simplex).
    void SetMaxIterations( unsigned int m = 1000000 ){ maxIteration = m; }

    //! Get the maximum number of iterations.
    unsigned int GetMaxIterations(){ return maxIteration; }

    
    //! Set the required tolerance.
    void SetTolerance( float a = 0.001 ){ tolerance = a; }

    //! Get the current tolerance.
    float GetTolerance(){ return tolerance;}


    //! Set the verbosity level.
    void SetPrintLevel( int a = 0 ){ printLevel = 0; }

    //! Get the verbosity level.
    int GetPrintLevel(){ return printLevel; }


private:

    vector<FieldTemplate> fields;
        //!< Vector object to hold electric field templates.
        //!< This should be replaced with actual field objects.

    vector<double> variables;

    double minValue;

    vector<double> steps;

    unsigned int maxIteration;

    float tolerance;

    int printLevel;

    TH2F* krData;
};


#endif
