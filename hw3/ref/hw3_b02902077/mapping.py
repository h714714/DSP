#-*- coding: big5 -*-
import sys

#argv[1]: input map ; argv[2]: output map
try:
    infile = open(sys.argv[1],'r')
except IOError as e:
    print "Can't open ", sys.argv[1]
    raise

outfile = open(sys.argv[2],'w')

Yin_to_word = {}
for line in infile:
    line_split = line.split()# split the line(['ni','ni3'])
#    print line_split
    word = line_split[0]  #get the word in Big5-ZhuYin map
    Yins = line_split[1].split("/") # get the ZhuYin of that word seperated by '/'
#    print Yins
    Yin_to_word[word] = word
#    print Yin_to_word[word].decode('big5')
    word_repeat = {}
    for head_yin in Yins:
        head_yin = head_yin[:2]
#        print head_yin.decode('big5')
        if Yin_to_word.has_key(head_yin) and  not(word_repeat.has_key(head_yin)):
            word_repeat[head_yin] = word
            Yin_to_word[head_yin] +=  ' '  + word
        #   print Yin_to_word[head_yin].decode('big5')
        elif not Yin_to_word.has_key(head_yin):
            Yin_to_word[head_yin] = word

#print Yin_to_word
for yin,word in Yin_to_word.items():
#    print yin, word
    outfile.write(yin+"    ")
    outfile.write(word+"\n")

