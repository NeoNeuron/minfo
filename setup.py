from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
import numpy

ext_modules = [Extension("mutual_info_cy", 
               sources=["mutual_info_cy.pyx", "mutual_info.cpp"], 
               language='c++',
               extra_compile_args=['-O2'],
               include_dirs=[numpy.get_include()])
              ]

setup(cmdclass = {'build_ext': build_ext}, ext_modules = ext_modules)