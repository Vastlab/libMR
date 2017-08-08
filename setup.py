from setuptools import setup
from setuptools.extension import Extension
import numpy

try:
    from Cython.Build import cythonize
except ImportError:
    raise ImportError("You must have Cython >=0.17 to build LibMR's python bindings!")

setup(name='libmr',
      ext_modules = __import__("Cython").Build.cythonize(Extension('libmr',[
                            "python/libmr.pyx",
                            "libMR/MetaRecognition.cpp",
                            "libMR/weibull.c"
                      ],
                      include_dirs = ["libMR/", numpy.get_include()],
                      language="c++",
                  )),
      data_files = [('libMR', ['libMR/MetaRecognition.h', 'libMR/weibull.h'])],
      version = "0.1.8",
      description="LibMR, the MetaRecognition library",
      long_description=open("README.rst").read(),
      url="https://github.com/Vastlab/libMR",
      license="http://www.metarecognition.com/libmr-license/",
      author='Terry Boult, Ethan Rudd, and Manuel Gunther',
      install_requires=['cython','numpy']
)
