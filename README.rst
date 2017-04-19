Synopsis
========

libMR provides core MetaRecognition and  Weibull fitting functionality.
It is used to calculate w-scores used for multi-modal fusion, renormalize SVM data and in general support open-set algorithms with outlier detection


Motivation
==========

Determine when something is an outlier is tricky business.
Luckily extreme value theory provides a strong basis for modeling of the largest or smallest known values and then determing if something else is too large or too small. This library supports such computations.
It is also easy to do the work in almost any advanced package (R, Matlab, etc.) but it is trick to keep it all straight.
The C++ classes here track translation and flipping to make it easier to correctly use the meta-recognition concepts.


License
=======

This version libMR is released under the BSD 3-Clause license. (see License.txt) and superseed previous license.  We dropped pursing the patent protection.

Please cite libMR in your publications if it helps your research::

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



This version uses object, that retain the "transforms" (including save/load) need to compute the actual w-scores.
It includes functions  for Prediction, CDF,  Inverse, FitHigh (larger is better), FitLow (smaller is better) and FitSVM.

Installation
============

Project layout
--------------

* libMR      Library source code and resources
* python     A python wrapper for libMR using Cython
* examples   Directory with example programs that test that also test for correctness
* doc        C++ documentation; directory starts at html/index.html



C++ library
-----------

This needs CMake (version > 2.8)

Quick build instructions for Windows::

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

Quick build instructions for linux/Mac::

  $ tar -zxf libMR.tgz
  $ cd libMR
  $ mkdir build;  % or whereyou like to build it
  $ cd build
  $ cmake ..
  or to get a different installation directory
  $ cmake -DCMAKE_INSTALL_PREFIX=<YOUR_INSTALLATON_PATH> ..
  $ make

Running examples - Run after installing libMR::

  $ examples/mr-test
  $ examples/wlb-test


Optionally you can "install" it (by default in the system path -- there you have to use sudo)::

  $ make install

There are built in tests in examples/mr-test to ensure it compiled correctly and is computing proper values



Python
------

Python bindings to LibMR are provided by Cython >=0.17. When building
this way, LibMR is statically linked into the Python library which
means you don't have to build the entire package.

There are several ways to install LibMR for Python.

1. (preferred) Installing with Pip:

   This method will build LibMR as a Python extension and will install
   it into your python path. If you are inside a virtualenv, this
   allows you to install LibMR as a standard user without needing
   sudo::

     $ virtualenv venv && source venv/bin/activate
     $ pip install -r python/requirements.txt
     $ pip install .

   Update: libmr is now available on PyPI. To install the latest version,
   you can simply::

     $ pip install libmr

2. Building a shared object

   If you have Cython and NumPy already installed but you do not want
   to install LibMR into Python's path, you can simply build the
   module directly::

     $ python setup.py build_ext -i

   This will place 'libmr.so' into the current folder. You can import
   it directly::

     $ file libmr.so
     libmr.so: ELF 64-bit LSB  shared object, ...
     $ python
     >>> import libmr
     >>> mr = libmr.MR()

   Now, move ``libmr.so`` into your project's folder, or add this path into your ``PYTHONPATH`` environment variable.


Contributors
============

Developed by Dr. Terrance (Terry) Boult,  Dr. Walter Scheir   with some implemntation by Vijay Iyer @Securics and Enhancements for Python by Ethan Rudd and Dr. Manuel Gunther
