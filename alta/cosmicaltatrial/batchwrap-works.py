import glob, os, shutil, math, sys, subprocess, time

def countalta():
    altacount = 0
    for root, dirs, files in os.walk("C:\\Users\\Monish\\Documents\\COSMIC RAY\\cosmicray\\alta\\cosmicaltatrial"):
        for file in files:    
            if file.endswith('.alta'):
                altacount += 1
    return altacount

def openbat():
    from subprocess import Popen
    p = Popen("C:/Users/Monish/Documents/COSMIC RAY/cosmicray/alta/cosmicaltatrial/trial.bat")
    print("Correctly opening batch and writing to .txt")

def movetxt():
    #os.system("TASKKILL C:/Windows/system32/cmd.exe")
    files = glob.iglob(os.path.join("C:\\Users\\Monish\\Documents\\COSMIC RAY\\cosmicray\\alta\\cosmicaltatrial", "*.txt"))
    for file in files:
        if os.path.isfile(file):
            shutil.copy(file, "C:\\Users\\Monish\\Documents\\COSMIC RAY\\cosmicray\\alta\\output")
            print("correctly copies files")
    files = os.listdir("C:\\Users\\Monish\\Documents\\COSMIC RAY\\cosmicray\\alta\\cosmicaltatrial")
    for f in files:
      if not os.path.isdir(f) and ".txt" in f:
        os.remove(f)
                    
currentalta=countalta()
print("correctly counts number of .alta files in dir")
print(currentalta)
openbat()
time.sleep(2)
movetxt()
num=0
newalta=countalta()
while num==0:
    if newalta>currentalta:
        currentalta=newalta
        openbat()
        time.sleep(2)
        movetxt()
        newalta=countalta()
end=input("enter something to close")
