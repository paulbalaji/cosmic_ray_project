#BY SURYA RAGAVAN
#The lists of values
import os, time
North = []
East = []
West = []
folderpath="C:/Users/Monish/Documents/COSMIC RAY/cosmicray/alta/text" #dir of the input files
    
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
                
#Playing Music:
def playingmusic():
    for x in range(0,len(North)):
        time.sleep(0.1)
        if North[x] > East[x] and North[x] > West [x]:
         #   os.startfile() #Note 1
            print("1")
        elif North[x] == East[x] and North[x] > West[x]:
          #  os.startfile() #Note 2
            print("2")
        elif North[x] == West[x] and North[x] > East[x]:
           # os.startdile() #Note 3
            print("3")
        elif West[x] > East[x] and West[x] > North[x]:
            #os.startfile() #Note 4
            print("4")
        elif West[x] == East[x] and West[x] > North[x]:
            #os.startfile() #Note 5
            print("5")
        elif East[x] > North[x] and East[x] > West[x]:
            #os.startfile() #Note 6
            print("6")
        elif North[x]==West[x]==East[x]:
            #os.startfile() #Note 7
            print("7")

if __name__ == "__main__":
    for filename in os.listdir(folderpath):
        filepath = os.path.join(folderpath, filename)
        txt = open(filepath, 'r')
        txt = list(txt)            
        assignment()
        playingmusic()
