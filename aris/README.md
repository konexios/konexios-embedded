# README #

### How to build ###

1. Open e2studio and create a new workspace (path/to/workspace)
2. Clone this project into new workspace (path/to/workspace/aris_test)
3. In the e2studio import this project:
  File->Imort
  General/Existing Projects into Workspace
  Select root directory (path/to/workspace/aris_test)
  Finish
4. Select License file:
  Help->Synergy License...
  C:\Renesas\e2_studio\internal\projectgen\arm\Licenses\SSP_License_Example_EvalLicense_20160205.xml
5. Build project:
  Project->Build All

### How to flash ###

1. Enter the Debug folder (path/to/workspace/aris_test/Debug)
2. Execute the Aris_IOT_Demo.bat script (you should to see jlink window with a progressbars)