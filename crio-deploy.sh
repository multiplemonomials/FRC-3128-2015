deploy_path=/ni-rt/system/FRC_UserProgram.out
ip=10.31.28.2
target="./Crio Build/FRC-3128-2015"

wput -u "${target}" "ftp://anonymous@${ip}${deploy_path}"
