# <a name='sWindow' /> public sWindow

Window structure. 



This structure represents a window created by the window module. It contains various properties and methods for interacting with the window.

> **note:** Certain members of this structure are meant to be not accessed directly, but cannot be made private due to the internal implementation. 




## Public Attributes
| Name | Description | 
| ---- | ---- |
| [dt](#sWindow-dt) | Delta time since the last frame.  |
| [width](#sWindow-width) | The width of the window.  |
| [height](#sWindow-height) | The height of the window.  |
| [flags](#sWindow-flags) | The window flags.  |



## Public Attributes
### <a name='sWindow-dt' /> public sWindow::dt 

Delta time since the last frame. 










This is the time in seconds since the last frame was rendered. 




[Go to Top](#sWindow)

### <a name='sWindow-width' /> public sWindow::width 

The width of the window. 










This is the width of the window in pixels. 




[Go to Top](#sWindow)

### <a name='sWindow-height' /> public sWindow::height 

The height of the window. 










This is the height of the window in pixels. 




[Go to Top](#sWindow)

### <a name='sWindow-flags' /> public sWindow::flags 

The window flags. 










This is a structure containing various properties of the window. > **see:** [sWindowFlags][sWindowFlags] 




[Go to Top](#sWindow)

[sWindowFlags]:./sWindowFlags.md#sWindowFlags
