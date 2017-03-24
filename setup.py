from distutils.core import setup
from distutils.extension import Extension
import sys

try:
    from Cython.Build import cythonize
    import numpy
except ImportError:
    print "You must have Cython >=0.17 and NumPy to build LibMR's python bindings!"
    sys.exit(1)

with open("README.rst") as f:
    l_descrip=f.read()

setup(name='libmr',
      ext_modules = cythonize(Extension('libmr',[
                            "python/libmr.pyx",
                            "libMR/MetaRecognition.cpp",
                            "libMR/weibull.c"
                      ],
                      include_dirs = ["libMR/", numpy.get_include()],
                      language="c++",
                  )),
      data_files = [('libMR', ['libMR/MetaRecognition.h', 'libMR/weibull.h'])],
      version = "0.1.3",
      description="Python bindings for LibMR, the metarecognition library",
      long_description=l_descrip,
      url="http://www.metarecognition.com/ml-mr-2/",
      license="http://www.metarecognition.com/libmr-license/",
      author='Terry Boult, Ethan Rudd, and Manuel Gunther',
      install_requires=['cython','numpy']
      )
