from skbuild import setup

setup(
    name="finmath",
    version="0.1.1",
    description="Python Financial Mathematics library written in C/C++",
    author="Prajwal Shah, .. , Boiler Quant",
    license="AGPL",
    packages=["finmath"],
    cmake_source_dir=".",
    cmake_install_dir="finmath"
)
