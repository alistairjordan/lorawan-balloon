if ./build.sh ; then
    sudo rm -rf ~/nfs/image
    sudo cp -rf output/image ~/nfs/image
else
    echo "Build failure, not copying"
fi

# Note to self, compiling a single thing is a lot easier when you add the toolchain!
# export PATH=/home/aj/lorawan-balloon/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin:$PATH