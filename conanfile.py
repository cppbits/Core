#!/usr/bin/env python3

from conans import ConanFile, CMake, tools

def get_version():
    import re
    import os

    try:
        content = tools.load("CMakeLists.txt")
        major   = re.search(".*BIT_CORE_VERSION_MAJOR ([0-9]+) .*", content).group(1)
        minor   = re.search(".*BIT_CORE_VERSION_MINOR ([0-9]+) .*", content).group(1)
        patch   = re.search(".*BIT_CORE_VERSION_PATCH ([0-9]+) .*", content).group(1)

        version = "{}.{}.{}".format(major,minor,patch)

        # Append build number if coming from a server
        build = os.getenv("BUILD_SUFFIX")
        if build is not None:
            version="{}-{}".format(version,build)

        return version
    except Exception:
        return None

class CppBitsCoreConan(ConanFile):
    name = "CppBitsCore"
    version = get_version()
    description = "A header-only standard template library"
    author = "cppbits"
    generators = "cmake"
    exports_sources = ("CMakeLists.txt",
                       "CMake/Templates/*.in",
                       "CMake/Modules/*.cmake",
                       "include/*",
                       "LICENSE.txt",
                       "docs/CMakeLists.txt",
                       "docs/Doxyfile.in")
    url = "https://github.com/cppbits/Core"
    license = "MIT License"
    options = {"install_docs" : [True,False],
               "dev_setup": [True,False]}
    default_options = ("install_docs=False",
                       "dev_setup=False")

    def requirements(self):
        if self.options.dev_setup:
            self.requires("Catch2/2.4.1@cppbits/stable",private=True)
        return

    def source(self):
        pass

    def build(self):
        pass

    def test(self):
        pass

    def package(self):
        cmake = CMake(self)
        cmake.definitions["BIT_CORE_COMPILE_SELF_CONTAINMENT_TESTS"] = "OFF"
        cmake.definitions["BIT_CORE_COMPILE_UNIT_TESTS"] = "OFF"
        cmake.definitions["BIT_CORE_GENERATE_DOCS"] = "ON" if self.options.install_docs else "OFF"
        cmake.definitions["BIT_CORE_INSTALL_DOCS"]  = "ON" if self.options.install_docs else "OFF"
        cmake.configure()

        # If we have specified to build install_docs, install them
        if self.options.install_docs:
            cmake.build(target="core_docs")
        cmake.install()

        self.copy(pattern="LICENSE.txt", dst="licenses")
        return

    def package_id(self):
        self.info.header_only()
        return
