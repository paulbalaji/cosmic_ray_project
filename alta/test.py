import datetime

txt=open("C:\\Users\\Monish\\Documents\\COSMIC RAY\\cosmicray\\alta\\output\\blah2.txt")
txt=list(txt)
y=list(txt[1])
m=y[0]
for count2 in range(1,19):
    m=(m)+(y[count2])
m=m.replace(" ","0")
print(m)
M=datetime.datetime.strptime(m,"%Y/%m/%d0%H:%M:%S")
z=list(txt[0])
n=z[0]
for count3 in range(1,19):
    n=(n)+(z[count3])
n=n.replace(" ","0")
print(n)
N=datetime.datetime.strptime(n,"%Y/%m/%d0%H:%M:%S")
delta=M-N
delta=(M-N).total_seconds()
delta=int(delta)
print(delta)

#where M is new datetime
#N is original

#output needs to be in seconds though


#  read 1, read 2, read 3, pitch, vol1, vol2, vol3, timedelta, time
