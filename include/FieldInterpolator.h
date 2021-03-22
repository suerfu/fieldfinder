#ifndef FIELDINTERPOLATOR_H
#define FIELDINTERPOLATOR_H 1

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class ElectricField{

public:
    
    ElectricField( double a, double b ): centerx(a), centery(b){}

    ~ElectricField(){}

    double operator()( double* x ){ return -exp( -(x[0]-centerx)*(x[0]-centerx)/2-(x[1]-centery)*(x[1]-centery)/2);}
        // For test purpose evaluate a 2D Gaussian.

private:

    double centerx;
    double centery;
};

class FieldInterpolator{

public:

    FieldInterpolator( vector<ElectricField> f ){ fields = f;}
        //!< Constructor.
        //!< In the implementation ElectricField should be changed to actual field object.

    ~FieldInterpolator(){}
        //!< Destructor.

    unsigned int GetDOF(){ return fields.size();}
        //!< Returns the number of field templates to be used.

    //unsigned int GetDimension(){ return 2;}
        //!< Dimension of the field. 2D for now.

    //! Evaluate the interpolated field at location X.
    double operator()( double* x, double* p){
        cout << "Evaluating at " << x[0] << ", "<< x[1] << endl;;

        double f = 0;

        for( unsigned int i=0; i<fields.size(); i++){
            f += p[i] * fields[i](x);
        }

        return f;
    }

private:

    vector<ElectricField> fields;
        //!< Vector object to hold electric field templates.
};


#endif
