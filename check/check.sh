#! /bin/bash

sudo ./DatadogMonitor --interface "eth0" > check/test.txt 2> check/error.txt &

PID=$!

trap "sudo kill $PID; exit" SIGINT SIGTERM

echo "WAIT: TEST IN PROGRESS"

sleep 12s

echo "WAIT: TEST IN PROGRESS"
curl http://dzaccaria.no-ip.biz/owncloud -o check/tmp.html > /dev/null 2> /dev/null

for i in {1..2}
do
    sleep 10s
    echo "WAIT: TEST IN PROGRESS"
done

curl http://dzaccaria.no-ip.biz/owncloud -o check/tmp.html > /dev/null 2> /dev/null
curl http://dzaccaria.no-ip.biz/owncloud -o check/tmp.html > /dev/null 2> /dev/null
curl http://dzaccaria.no-ip.biz/owncloud -o check/tmp.html > /dev/null 2> /dev/null

for i in {1..4}
do
    sleep 10s
    echo "WAIT: TEST IN PROGRESS"
done

curl http://dzaccaria.no-ip.biz/zenphoto -o check/tmp.html > /dev/null 2> /dev/null
curl http://dzaccaria.no-ip.biz/zenphoto -o check/tmp.html > /dev/null 2> /dev/null

sleep 10s
echo "WAIT: TEST IN PROGRESS"

curl http://dzaccaria.no-ip.biz/zenphoto -o check/tmp.html > /dev/null 2> /dev/null
curl http://dzaccaria.no-ip.biz/zenphoto -o check/tmp.html > /dev/null 2> /dev/null

for i in {1..16}
do
    sleep 10s
    echo "WAIT: TEST IN PROGRESS"
done

sudo kill $PID

rm check/tmp.html

diff check/ref.txt check/test.txt > /dev/null 2> /dev/null

if [ $? != 0 ]; then
    echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
    echo "                           FAIL                            "
    echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
else
    echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
    echo "                          SUCCESS                          "
    echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
fi

#rm check/test.txt
#rm check/error.txt
