#!/bin/bash

#Filename: checkword.sh

word=$1

grep "^$1$" dt38_american_english_这个是词汇列表 -q
 
if [ $? -eq 0 ]; then

echo $word 词汇_在_dt38_american_english_这个是词汇列表_文件中;

else

echo $word 词汇_不在_dt38_american_english_这个是词汇列表_文件中;

fi
