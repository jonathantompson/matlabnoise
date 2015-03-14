#include <stdio.h>
#include <string>
#include "noise_common.h"
#include "stdint.h"
#include "mex.h"

using namespace std;

// The gateway function
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
  // Only 2 inputs allowed
  if (nrhs != 2) {
    mexErrMsgIdAndTxt("MATLAB:Perlin2DDeriv:invalidNumInputs", 
      "Input must be X, Y");
  }
  
  // input must be X, Y
  for (int i = 0; i < 2; i++) {
    if (mxIsDouble(prhs[i]) != 1) {
      mexErrMsgIdAndTxt("MATLAB:Perlin2DDeriv:notDouble",
        "Inputs must be double.");
    }
  }
  
  // Check that the inputs are the same dimension
  for (int i = 1; i < 2; i++) {
    if (mxGetNumberOfDimensions(prhs[i]) != mxGetNumberOfDimensions(prhs[0]) ||
        mxGetNumberOfElements(prhs[i]) != mxGetNumberOfElements(prhs[0])) {
      mexErrMsgIdAndTxt("MATLAB:Perlin2DDeriv:badSize",
        "Inputs must be the same size.");
    }
    size_t K = mxGetNumberOfDimensions(prhs[i]);
    const mwSize* Ni = mxGetDimensions(prhs[i]);
    const mwSize* N0 = mxGetDimensions(prhs[0]);
    for (size_t j = 0; j < K; j++) {
      if (Ni[j] != N0[j]) {
        mexErrMsgIdAndTxt("MATLAB:Perlin2DDeriv:badSize",
          "Inputs must be the same size.");
      }
    }
  }
  
  // Only 1 input allowed
  if (nlhs != 2) {
    mexErrMsgIdAndTxt("MATLAB:Perlin2DDeriv:invalidNumOutputs", 
      "Two outputs are required");
  }  
  
  // Allocate the output
  size_t K = mxGetNumberOfDimensions(prhs[0]);
  const mwSize* Ni = mxGetDimensions(prhs[0]);
  size_t Kout = K + 1;
  mwSize* Nout = (mwSize*)mxCalloc(Kout, sizeof(Ni[0]));
  Nout[0] = 2;  // (ds/dx, ds/dy)
  for (int i = 0; i < K; i++) {
    Nout[i+1] = Ni[i];
  }
  
  mxArray* fout = mxDuplicateArray(prhs[0]);
  mxArray* dfout = mxCreateNumericArray(Kout, Nout, mxGetClassID(prhs[0]), 
          mxIsComplex(prhs[0]) ? mxCOMPLEX : mxREAL);
  plhs[0] = fout;
  plhs[1] = dfout;
  
  size_t npts = mxGetNumberOfElements(prhs[0]);
  double* X = mxGetPr(prhs[0]);
  double* Y = mxGetPr(prhs[1]);
  double* out = mxGetPr(fout);
  double* dout = mxGetPr(dfout);
  
  // If an array is allocated:
  // mwSize dims[3] = {7, 8, 9};
  // mxCreateNumericArray(3, dims, mxDOUBLE_CLASS, mxREAL);
  // Then you index it by (m,n,p):
  // data[(m-1)+(n-1)*7+(p-1)*7*8] = 1;
  
  for (size_t i = 0; i < npts; i++) {
    vec3 s_and_deriv = Perlin2DDeriv(vec2(X[i], Y[i]));
    out[i] = s_and_deriv.x;
    dout[i*2] = s_and_deriv.y;
    dout[i*2+1] = s_and_deriv.z;
  }
  
  mxFree(Nout);
}