#include "FieldInterpolator.h"

#include "TRandom2.h"

#include <string>
#include <cmath>

int main(){

    string minName = "Minuit";
    string algName = "Simplex";

    // Degree of freedom. This is the number of independent variables to minimize.
    const unsigned int dof = 5;

    // Random seed. Will be used to set randomly the centers of each Gaussian as well as starting point.
    int randomSeed = 0;
    TRandom2 rand(randomSeed);

    // Position of the global minium.
    double center[dof];
    
    // Create the field interpolator object with the 5 Gaussian functions.
    FieldInterpolator fieldman;
    vector<double> var;
    vector<double> step;

    for( unsigned int i=0; i<dof; i++ ){

        center[i] = rand.Uniform(-5,5);

        fieldman.LoadField( FieldTemplate( center[i]) );
        var.push_back( center[i]+rand.Uniform(-2,2) );
        step.push_back( 0.01 );
    }

    // Set the initial values of the independent variables and the stepsize.
    fieldman.SetVariables( var );
    fieldman.SetStepSizes( step );
    fieldman.SetPrintLevel( 1 );
 
    // Actual function call to minimize the multi-dim Gaussian.
    cout << "Using:            " << algName << " from " << minName << endl;
    fieldman.Minimize( minName, algName);

    var = fieldman.GetVariables();

    cout << "Computed minimum: ";
    for( unsigned int i=0; i<dof; i++){
        cout << var[i] << " ";
    }
    cout << endl;

    cout << "Actual minimum:   ";
    for( unsigned int i=0; i<dof; i++){
        cout << center[i] << " ";
    }
    cout << endl;

    cout << "Distance:         ";
    double diff = 0;
    for( unsigned int i=0; i<dof; i++){
        diff += (center[i]-var[i]) * (center[i]-var[i]);
    }
    cout << sqrt(diff) << endl;

    return 0;
}
