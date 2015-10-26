#BY PAUL BALAJI with Surya Ragavan
#The lists of values
import os, time, math, random, datetime, cmath
North = []
East = []
West = []
folderpath="C:/Users/Monish/Documents/COSMIC RAY/cosmicray/alta/output" #dir of the input files
outpath="C:/Users/Monish/Documents/COSMIC RAY/cosmicray/alta/MIDIout" #dir of the output files
#NB python does not accept "\"  MUST USE "/" or "\\" when naming folderpath



def deltatime(count, O):
    txtt=open("C:\\Users\\Monish\\Documents\\COSMIC RAY\\cosmicray\\alta\\output\\textfilescombined.txt")
    txtt=list(txtt)
    y=txtt[count]
    m=y[0]
    for appendingcount in range(1,19):
        m=(m)+(y[appendingcount])
    m=m.replace(" ","0")
    M=datetime.datetime.strptime(m,"%Y/%m/%d0%H:%M:%S")
    delta=M-O
    delta=(M-O).total_seconds()
    delta=int(delta)
    #O=M
    return delta, O
    

#function to calculate the pitch
def pitchnum(mean):
    mean=mean/440
    pitch=cmath.log(mean,2)
    pitch=12*pitch
    pitch=pitch+69
    pitch=pitch-0j
    pitch=round(pitch.real)
    pitch=int(pitch)
    return pitch
    
#All of this is meant to assign the readouts to the three different variables.
def finisher(i,j):
    return(int(txt[i][j:j+5]))

#appends data from txt to different lists
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

#defining variables and ACTUALLY writing the MIDI files
def playMIDI(x,counter,counter2, O):
    from midiutil.MidiFile import MIDIFile
    MyMIDI=MIDIFile(1, removeDuplicates=False, deinterleave=False)
    track=0
    time=0
    MyMIDI.addTrackName(track,time,"Sample")
    MyMIDI.addTempo(track,time,60)
    track=0
    channel=0
    a=North[x]
    b=East[x]
    c=West[x]
    averagex=round(((a+b+c)/3),0)
    pitchcalc=pitchnum(averagex)
    pitch=pitchcalc
    timecalc,O=deltatime(counter, O)
    time=2
    duration=timecalc
    if counter2==0:
        volume=10*(int(round(math.pow(North[x],1.0/3))))
    elif counter2==1:
        volume=10*(int(round(math.pow(East[x],1.0/3))))
    elif counter2==2:
        volume=10*(int(round(math.pow(West[x],1.0/3))))
    else:
        print("numcount error", counter2)
        volume=127
    MyMIDI.addNote(track,channel,pitch,time,duration,volume)
    outcount=str(counter)
    numcount=str(counter2)
    binfile=open(outpath+"/output "+outcount+" "+numcount+".mid",'wb')
    MyMIDI.writeFile(binfile)
    print(counter,"\n",track,channel,pitch,time,duration,volume)
    binfile.close()
    return O
                
#Playing Music:
def playingmusic(O):
    for x in range(0,len(North)):
        counter=x
        for midout in range(0,3):
            O=playMIDI(x,counter, midout, O)
    return O

#opening the text files sequentially
if __name__ == "__main__":
    for filename in os.listdir(folderpath):
        filepath = os.path.join(folderpath, filename)
        txt = open(filepath, 'r')
        txt = list(txt)
        ori=list(txt[0])
        orig=ori[0]
        for oricount in range(1,19):
            orig=(orig)+(ori[oricount])
        orig=orig.replace(" ","0")
        O=datetime.datetime.strptime(orig,"%Y/%m/%d0%H:%M:%S")
        assignment()
        O=playingmusic(O)
    print("MIDI Output Finished")
