@echo off

SET BUILD_DIR="%~dp0build"

IF NOT EXIST %BUILD_DIR% ( 
    mkdir %BUILD_DIR%
)

pushd %BUILD_DIR%

IF NOT EXIST "CMakeCache.txt" ( 
    cmake -DCMAKE_BUILD_TYPE=%1 -A x64 ..
)

IF "%1"=="Clean" (
    echo clean
    cmake --build . --target clean
) ELSE IF "%1"=="Rebuild" (
    echo rebuild
    cmake --build . --target clean
    cmake --build .
) ELSE (
    echo build
    cmake --build .
)


popd
