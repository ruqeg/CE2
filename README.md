![Intro](https://github.com/ruqeg/CE2/blob/main/docs/images/intro.jpg?raw=true)

## Setup
Download and install Vulkan SDK from [LunarG website](https://vulkan.lunarg.com/) for your system. Then clone this repository
```
git clone https://github.com/ruqeg/CE2
cd CE2
```
build VS2022 project with cmake
```
mkdir build
cd build
# For some reason we need to execute cmake two times because of the imgui library (idk why)
cmake ../ -G "Visual Studio 17 2022"
cmake ../ -G "Visual Studio 17 2022"
```
open generated `.sln` file in Visual Studio 2022 and build solution. 

To edit shaders in VS download the [GLSL language integration (for VS2022)](https://marketplace.visualstudio.com/items?itemName=DanielScherzer.GLSL2022) extension and configure:
```
"Audo detect shader type file extension":
.crude_shader

"Arguments for the external compiler executable":
--target-env vulkan1.2 --glsl-version 460 -DCRUDE_VALIDATOR_LINTING -e GL_GOOGLE_include_directive

"External compiler executable file path (without quotes)":
glslangValidator.exe
```

To edit shaders in VSCode, download the [GLSL Lints](https://marketplace.visualstudio.com/items?itemName=dtoplak.vscode-glsllint) extension and configure custom glslangValidatorArgs
```
"glsllint.glslangValidatorArgs": [
	"--target-env",
	"vulkan1.2",
	"--glsl-version",
	"460",
	"-DCRUDE_VALIDATOR_LINTING"
    "-e"
    "GL_GOOGLE_include_directive"
],
```

## Screenshots
![Editor](https://github.com/ruqeg/CE2/blob/main/docs/images/editor.png?raw=true)
![Editor1](https://github.com/ruqeg/CE2/blob/main/docs/images/editor1.png?raw=true)

## Metadata
Development was started at Feb 2025
