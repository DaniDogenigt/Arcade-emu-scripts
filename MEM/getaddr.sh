#!/bin/bash
clear

sudo rm _get-section.sh _secdump _section _dump-cmd.sh _MEM _ADDR
echo "null" | sudo tee _MEM
#sudo chmod 777 _MEM


gamename=$(grep "gamename" -m1 /opt/retropie/configs/all/runcommand.log | awk -F':' '{print $3;}' | sed 's/,.*//')
section=$(grep $gamename gameaddr.txt | awk '{print $3}')
line=$(grep $gamename gameaddr.txt | awk '{print $4}')
varpos=$(grep $gamename gameaddr.txt | awk '{print $5}')
cmddump=$(printf "scanmem\x20\x2dp\x20\x60pidof\x20retroarch\x60\x20\x2dc\x27dregions\x20\x21$section\x3blregions\x3bexit\x27")

echo $cmddump |& sudo tee _get-section.sh 1>/dev/null
bash _get-section.sh |& sudo tee _secdump 1>/dev/null

awk 'NR==16' _secdump | awk -F'] |,' '{print $2;}' | sudo tee _section >/dev/null
awk 'NR==16' _secdump | awk -F',|b' '{print $2;}' | sudo tee -a _section >/dev/null
sed -i "s/ //g" _section

printf "scanmem\x20\x2dp\x20\x60pidof\x20retroarch\x60\x20\x2dc\x27dump\x20" | sudo tee _dump-cmd.sh >/dev/null
sed -n '1 h; 2{x;G;s/\n/ /;p}' _section | awk '{print $0 "\x3bexit\x27"}' | sudo tee -a _dump-cmd.sh >/dev/null

bash _dump-cmd.sh | sudo tee _MEM  &>/dev/null

#inotifywait -e close_write _MEM  |
#while read file eventlist eventfile
#do
#    echo "$eventfile"
#done
echo "done writing to file '_MEM' "

i=`expr $varpos + 1`
awk -v l="$line" -v p="$i" 'NR==l {print $1 $i}' _MEM | sudo tee _ADDR >/dev/null
sed -i "s/\:/ /" _ADDR

printf "\n\n"
echo "Game is:" $gamename
printf "\nMEM SECTION: $section\nLINE#: $line\nCOIN VAR POS.: $varpos\n"

coin_addr=$(cat _ADDR | awk '{print $1}')
printf "$coin_addr"


printf "scanmem\x20\x2dp\x20\x60pidof\x20retroarch\x60\x20\x2dc\x27dump\x20$coin_addr\x20$varpos\x3bexit\x27" | sudo tee _counter.sh &>/dev/null
count=$(sudo bash _counter.sh | awk -v p="$i" 'NR==1 {print $4}')
sudo bash _counter.sh |& awk 'NR==15 {print $4}'
clear

while true; do
  sleep 5 &
  sudo bash _counter.sh |& awk 'NR==15 {print $4}'
  wait # for sleep
done

