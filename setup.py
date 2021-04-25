import setuptools
from Cython.Build import cythonize
import numpy

ext_modules = [
    setuptools.Extension(
        "minfo", 
        sources=["src/minfo.pyx", "src/mutual_info.cpp"], 
        language='c++',
        extra_compile_args=['-O2', '-std=c++11'],
        include_dirs=['include/', numpy.get_include()]
    )
]

setuptools.setup(
    name="minfo",
    author="Kyle Chen",
    author_email="kchen513@sjtu.edu.cn",

    version="0.0.1",
    url="https://github.com/NeoNeuron/mutual_information",

    description="Python(Cython)-based mutual information estimator with adaptive partitioning strategy.",

    install_requires=['numpy', 'cython'],
    packages=setuptools.find_packages(),
    ext_modules = cythonize(ext_modules),
)