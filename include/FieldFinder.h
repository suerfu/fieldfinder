#ifndef FIELDFINDER_H
#define FIELDFINDER_H 1

#include <iostream>
#include <string>
#include <vector>

#include "FieldInterpolator.h"

#include "TFile.h"
#include "TH2F.h"
#include "TF2.h"

using namespace std;

class FieldFinder{

public:

    //! Constructor.
    FieldFinder( string filename, string objname){
        TFile file( filename.c_str() );
        prob = (TH2F*) file.Get( objname.c_str());
    }

    ~FieldFinder(){}
        //!< Destructor.

    //! To use Simplex method, the easiest way is to make it into a ROOT function and call the fit routine.
    void FindField( FieldInterpolator* intpl, vector<double> p ){

        // If size of parameters is less than the number of templates, initialize to 0.
        if( p.size()<intpl->GetDOF() ){
            cerr << "Not enough parameters. Initializing additional parameters to 0." << endl;
            while( p.size()!=intpl->GetDOF() ){
                p.push_back(0);
            }
        }
        // Define a 2D function object
        // The function range should be customized later.
        cout << "Creating 2D function object with " << intpl->GetDOF() << " templates...\n";
        TF2* field = new TF2("field", *intpl, -10, 10, -10, 10, intpl->GetDOF() );

        double par[9] = {1,1,1,1,1,1,1,1,1};
        // Set the parameters and perform the fit.
//        field->SetParameters( &p[0] );
        field->SetParameters( par );
        cout << "Fitting...\n";
        prob->Fit("field");
    }

private:

    TH2F* prob;
        //!< Profile likelihood map for the Kr data.
};


#endif
