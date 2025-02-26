from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import os
import shutil


class CMakeBuildExt(build_ext):
    def build_extension(self, ext):
        # Paths
        source_dir = os.path.abspath(os.path.dirname(__file__))
        build_temp = os.path.join(source_dir, "build")
        ext_dir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))

        # Step 1: Ensure the build directory exists
        os.makedirs(build_temp, exist_ok=True)

        # Step 2: Run `cmake ..` in the build directory
        if os.system(f"cmake {source_dir} -B {build_temp}") != 0:
            raise RuntimeError("CMake configuration failed.")

        # Step 3: Run `make` (or build using CMake)
        if os.system(f"cmake --build {build_temp}") != 0:
            raise RuntimeError("CMake build failed.")

        # Step 4: Locate the generated Python module (`.so` file)
        shared_lib = os.path.join(build_temp, "finmath.cpython-312-darwin.so")  # Update based on your platform
        if not os.path.exists(shared_lib):
            raise FileNotFoundError(f"Cannot find the compiled library: {shared_lib}")

        # Step 5: Copy the `.so` file to the Python package directory
        os.makedirs(ext_dir, exist_ok=True)
        shutil.copy(shared_lib, ext_dir)
        print(f"Copied {shared_lib} to {ext_dir}")


setup(
    name="finmath",
    version="0.0.1",
    author="Your Name",
    author_email="your_email@example.com",
    description="High-performance financial math library with Python bindings.",
    ext_modules=[
        Extension(
            "finmath",  # Must match the target output in CMakeLists.txt
            sources=[],  # CMake handles source files
        )
    ],
    cmdclass={"build_ext": CMakeBuildExt},
    packages=["finmath"],
    package_dir={"": "src"},
    include_package_data=True,
    package_data={"finmath": ["*.so"]},  # Ensure the shared library is included
)
