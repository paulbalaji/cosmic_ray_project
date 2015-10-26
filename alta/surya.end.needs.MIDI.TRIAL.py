#BY PAUL BALAJI and SURYA RAGAVAN
#The lists of values
import os, time, math, random
North = []
East = []
West = []
folderpath="C:/Users/Monish/Documents/COSMIC RAY/cosmicray/alta/output" #dir of the input files
    
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

def getrandom():
    r=random.uniform(0,10)
    r=round(r, 3)
    return r

def playMIDI(y,counter):
    from midiutil.MidiFile import MIDIFile
    MyMIDI=MIDIFile(1)
    track=0
    time=0
    MyMIDI.addTrackName(track,time,"Sample")
    MyMIDI.addTempo(track,time,120)
    track=0
    channel=0
    pitch=60+y+getrandom()
    time=0
    duration=getrandom()
    volume=100
    MyMIDI.addNote(track,channel,pitch,time,duration,volume)
    outcount=str(counter)
    binfile=open("C:/Users/Monish/Documents/COSMIC RAY/cosmicray/alta/MIDIout/output"+outcount+".mid",'wb')
    MyMIDI.writeFile(binfile)
    binfile.close()
    counter=counter+1
    return counter
                
#Playing Music:
def playingmusic(counter):
    for x in range(0,len(North)):
       # time.sleep(round(random.uniform(0,1)),2)
        if North[x] > East[x] and North[x] > West [x]:
         #   os.startfile() #Note 1
            y=float(1)
            print(y)
            counter=playMIDI(y,counter)
        elif North[x] == East[x] and North[x] > West[x]:
          #  os.startfile() #Note 2
            y=float(2)
            print(y)
            counter=playMIDI(y,counter)
        elif North[x] == West[x] and North[x] > East[x]:
           # os.startdile() #Note 3
            y=float(3)
            print(y)
            counter=playMIDI(y,counter)
        elif West[x] > East[x] and West[x] > North[x]:
            #os.startfile() #Note 4
            y=float(4)
            print(y)
            counter=playMIDI(y,counter)
        elif West[x] == East[x] and West[x] > North[x]:
            #os.startfile() #Note 5
            y=float(5)
            print(y)
            counter=playMIDI(y,counter)
        elif East[x] > North[x] and East[x] > West[x]:
            #os.startfile() #Note 6
            y=float(6)
            print(y)
            counter=playMIDI(y,counter)
        elif North[x]==West[x]==East[x]:
            #os.startfile() #Note 7
            y=float()
            print(y)
            counter=playMIDI(y,counter)

if __name__ == "__main__":
    counter=0
    for filename in os.listdir(folderpath):
        filepath = os.path.join(folderpath, filename)
        txt = open(filepath, 'r')
        txt = list(txt)            
        assignment()
        playingmusic(counter)
