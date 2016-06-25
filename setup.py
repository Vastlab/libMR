from distutils.core import setup
from distutils.extension import Extension
import sys

try:
    from Cython.Build import cythonize
    import numpy
except ImportError:
    print "You must have Cython >=0.17 and NumPy to build LibMR's python bindings!"
    sys.exit(1)

# Needed to fix pip
# See https://pypi.python.org/pypi/setuptools_cython/,
# http://mail.python.org/pipermail/distutils-sig/2007-September/thread.html#8204
if 'setuptools.extension' in sys.modules:
    m = sys.modules['setuptools.extension']
    m.Extension.__dict__ = m._Extension.__dict__

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
      version = "0.1.0",
      description="Python bindings for LibMR, the metarecognition library",
      url="http://www.metarecognition.com/ml-mr-2/",
      license="http://www.metarecognition.com/libmr-license/",
      author='Terry Boult',
)
