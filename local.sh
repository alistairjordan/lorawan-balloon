if ./build.sh ; then
    sudo rm -rf ~/nfs/image
    sudo cp -rf output/image ~/nfs/image
else
    echo "Build failure, not copying"
fi