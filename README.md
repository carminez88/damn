# DAMN
## _Device Activity Monitor_

## Dependencies

DAMN requires the following dependencies: 

| Dep | Version |
| ------ | ------ |
| [Qt](https://www.qt.io/) | 5.15 or 6 |
| [ZeroMQ](https://zeromq.org/) | 4.3.4 |
| [cpp-ZMQ](https://github.com/zeromq/cppzmq) | 4.9.0 |
| [spdlog](https://github.com/gabime/spdlog) | 1.11.0 |
| [CLI11](https://github.com/CLIUtils/CLI11) | 2.3.2 |
| [cereal](https://github.com/USCiLab/cereal) | 1.3.2 |

All the above dependencies are automatically handled by conan with the exception on Qt. This has to be manually installed by the user.

## Installation

DAMN requires [Conan2](https://docs.conan.io/2/tutorial.html) to build the project.

First of all, [install conan2](https://docs.conan.io/2/installation.html):

```sh
pip install conan
```

Then, to build the project:

```sh
cd <wherever your DAMN repo is cloned>
conan install . --output-folder=build --build=missing
cd build
cmake -A x64 .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake
cmake --build . --config Release
```

## Run

The build process will generate three executable:

- **Broker**: this is the application that will act as a central server. Must be installed on a dedicated machine
- **VisualizationClient**: this is the application that will monitor all the registered devices
- **RegistrationClient**: this is the application that will allow current user to register her/his current session

To run the **Broker**:

```sh
./Broker
```

To run the **VisualizationClient**:

```sh
./VisualizationClient -a <broker IP address>
```

If no address is supplied, it will defaulted to 127.0.0.1.

To run the **RegistrationClient**:

```sh
./VisualizationClient -i <device identifier> -a <broker IP address>
```

The parameter *-i <device identifier>* is mandatory. If no address is supplied, it will defaulted to 127.0.0.1.


## License

**DWYW** - **D**o **W**hatever **Y**ou **W**ant