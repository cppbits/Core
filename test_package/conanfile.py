#!/usr/bin/env python

from os import getenv, path
from conans import ConanFile, CMake


class CppBitsCoreTestConan(ConanFile):
    generators = "cmake"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        return

    def test(self):
        return
