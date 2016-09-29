#!/bin/sh 
appname=fs4412_key_app
drvname=fs4412_key 
arm-linux-gcc ${appname}.c -o ${appname} 
cp ${appname} ~/rootfs/ 
cp ${drvname}.ko ~/rootfs/ 
