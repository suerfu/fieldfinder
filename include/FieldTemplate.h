#ifndef FIELDTEMPLATE_H
#define FIELDTEMPLATE_H 1

using namespace std;

//! Abstract class for electric field template. 
//! This class should return the likelihood for the specified location.

class FieldTemplate{

public:
    
    //! Constructor.
    //! Currently it uses a 1D Gaussian as a placeholder. The argument is the center of the Gaussian.
    FieldTemplate( double a ): center(a){}

    //! Destructor.
    ~FieldTemplate(){}

    //! Field evaluation. This function should return the likelihood value at the location specified by x.
    double operator()( double* x ){
        return 0;
    }

    //! Mockup function.
    double operator()(){
        return center;
    }

private:
    
    double center;
};

#endif
