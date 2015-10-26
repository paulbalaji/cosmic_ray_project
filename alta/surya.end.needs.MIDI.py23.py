#BY PAUL BALAJI AND SURYA RAGAVAN
#developed in the python 3.2.1 environment
#ported to py2.3
#The lists of values
import os, time, math, pypm
pypm.Initialize()
North = []
East = []
West = []
folderpath="C:/Users/Monish/Documents/COSMIC RAY/cosmicray/alta/text" #dir of the input files

class Channel(object):
    def __init__(self, devicenum=0):
        self.devicenum = devicenum
        self.latency = 0
        self.midi_device = pypm.Output(self.devicenum, self.latency)
        
    def startnote(self, midi_note):
        self.midi_device.WriteShort(0x90, midi_note, 100)
       
    def stopnote(self, midi_note):
        self.midi_device.WriteShort(0x90, midi_note, 0)
    
#All of this is meant to assign the readouts to the three different variables.
def finisher(i,j):
    return(int(txt[i][j:j+5]))

def assignment():
    for i in range (0,len(txt)):
        txt[i] = txt[i][19:]
    for i in range (0,len(txt)):
        for j in range (0,14,5):
            if j == 0:
                North.append(finisher(i,j))
            elif j == 5:
                East.append(finisher(i,j))
            elif j == 10:
                West.append(finisher(i,j))

def playMIDI(y,devicenum):
    channel = Channel(devicenum)
    play = channel.startnote
    sleep = time.sleep
    stop = channel.stopnote
    
    play(60+y)
    play(65+y)
    play(70+y)
    play(50+y)
    sleep(y)
    stop(60+y)
    stop(65+y)
    stop(70+y)
    stop(50+y)
    #return counter

def PrintDevices(InOrOut):
    for loop in range(pypm.CountDevices()):
        interf,name,inp,outp,opened = pypm.GetDeviceInfo(loop)
        if ((InOrOut == 0) & (inp == 1) |
            (InOrOut == 1) & (outp ==1)):
            print loop, name," ",
            if (inp == 1): print "(input) ",
            else: print "(output) ",
            if (opened == 1): print "(opened)"
            else: print "(unopened)"
    print
                
#Playing Music:
def playingmusic(devicenum):
    for x in range(0,len(North)):
        time.sleep(0.1)
        if North[x] > East[x] and North[x] > West [x]:
         #   os.startfile() #Note 1
            y=1
            playMIDI(y,devicenum)
            print("1")
        elif North[x] == East[x] and North[x] > West[x]:
          #  os.startfile() #Note 2
            print("2")
            y=2
            playMIDI(y,devicenum)
        elif North[x] == West[x] and North[x] > East[x]:
           # os.startdile() #Note 3
            print("3")
            y=3
            playMIDI(y,devicenum)
        elif West[x] > East[x] and West[x] > North[x]:
            #os.startfile() #Note 4
            print("4")
            y=4
            playMIDI(y,devicenum)
        elif West[x] == East[x] and West[x] > North[x]:
            #os.startfile() #Note 5
            print("5")
            y=5
            playMIDI(y,devicenum)
        elif East[x] > North[x] and East[x] > West[x]:
            #os.startfile() #Note 6
            print("6")
            y=6
            playMIDI(y,devicenum)
        elif North[x]==West[x]==East[x]:
            #os.startfile() #Note 7
            print("7")
            y=7
            playMIDI(y,devicenum)

if __name__ == "__main__":
    counter=0
    PrintDevices(1)
    devicenum = int(raw_input('enter devicenum: '))
    for filename in os.listdir(folderpath):
        filepath = os.path.join(folderpath, filename)
        txt = open(filepath, 'r')
        txt = list(txt)            
        assignment()
        playingmusic(devicenum) #removed counter for trying out
