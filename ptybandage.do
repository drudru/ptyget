dependon warn-auto.sh $1.sh home
cat warn-auto.sh $1.sh | sed s}HOME}`./home`}g
chmod 755 $3
formake cat warn-auto.sh $1.sh '| sed s}HOME}`./home`}g >' $1
formake chmod 755 $1
