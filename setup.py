import setuptools
import sys
from Cython.Build import cythonize
import numpy

ext_modules = [
    setuptools.Extension(
        "minfo.mi_float", 
        sources=["src/minfo.pyx", "src/mutual_info.cpp"], 
        language='c++',
        extra_compile_args=['-O2', '-std=c++11', '-fopenmp'],
            # + ['-fopenmp=libomp5' if sys.platform.startswith('darwin') else '-fopenmp'],
        include_dirs=['include/', numpy.get_include()]
    )
]

setuptools.setup(
    name="minfo",
    author="Kyle Chen",
    author_email="kchen513@sjtu.edu.cn",

    version="0.0.4",
    url="https://github.com/NeoNeuron/minfo",

    description="Python(Cython)-based mutual information estimator with adaptive partitioning strategy.",

    install_requires=['numpy', 'cython'],
    packages=setuptools.find_packages(),
    ext_modules = cythonize(ext_modules),
)