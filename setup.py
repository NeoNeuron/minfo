import setuptools
import sys
from Cython.Build import cythonize
import numpy
import sysconfig

extra_compile_args = sysconfig.get_config_var('CFLAGS')
if extra_compile_args is not None:
    extra_compile_args = extra_compile_args.split()
    extra_compile_args += ['-O2', '-std=c++11', '-fopenmp']
else:
    extra_compile_args = ['-O2', '-std=c++11', '-fopenmp']

ext_modules = [
    setuptools.Extension(
        "minfo.mi_c", 
        sources=["src/minfo.pyx", "src/mutual_info.cpp"], 
        language='c++',
        extra_compile_args= extra_compile_args,
            # + ['-fopenmp=libomp5' if sys.platform.startswith('darwin') else '-fopenmp'],
        include_dirs=['include/', numpy.get_include()]
    )
]

setuptools.setup(
    name="minfo",
    author="Kai Chen",
    author_email="kchen513@sjtu.edu.cn",

    version="0.0.7",
    url="https://github.com/NeoNeuron/minfo",

    description="Python-based mutual information estimator, designed for multiple data types.",

    install_requires=['numpy', 'cython', 'numba'],
    packages=setuptools.find_packages(),
    ext_modules = cythonize(ext_modules),
)