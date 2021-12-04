# C2XLib

C2XLib is a multiplatform (tested with Windows and Linux) C++ implementation for ETSI standardized communication protocols Co-operative Awareness Messages (CAM) and Decentralized Environmental Notification basic service Messages (DENM) with an Embedded Coder compatible Simulink API. These protocols are used for car to car and car to infrastructure communication. This implementation uses UDP as the transport layer and Basic-XER encoding rules, a human-readable XML encoding.

## Setup


### Windows
For setup on Windows and usage in Simulink, you can run the precompile and ready-to-use Windows installer or compile the library by yourself.

The easiest and fastest way is to use the installer:
1. Download and run the [Windows installer](C2XLib-0.1.0-win64.msi)
2. Add the following path to the compiled library to your environment variables. Simulink must find the library files. Otherwise, it will not run simulations when it can not find the execution engine.
```
C:\Program Files\c2xlib\lib
```
3. Open (or restart) Matlab and add the installation path to your Matlab paths.
```
C:\Program Files\c2xlib
```
![Matlab Set Path](doc/images/Matlab_Set-Path.jpg)

After successful installation, you find the C2XLib in your Simulink Library Browser, and you can add the API blocks to your model. Look into the matlab/examples directory for some examples, which you can also find under the following path.
```
C:\Program Files\c2xlib\matlab\examples
```

<!--## Matlab/Simulink Codegeneration-->
