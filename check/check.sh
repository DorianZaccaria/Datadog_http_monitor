#! /bin/bash

sudo ./DatadogMonitor --interface "en1" > check/test.txt 2> check/error.txt &

PID=$!
PROGRESS=0

trap "sudo kill $PID; exit" SIGINT SIGTERM

echo "WAIT: TEST IN PROGRESS [$PROGRESS %]"
PROGRESS=$(($PROGRESS + 4))
sleep 12s

echo "WAIT: TEST IN PROGRESS [$PROGRESS %]"
PROGRESS=$(($PROGRESS + 4))
curl http://google.com/translate -o check/tmp.html > /dev/null 2> /dev/null

for i in {1..2}
do
    sleep 10s
    echo "WAIT: TEST IN PROGRESS [$PROGRESS %]"
    PROGRESS=$(($PROGRESS + 4))
done

curl http://google.com/translate -o check/tmp.html > /dev/null 2> /dev/null
curl http://google.com/translate -o check/tmp.html > /dev/null 2> /dev/null
curl http://google.com/translate -o check/tmp.html > /dev/null 2> /dev/null

for i in {1..4}
do
    sleep 10s
    echo "WAIT: TEST IN PROGRESS [$PROGRESS %]"
    PROGRESS=$(($PROGRESS + 4))
done

curl http://google.com/maps -o check/tmp.html > /dev/null 2> /dev/null
curl http://google.com/maps -o check/tmp.html > /dev/null 2> /dev/null

sleep 10s
echo "WAIT: TEST IN PROGRESS [$PROGRESS %]"
PROGRESS=$(($PROGRESS + 4))

curl http://google.com/maps -o check/tmp.html > /dev/null 2> /dev/null
curl http://google.com/maps -o check/tmp.html > /dev/null 2> /dev/null

for i in {1..16}
do
    sleep 10s
    echo "WAIT: TEST IN PROGRESS [$PROGRESS %]"
    PROGRESS=$(($PROGRESS + 4))
done

sudo kill $PID
echo "WAIT: TEST IN PROGRESS [$PROGRESS %]"
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
    rm check/test.txt
    rm check/error.txt
fi

