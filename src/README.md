# Src Directories

To make the engine extensible we need to seperate it into sections.
that is why the modules directory exists.
for example the modules/win directory compiles into win_**PLATFORM**.splmod and is loaded by the engine (if **PLATFORM** is selected for the window module).
Same for the modules/gfx for the graphics module.