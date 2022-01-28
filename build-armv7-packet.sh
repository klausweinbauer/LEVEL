#!/bin/bash  

arm_build_dir="build-armv7"
tar_dir="c2xlib-armv7"
cross_compiler_image="thewtex/cross-compiler-linux-armv7"

if [ ! -x "$(command -v docker)" ]
then
    echo "[ERROR] Install Docker and try again."
    exit
fi

mkdir -p $arm_build_dir

if [ ! -f "dockcross" ]
then 
  echo "[INFO] Create dockcross script"
  docker run --rm $cross_compiler_image > ./dockcross
  chmod +x ./dockcross
fi

./dockcross cmake -B$arm_build_dir -H. -G"Unix Makefiles"
./dockcross make -C$arm_build_dir

echo "[INFO] Prepare library package..."
mkdir -p $arm_build_dir/$tar_dir
cp $arm_build_dir/c2xcam/libc2xcam.so $arm_build_dir/$tar_dir/
cp $arm_build_dir/c2xdenm/libc2xdenm.so $arm_build_dir/$tar_dir/
cp include/* $arm_build_dir/$tar_dir/

echo "[INFO] Create setup test"
testExec=$'#include <c2xcam.h>\n#include <stdio.h>\n\nint main(int argc, char** argv) {\n    return createCAM(1);\n}'
echo "$testExec" > $arm_build_dir/$tar_dir/test-exec.c

echo "[INFO] Create setup script"
script=$'cp libc2x*.so /usr/local/lib/\ncp c2x*.h /usr/local/include/\ngcc -Wall test-exec.c -o test-exec.a -lc2xcam\n./test-exec.a\nret=$?\nif [ $ret = 1 ]\nthen\n  echo "Install c2xlib: OK"\nelse\n  echo "Install c2xlib: FAILED"\nfi'
echo "$script" > $arm_build_dir/$tar_dir/setup.sh
chmod +x $arm_build_dir/$tar_dir/setup.sh

echo "[INFO] Pack library"
tar -C $arm_build_dir -cf $tar_dir.tar.gz $tar_dir/