#!/bin/bash
echo "GIMP will convert black to alpha and scale images"
case $1 in
  help)
    echo -e "parameters: \n  [input folder] ex. /home/user/mypics/\n  [output folder] ex. /home/user/newpics/\n  [size] size after scale\n ex. $ bash ares.sh start /home/user/mypics/ /home/user/newpics/ 100"
    ;;  
  start)
    if [ ! -d "$3" ]; then
      mkdir $3
    fi
    cf=$(ls -1 $2 | wc -l)
    nc=$(echo -n $(echo $(ls $2 | sort -n | head -1)) | wc -c)-4
    for ((i=1;i<=cf;i++)); 
      do
        getfilename=$(ls $2 | sort -n | head -$i | tail -1)
        fl=$2$getfilename
        ftmp=$2$getfilename.tmp
        opc=$3$getfilename
        echo -e "\nFile: $fl \n-------------------------------"
        gimp -i -b "(let* ( ( image (car (file-png-load 1 \"$fl\" \"$ftmp\") ) ) (drawable (car (gimp-image-active-drawable image) ) ) (mask (car (gimp-layer-create-mask drawable ADD-WHITE-MASK))))
   (gimp-layer-add-mask drawable mask) (gimp-brightness-contrast mask -127 0) (gimp-image-remove-layer-mask image drawable MASK-APPLY)  (gimp-file-save RUN-NONINTERACTIVE image drawable \"$opc\" \"$opc\") ) (gimp-quit 0)"
    done
    ;;
  *)
    echo -e "parameters: \n  [input folder] ex. /home/user/mypics/\n  [output folder] ex. /home/user/newpics/\n  [size] size after scale\n ex. $ bash ares.sh start /home/user/mypics/ /home/user/newpics/ 100"
    ;;
esac



