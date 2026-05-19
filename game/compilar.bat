@echo off

gcc src/*.c -Iinclude -Llib -lraylib -lopengl32 -lgdi32 -lwinmm -o jogo.exe

pause