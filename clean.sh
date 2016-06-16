#!/bin/bash
rm *.o 2> /dev/null
rm _*.so 2> /dev/null
rm matlabnoise.py 2> /dev/null
rm *.pyc 2> /dev/null
rm matlabnoise_wrap.cxx 2> /dev/null
rm *.mexw64 2> /dev/null
rm *.mexa64 2> /dev/null
rm -r __pycache__ 2> /dev/null
echo "All Clean!"
