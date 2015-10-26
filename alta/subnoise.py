import winsound, random

if __name__ == '__main__':
    for count in range(29):
        winsound.Beep(random.randint(750,1500),random.randint(100,600))
    winsound.Beep(random.randint(1300,2000),3000)
