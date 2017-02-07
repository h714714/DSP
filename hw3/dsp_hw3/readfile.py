import sys
file=open(sys.argv[1], "r", encoding="big5hkscs")
content=file.read()
print (content)
