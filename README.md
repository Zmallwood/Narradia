# Narradia

Wilderness simulation game developed in C++ using:
* SDL2
* SDL2_image
* SDL2_ttf
* SDL2_mixer
* OpenGL
* GLEW
* assimp

![alt text](https://github.com/Zmallwood/Narradia/blob/e6892b3e5882a48c7049917c0183e7db0ec8eb28/Media/Screenshot0.png)

![alt text](https://github.com/Zmallwood/Narradia/blob/92e45121bf3fa64a75bea39a494207cb2cb7d2f3/Media/Screenshot1.png)

![alt text](https://github.com/Zmallwood/Narradia/blob/92e45121bf3fa64a75bea39a494207cb2cb7d2f3/Media/Screenshot2.png)


## Getting Started

### Install release

Further instructions for your OS is described below.

All available releases are found [here](https://github.com/Zmallwood/Narradia/releases).

#### Linux (Debian, Ubuntu)

```
# Add respository
add-apt-repository <REPO>

# Download and install
apt-get install narradia-<RELEASE_NUMBER>
```

#### Windows 10+

### Build from Source

#### Linux (Debian, Ubuntu)

```
git clone https://github.com/Zmallwood/Narradia.git
cd Narradia
./InstallDependencies.sh
git clone https://github.com/Zmallwood/NarradiaResources.git
mv NarradiaResources Bin
./Build.sh
```
To run:
```
Bin/Narradia
```
#### Windows 10+
