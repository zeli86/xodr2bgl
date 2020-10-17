# MIT License
# 
# Copyright (c) 2024 Zelimir Marojevic
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

from conans import ConanFile, CMake
from conans.tools import download, untargz
import os, shutil

class pugixml_conan_recipe(ConanFile):
    name = "fakeit"
    version = "2.4.0"
    settings = {}
    user = "atus"
    channel = "stable"
    no_copy_source = True 
    options = {}
    default_options = {}

    def source(self):
        src_archive = "fakeit.tar.gz"
        download("https://github.com/eranpeer/FakeIt/archive/refs/tags/2.4.0.tar.gz", src_archive)
        untargz(src_archive, strip_root=True)
        os.unlink(src_archive)

    def configure_cmake(self):
        cmake = CMake(self)
        cmake.verbose = False
        cmake.parallel = True
        cmake.configure()
        return cmake

    def build(self):
        cmake = self.configure_cmake()
        cmake.build()

    def package_id(self):
        self.info.clear()

    def package(self):
        cmake = self.configure_cmake()
        cmake.install()
        tmp = os.path.dirname(self.build_folder)
        shutil.rmtree( os.path.join(os.path.dirname(tmp), "source" ), ignore_errors=True )
        shutil.rmtree( self.build_folder, ignore_errors=True )

    def package_info(self):
        self.env_info.LD_LIBRARY_PATH.append(os.path.join(self.package_folder, "lib"))

