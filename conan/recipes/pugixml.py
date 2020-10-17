# MIT License
# 
# Copyright (c) 2023 Zelimir Marojevic
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake
from conan.tools.files import get
import os

required_conan_version = ">=2.0"

class pugixml_recipe(ConanFile):
    name = "pugixml"
    version = "1.14.0"
    settings = "os", "compiler", "build_type", "arch"
    user = "atus"
    channel = "stable"
    no_copy_source = True 
    options = { 
                "shared": [True, False],
                "wchar_mode": [True, False],
                "no_exceptions": [True, False],
                "no_stl": [True, False],
              }
    default_options = {
                        "shared": True,
                        "wchar_mode": False,
                        "no_exceptions": True,
                        "no_stl": False,
                      }

    def requirements(self):
        for req in self.conan_data["tool_requires"]:
            self.tool_requires(req)

    def source(self):
        get(self, **self.conan_data["sources"][self.name][self.version])

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["BUILD_TESTING"] = False
        tc.variables["BUILD_SHARED_LIBS"] = self.options.shared
        tc.variables["PUGIXML_WCHAR_MODE"] = self.options.wchar_mode
        tc.variables["PUGIXML_NO_EXCEPTIONS"] = self.options.no_exceptions
        tc.variables["PUGIXML_NO_STL"] = self.options.no_stl
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.runenv_info.prepend_path("LD_LIBRARY_PATH", os.path.join(self.package_folder, "lib"))
        self.cpp_info.set_property("cmake_find_mode", "none")
        self.cpp_info.builddirs.append(os.path.join("lib", "cmake", "pugixml"))
