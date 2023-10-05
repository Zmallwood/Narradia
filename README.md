# Narradia

Wilderness simulation game developed in C++ using:
* SDL2
* SDL2_image
* SDL2_ttf
* SDL2_mixer
* OpenGL
* GLEW
* assimp

![alt text](https://github.com/Zmallwood/Narradia/blob/6b05d9c222abc17aba4283e17fe4d17d231cdec5/Media/Screenshot0.png)

![alt text](https://github.com/Zmallwood/Narradia/blob/6b05d9c222abc17aba4283e17fe4d17d231cdec5/Media/Screenshot1.png)

![alt text](https://github.com/Zmallwood/Narradia/blob/6b05d9c222abc17aba4283e17fe4d17d231cdec5/Media/Screenshot2.png)


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
