#!/bin/bash

# Jonathan Tompson 3/16/2015

echo compiling the mex files...
matlab -nodisplay -nosplash -nodesktop -r "run('compile_mex.m'); exit();"

echo 'All done!'
