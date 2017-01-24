@echo off

REM A simple batch script to compile on windows
REM Assumes that SWIG & Python are on the path, 
REM and that this is an appropriate VS command prompt

REM Find the currently active python
for /f "delims=" %%A in ('where python') do set "PYTHON_ROOT=%%~pA"
IF NOT DEFINED PYTHON_ROOT ( 
	ECHO -- Python Not Found - add the python folder to your current PATH variable
	GOTO EOF 
)
echo Found python at: %PYTHON_ROOT%

REM Are we a valid command-line of VS?
for /f "delims=" %%A in ('where cl') do set "CL_BINARY_P=%%A"
IF NOT DEFINED CL_BINARY_P ( 
	ECHO -- Visual Studio Not Found - ensure this is a VS command prompt *by running vcvarsall.bat*
	GOTO EOF 
)
echo Found Visual Studio

REM Assert swig is runnable
for /f "delims=" %%A in ('where swig') do set "SWIG_PATH=%%A"
IF NOT DEFINED SWIG_PATH ( 
	ECHO -- Swig Not Found - add the swig folder to your current PATH variable
	GOTO EOF 
	)
echo Found swig at: %SWIG_PATH%

echo Compiling python (swig) wrapper...
swig -c++ -python matlabnoise.i
cl /O2 /Ot /Ox  /EHs /LD /D__WIN32__ /fp:precise /Zi /I%PYTHON_ROOT%\include vec2.cpp vec3.cpp vec4.cpp noise_common.cpp matlabnoise_wrap.cxx /link /DLL /libpath:%PYTHON_ROOT%\libs /pdb:_matlabnoise.pdb /out:_matlabnoise.pyd
echo -- Add done! --

:EOF
