import threading
import time
import random

class Filosofo(threading.Thread):

    def __init__(self, index, palilloIzq, palilloDer):
        threading.Thread.__init__(self) #inicializacion de hilos
        self.index = index
        self.palilloIzq = palilloIzq
        self.palilloDer = palilloDer

    def run(self):
       while(True):
            time.sleep(1)
            self.palillo()

#Funciones liberar y ocupar los palillos
    def palillo(self):
        palillo1 = self.palilloIzq
        palillo2 = self.palilloDer

        while True:
            #palillo1 esta siendo usado
            palillo1.acquire(True) 
            locked = palillo2.acquire(False)
            if locked:
                break
            #palillo1 no esta siendo usado
            palillo1.release()
            palillo1, palillo2 = palillo2, palillo1
        else:
            return 

        self.comer()
        palillo2.release()
        palillo1.release()

    def comer(self):
        print("Filosofo "+str(self.index) + " esta comiendo\n")
        time.sleep(1)
        print("Filosofo "+str(self.index) + " esta pensando\n")

def Cenafilosofos():
    filosofos = []
    palillos = []
    for n in range(5):
        palillos.append(threading.Lock())

    for i in range(5):
       filosofos.append(Filosofo(i, palillos[i], palillos[(i+1)%5]))
       print("Filosofo " + str(i) + " esta pensando\n")

    for p in filosofos:
        p.start()

if __name__ == "__main__":
    Cenafilosofos()
