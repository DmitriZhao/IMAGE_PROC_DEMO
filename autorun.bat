@echo off
for /r %%i in (.\res\new\*.BMP) do (
    call .\bin\Image_Proc_Demo.exe %%i
    echo In %%i
    echo.
)