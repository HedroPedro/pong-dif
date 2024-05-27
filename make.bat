echo OFF
set CC=gcc
set INCLUDE_DIR=src/include src/game.c
set LIB_DIR=src/lib
set DEFAULT_FLAGS=-std=c99 -lmingw32 -lSDL2main -lSDL2 -lSDL2_TTF
set GAME_PATH=game
echo "1- Compilar"
echo "2- Compilar para release"
echo "3- Limpar"
set /p input="Digite sua opcao: "
if input == "1" goto compile_dev
if input == "2" goto compile_release
if input == "3" goto clean

:compile_dev
  %CC% -Wall -Wextra -L %LIB_DIR% -I %INCLUDE_DIR% -o %GAME_PATH% %DEFAULT_FLAGS%
  goto end

:compile_release
  %CC% -L %LIB_DIR% -I %INCLUDE_DIR% -o %GAME_PATH% %DEFAULT_FLAGS%
  goto end

:clean
  del game

:end