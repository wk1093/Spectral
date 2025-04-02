# group `ModuleWin` 

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`enum `[`Key`](#group___module_win_1gab3c7af4820830f9166ede9e5623c4e73)            | 
`enum `[`CursorMode`](#group___module_win_1ga3bbf59221044115f117dc97bb8930648)            | Cursor mode enumeration.
`public inline `[`sWindow`](docs/ModuleWin.md#structs_window)` * `[`loadWindow`](#group___module_win_1gaf37eab1ce1cde80532f09d9b32133184)`(const char * name,int width,int height,`[`sWindowFlags`](docs/ModuleWin.md#structs_window_flags)` flags)`            | 
`public inline `[`sWindow`](docs/ModuleWin.md#structs_window)` * `[`loadWindow`](#group___module_win_1gafbb39fabdd17eabc5fc13c5f35b26a59)`(const char * name,int width,int height,bool vsync,bool resizable)`            | 
`public inline void `[`updateWindow`](#group___module_win_1ga332a23ed050d2f60c9aa706580ade3f3)`(`[`sWindow`](docs/ModuleWin.md#structs_window)` * window)`            | 
`public inline double `[`getTime`](#group___module_win_1ga9efdb8131b23be2226b841d1969265a2)`(`[`sWindow`](docs/ModuleWin.md#structs_window)` window)`            | 
`public inline  explicit `[`WindowModule`](#group___module_win_1ga2427e19698191d79b01bde5e380533ff)`(const char * dynlib)`            | 
`struct `[`sWindowFlags`](#structs_window_flags) | 
`struct `[`sWindow`](#structs_window) | Window structure.
`struct `[`WindowModule`](#struct_window_module) | Window module class.

## Members

#### `enum `[`Key`](#group___module_win_1gab3c7af4820830f9166ede9e5623c4e73) 

 Values                         | Descriptions                                
--------------------------------|---------------------------------------------
A            | 
B            | 
C            | 
D            | 
E            | 
F            | 
G            | 
H            | 
I            | 
J            | 
K            | 
L            | 
M            | 
N            | 
O            | 
P            | 
Q            | 
R            | 
S            | 
T            | 
U            | 
V            | 
W            | 
X            | 
Y            | 
Z            | 
Num0            | 
Num1            | 
Num2            | 
Num3            | 
Num4            | 
Num5            | 
Num6            | 
Num7            | 
Num8            | 
Num9            | 
Escape            | 
LControl            | 
LShift            | 
LAlt            | 
LSystem            | 
RControl            | 
RShift            | 
RAlt            | 
RSystem            | 
Menu            | 
LBracket            | 
RBracket            | 
SemiColon            | 
Comma            | 
Period            | 
Quote            | 
Slash            | 
BackSlash            | 
Tilde            | 
Equal            | 
Dash            | 
Space            | 
Return            | 
BackSpace            | 
Tab            | 
PageUp            | 
PageDown            | 
End            | 
Home            | 
Insert            | 
Delete            | 
Add            | 
Subtract            | 
Multiply            | 
Divide            | 
Left            | 
Right            | 
Up            | 
Down            | 
Numpad0            | 
Numpad1            | 
Numpad2            | 
Numpad3            | 
Numpad4            | 
Numpad5            | 
Numpad6            | 
Numpad7            | 
Numpad8            | 
Numpad9            | 
F1            | 
F2            | 
F3            | 
F4            | 
F5            | 
F6            | 
F7            | 
F8            | 
F9            | 
F10            | 
F11            | 
F12            | 
F13            | 
F14            | 
F15            | 
Pause            | 
KeyCount            | 

#### `enum `[`CursorMode`](#group___module_win_1ga3bbf59221044115f117dc97bb8930648) 

 Values                         | Descriptions                                
--------------------------------|---------------------------------------------
Normal            | Normal cursor mode.
Hidden            | Hidden cursor mode.
Disabled            | Locked cursor mode.

Cursor mode enumeration.

This enumeration defines the different modes for the cursor in the window.

The cursor mode may not be supported on all platforms or windowing libraries. 

**See also**: WindowModule::setCursorMode

#### `public inline `[`sWindow`](docs/ModuleWin.md#structs_window)` * `[`loadWindow`](#group___module_win_1gaf37eab1ce1cde80532f09d9b32133184)`(const char * name,int width,int height,`[`sWindowFlags`](docs/ModuleWin.md#structs_window_flags)` flags)` 

#### `public inline `[`sWindow`](docs/ModuleWin.md#structs_window)` * `[`loadWindow`](#group___module_win_1gafbb39fabdd17eabc5fc13c5f35b26a59)`(const char * name,int width,int height,bool vsync,bool resizable)` 

#### `public inline void `[`updateWindow`](#group___module_win_1ga332a23ed050d2f60c9aa706580ade3f3)`(`[`sWindow`](docs/ModuleWin.md#structs_window)` * window)` 

#### `public inline double `[`getTime`](#group___module_win_1ga9efdb8131b23be2226b841d1969265a2)`(`[`sWindow`](docs/ModuleWin.md#structs_window)` window)` 

#### `public inline  explicit `[`WindowModule`](#group___module_win_1ga2427e19698191d79b01bde5e380533ff)`(const char * dynlib)` 

# struct `sWindowFlags` 

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# struct `sWindow` 

Window structure.

This structure represents a window created by the window module. It contains various properties and methods for interacting with the window.

Certain members of this structure are meant to be not accessed directly, but cannot be made private due to the internal implementation.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# struct `WindowModule` 

```
struct WindowModule
  : public Module
```  

Window module class.

This class represents the window module and provides methods for creating and managing windows. It is responsible for loading the windowing library and providing access to its functions.

This class contains function pointers that will be loaded from the module file (a dynamic library). 

These functions will do something different depending on the implementation selected at runtime.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

