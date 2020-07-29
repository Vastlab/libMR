libMR provides core MetaRecognition and  Weibull fitting functionality.
It is used to calculate w-scores used for multi-modal fusion.

This version libMR is released under the BSD 3-Clause license. (see License.txt) and superseed previous license.  We dropped pursing the patent protection. 

Please cite libMR in your publications if it helps your research:


@article{Scheirer_2011_TPAMI,
author = {Walter J. Scheirer and Anderson Rocha and Ross Michaels and Terrance E. Boult},
title = {Meta-Recognition: The Theory and Practice of Recognition Score Analysis},
journal = {IEEE Transactions on Pattern Analysis and Machine Intelligence (PAMI)},
volume = {33},
issue = {8},
pages = {1689--1695},
year = {2011}
}    

or one of our later works if combining it with them.  


************************************************************************************************************************

Changelog

Version 1.2      This version also exports the computed confidence intervals via the python interface.  Fixed broken wbl-test and added python-example that tests new interface funcions and does not need plotting.


Version 1.1
Updated license
This version uses object, that saves the "transforms" (including save/load) need to compute the actual w-scores in binary save form

It includes functions  for Prediction, CDF,  Inverse, FitHigh (larger is better), FitLow (smaller is better) and FitSVM.




***************************************************************************************************************
This needs CMake (version > 2.8)

Quick build instructions for Windows:
use CMake GUI
Source code = <where libmr is> libMR/libMR
Where to build binaries = < where libmr is > /libMR/build
Click generate
Select correct visual studio version compiler
Click yes to add directory build
Open Visual Studio Solution
Build ALL_BUILD project to build libmr
Build INSTALL project to install
Add install dir to path to link against libmr dll.

Quick build instructions for linux/Mac:
$> tar -zxf libMR.tgz
$> cd libMR
%You can edit the CMakeLists.txt in libMR level, to change install dir if desired
$> mkdir build;  % or whereyou like to build it
$> cd build
$> cmake -DCMAKE_BUILD_TYPE=Debug ..
$> make

Running examples - Run after installing libMR
$>  examples/mr-test
$>  examples/wlb-test


Optionally  you can "install" wherever the CMAKE file,
$> sudo make install


Project layout:
 * libMR      Library source code and resources
 * python     A python wrapper for libweibull using Cython
 * examples   Directory with example programs that test that also test for correctness
 * doc        Documentation directory  starts at html/index.html


Python bindings to LibMR are provided by Cython >=0.17. When building
this way, LibMR is statically linked into the Python library which
means you don't have to build the entire package.

There are several ways to install LibMR for Python:

1. (preferred) Installing with Pip

   This method will build LibMR as a Python extension and will install
   it into your python path. If you are inside a virtualenv, this
   allows you to install LibMR as a standard user without needing
   sudo.

      $ 
      $ pip install -r python/requirements.txt
      $ pip install .

2. Building a shared object

   If you have Cython and NumPy already installed but you do not want
   to install LibMR into Python's path, you can simply build the
   module directly:

      $ python setup.py build_ext -i

   This will place 'libmr.so' into the current folder. You can import
   it directly:

      $ file libmr.so
      libmr.so: ELF 64-bit LSB  shared object, ...
      $ python
      >>> import libmr
      >>> mr = libmr.MR()

   Now, move libmr.so into your project's folder.
