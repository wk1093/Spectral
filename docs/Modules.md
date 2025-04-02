[[_TOC_]]

# [Window Module](https://dev.azure.com/WyattKloos/Spectral/_git/Spectral?path=/src/modules/win)

This module manages opening a window to eventually be drawn to by the graphics module. This module also handles basic user input like mouse and keyboard, but does not support controllers. This basic user input should not be used for the entire game: #20

## Data Types

### `struct sWindowFlags`
- `bool vsync`
- `bool resizable`

### `struct sWindow`
- `double dt`: deltaTime, time since last main loop.
- `int width`: width of the window.
- `int height`: height of the window.
- `sWindowFlags flags`: The window flags given to `loadWindow`