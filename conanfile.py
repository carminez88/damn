from conan import ConanFile

class DamnRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        self.requires("zeromq/4.3.4")
        self.requires("cppzmq/4.9.0")
        self.requires("protobuf/3.21.9")
        self.requires("spdlog/1.11.0")
        self.requires("cli11/2.3.2")
        self.requires("gtest/1.13.0")
