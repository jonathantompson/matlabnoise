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
    mexErrMsgIdAndTxt("MATLAB:Hermite2D:invalidNumInputs", 
      "Input must be X, Y");
  }
  
  // input must be X, Y
  for (int i = 0; i < 2; i++) {
    if (mxIsDouble(prhs[i]) != 1) {
      mexErrMsgIdAndTxt("MATLAB:Hermite2D:notDouble",
        "Inputs must be double.");
    }
  }
  
  // Check that the inputs are the same dimension
  for (int i = 1; i < 2; i++) {
    if (mxGetNumberOfDimensions(prhs[i]) != mxGetNumberOfDimensions(prhs[0]) ||
        mxGetNumberOfElements(prhs[i]) != mxGetNumberOfElements(prhs[0])) {
      mexErrMsgIdAndTxt("MATLAB:Hermite2D:badSize",
        "Inputs must be the same size.");
    }
    size_t K = mxGetNumberOfDimensions(prhs[i]);
    const mwSize* Ni = mxGetDimensions(prhs[i]);
    const mwSize* N0 = mxGetDimensions(prhs[0]);
    for (size_t j = 0; j < K; j++) {
      if (Ni[j] != N0[j]) {
        mexErrMsgIdAndTxt("MATLAB:Hermite2D:badSize",
          "Inputs must be the same size.");
      }
    }
  }
  
  // Only 1 input allowed
  if (nlhs != 1) {
    mexErrMsgIdAndTxt("MATLAB:Hermite2D:invalidNumOutputs", 
      "One output is required");
  }  
  
  // Allocate the output
  mxArray* fout = mxDuplicateArray(prhs[0]);
  plhs[0] = fout;
  
  size_t npts = mxGetNumberOfElements(prhs[0]);
  double* X = mxGetPr(prhs[0]);
  double* Y = mxGetPr(prhs[1]);
  double* out = mxGetPr(fout);
  
  for (size_t i = 0; i < npts; i++) {
    out[i] = Hermite2D(vec2(X[i], Y[i]));
  }
}