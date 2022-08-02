import matplotlib.pyplot as plt
import math
import random
import numpy as np

pole_bodov = []
pole_vsetkych_bodov = []


class Bod:
    def __init__(self, suradnicaX, suradnicaY):
        self.x = suradnicaX
        self.y = suradnicaY
        self.farba = ""

    def __repr__(self):
        x = str(self.x)
        y = str(self.y)
        retazec = ("(" + x + "," + y + ")")
        return retazec

    def nastav_farbu(self, f):
        self.farba = f


def vykresli_body(pole_bodov):
    for i in range(0, len(pole_bodov)):
        plt.scatter(pole_bodov[i].x, pole_bodov[i].y, label="", color=pole_bodov[i].farba, marker="o", s=20)


def euklidovska_vzdialenost(x1, y1, x2, y2):
    vzdialenost = 0
    x = abs(x1 - x2)
    y = abs(y1 - y2)
    vzdialenost = math.sqrt(x * x + y * y)
    return vzdialenost


def classify(x, y, k):
    pole_vzdialenosti = []
    for i in range(0, len(pole_vsetkych_bodov)):
        pole_vzdialenosti.append((euklidovska_vzdialenost(x, y, pole_vsetkych_bodov[i].x, pole_vsetkych_bodov[i].y), pole_vsetkych_bodov[i].farba))
    pole_vzdialenosti.sort(key=lambda b: b[0])
    # vrati mi rovno cast pola - ak k= 3 tak vrati rovne prve tri najblizsie
    najblizsie = pole_vzdialenosti[0:k]
    najblizsie = np.array(najblizsie)
    farby = najblizsie[:, 1]
    unikatna_farba, pocet = np.unique(farby, return_counts=True)
    return unikatna_farba[pocet.argmax()]


def nahodny_bod(prvy_interval, druhy_interval, treti_interval, stvrty_interval):
    bod = Bod(random.randint(prvy_interval, druhy_interval), (random.randint(treti_interval, stvrty_interval)))
    return bod


# x-ove suradnice pociatocnych bodov si ulozim do pola
pociatocne_body_x = [-4500, -4100, -1800, -2500, -2000, 4500, 4100, 1800, 2500, 2000, -4500, -4100, -1800, -2500, -2000,
                     4500, 4100, 1800, 2500, 2000]

# y-ove suradnice pociatocnych bodov si ulozim do pola
pociatocne_body_y = [-4400, -3000, -2400, - 3400, -1400, -4400, -3000, -2400, - 3400, -1400, 4400, 3000, 2400, 3400,
                     1400, 4400, 3000, 2400, 3400, 1400]

# pociatocnym bodom idem priradit triedu s farbou
# 1. 5 bodov zaradim do cervenej triedy, dalsie do zelenej, dalsie do modrej a poslednych 5 bodov do fialovej triedy
for i in range(0, 20):
    # bod si vratim ako surandnicu v 2d priestore
    bod = Bod(pociatocne_body_x[i], pociatocne_body_y[i])
    if i <= 4:
        bod.nastav_farbu("red")
    if i > 4 and i <= 9:
        bod.nastav_farbu("green")
    if i > 9 and i <= 14:
        bod.nastav_farbu("blue")
    if i > 14:
        bod.nastav_farbu("purple")

    pole_bodov.append(bod)
    pole_vsetkych_bodov.append(bod)
    vykresli_body(pole_vsetkych_bodov)


# k = 1,3,7,15
bod = 0
counter_dobre = 0
counter_zle = 0
pole_farieb = ["cervena", "zelena", "modra", "fialova"]
predpokladana_farba = ""
predosla_farba = ""

# generujem postupne vsetky body, ocakavam ze budu patrit do tried v tomto poradi: cervena, zelena, modra, fialova
# body generujem s 99% uspesnou pravdepobonost - 1 bod zo 100 nie je z daneho intervalu
# vytvorim si pomocnu premennu, do ktorej si ulozim triedu, ktoru ocakava
# tuto premennu porovnam s tou, ktoru vratil klasifikator - na zaklade toho vypocitam uspenost klasifikatora
# vo while 1 skontrolujem, ci neboli dvakrat po sebe priradene tie iste triedy ( 2 rovnake farby po sebe )
# v druhom while osetrujem generovanie s pravdepodobnostou 99 percent
k = 3
for i in range(0, 20000):
    while 1:
        while 1:
            if abs(i) % 100 == 99:
                bod = nahodny_bod(-5000, 5000, -5000, 5000)
            elif i == 0 or i % 4 == 0:
                bod = nahodny_bod(-5000, 500, -5000, 500)
                predpokladana_farba = "red"
            elif i == 1 or i % 4 == 1:
                bod = nahodny_bod(-500, 5000, -5000, 500)
                predpokladana_farba = "green"
            elif i == 2 or i % 4 == 2:
                bod = nahodny_bod(-5000, 500, -500, 5000)
                predpokladana_farba = "blue"
            else:
                bod = nahodny_bod(-500, 5000, -500, 5000)
                predpokladana_farba = "purple"
            if bod not in pole_vsetkych_bodov:
                break
        zaradenie = classify(bod.x, bod.y, k)
        #bod.farba = zaradenie
        if zaradenie != predosla_farba:
            break

    pole_vsetkych_bodov.append(bod)
    predosla_farba = zaradenie
    bod.nastav_farbu(zaradenie)

    if predpokladana_farba == zaradenie:
        counter_dobre += 1
    else:
        counter_zle += 1

vykresli_body(pole_vsetkych_bodov)
print(counter_dobre)
print(counter_zle)

vypocet = counter_zle / 20000
vypocet = 100 - vypocet*100
print("Uspesnost mojho klasikatora: " + str(vypocet))

# popis grafu
plt.title('Klasifikácia pre k: ' + str(k) + "-NN")
#plt.legend()
# vykreslenie grafu
plt.show()
