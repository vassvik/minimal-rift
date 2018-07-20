@echo OFF
cl /nologo /Iinclude /MT /wd4533 main.cpp lib/LibOVR.lib lib/glfw3.lib user32.lib gdi32.lib shell32.lib && main.exe
del main.obj
del main.lib
del main.exp
