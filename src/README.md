# Src Directories

To make the engine extensible we need to seperate it into sections.
that is why the modules directory exists.
for example the modules/scriptloading directory compiles into scriptloading.dll / scriptloading.so
and the scriptloading.dll / scriptloading.so is loaded by the engine.

The include dir contains code that the scripts can access, such as the ECS, and some other script specific stuff.