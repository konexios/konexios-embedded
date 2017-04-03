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

5. Before build project you should to define DEFAULT_API_KEY and DEFAULT_SECRET_KEY;
   Project->Properties
   In the properties window select "C/C++ Build" Settings:
   Tool Settings panel -> Cross ARM C Compiler -> Preprocessor
   And in the "Define symbols" area on the right add a new items like this:
   DEFAULT_API_KEY=\\\"xxxxxxx\\\"
   or define this keys into a acn-sdk-c/private.h header file:

#ifndef ACN_SDK_C_PRIVATE_H_
#define ACN_SDK_C_PRIVATE_H_

#define DEFAULT_API_KEY "xxxxxx"
#define DEFAULT_SECRET_KEY "yyyyy"
#define DEV_ENV

#endif

   Warning: if you define the DEV_ENV symbol application will works with Arrow development portal only
   API address: pgsdev01.arrowconnect.io
   MQTT address: pgsdev01.arrowconnect.io


6. Build project:
  Project->Build All

### How to flash ###

1. Enter the Debug folder (path/to/workspace/aris_test/Debug)
2. Execute the aris.bat script (you should to see jlink window with a progressbars)
