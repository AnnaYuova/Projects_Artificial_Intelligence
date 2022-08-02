import math
import random

#v triede priradim cez konstruktor x a y suradnice  a vratim v tvare (x,y)
class Mesto:
    def __init__(self, suradniccaX, suradnicaY):
        self.x = suradnicaX
        self.y = suradnicaY

    def __repr__(self):
        x = str(self.x)
        y = str(self.y)
        retazec = ("(" + x + "," + y + ")")
        return retazec


#vypocet vzdialenosti medzi mestami pytagorovou vetou = odmocnica z x na druhu + y na druhu
#fukncia je volana z funkcie dlzkaCesty a vracia dlzku cesty medzi 2 miestami vypocitanu pytagorovou vetou
def vzdialenostMiest(mesto1, mesto2):
    suradnicaX = abs(mesto1.x - mesto2.x)
    suradnicaY = abs(mesto1.y - mesto2.y)
    pytagorovaVeta = math.sqrt((suradnicaX * suradnicaX) + (suradnicaY * suradnicaY))
    return pytagorovaVeta


#vypocet celkovej dlzky cesty medzi mestami
#fukncia je volana z fukncie simulovaneZihanie a vracia celkovu dlzku cesty medzi mestami
def dlzkaCesty(vymeneneMiesta, pocetMiest):
    dlzkaCesty = 0
    for i in range(0, pocetMiest - 1):
        dlzkaCesty += vzdialenostMiest(vymeneneMiesta[i], vymeneneMiesta[i + 1])
    return dlzkaCesty


#vymeni 2 nahodne miesa vedla seba na to, aby vytvorila nasledovnikov
#fukncia je volana z fukncie simulovaneZihanie a novy zoznam miest s vymenenymi mestami
def vymenDveMiestaVedlaSeba(prvotnePoradieMiest, pocetMiest):
    vymeneneMiesta = []
    nahodnaPoziciaNaVymenu = random.randint(0, pocetMiest-2)
    idemMenit = nahodnaPoziciaNaVymenu
    idemMenitS = nahodnaPoziciaNaVymenu + 1

    for i in range(0, pocetMiest):
        if (idemMenit == i):
            vymeneneMiesta.append(prvotnePoradieMiest[idemMenitS])
        elif (idemMenitS == i):
            vymeneneMiesta.append(prvotnePoradieMiest[idemMenit])
        else:
            vymeneneMiesta.append(prvotnePoradieMiest[i])
    return vymeneneMiesta


#na zaciatku nahodne preusporiadam zadanu mnozinu miest
#fukncia je volana z fukncie simulovaneZihanie a vracia nahodne premiesanie miest
def nahodnaPostupnostMiestNaZaciatku(zoznamMiest, pocetMiest):
    from random import sample
    poradieMiest = []
    #sample nahodne premiesa miesta
    poradieMiest = (sample(zoznamMiest, pocetMiest))
    return poradieMiest


#simulovane zihanie volane z fukncie main
def simulovaneZihanie(zoznamMiest, pocetMiest, teplota):
    aktualnePoradie = []
    nasledovnici = []
    najmensie = 1000000
    najkratsiaCesta = ""
    aktualny = nahodnaPostupnostMiestNaZaciatku(zoznamMiest, pocetMiest)

    for i in range(0, pocetMiest):
        nasledovnici.append(vymenDveMiestaVedlaSeba(aktualny, pocetMiest))

    for i in range(0, pocetMiest):
        print(aktualny)
        dlzkaCestyZaciatok = dlzkaCesty(aktualny, pocetMiest)
        print("dlzka cesty prveho")
        print(dlzkaCestyZaciatok)
        if (dlzkaCestyZaciatok < najmensie):
            najmensie = dlzkaCestyZaciatok
            najkratsiaCesta = aktualny

        nahodnyVyberNasledovnika = random.randint(0, pocetMiest - 1)
        nasledovnik = nasledovnici[nahodnyVyberNasledovnika]
        dlzkaCestyNasledovnik = dlzkaCesty(nasledovnik, pocetMiest)
        print("dlzka cesty nasledovnik")
        print(dlzkaCestyNasledovnik)
        if (dlzkaCestyNasledovnik < najmensie):
            najmensie = dlzkaCestyNasledovnik
            najkratsiaCesta = nasledovnik

        if (dlzkaCestyNasledovnik > dlzkaCestyZaciatok):
            # ak je nasledovnik horsi vypocitam teplotu
            pravdepodobnost = math.exp((dlzkaCestyZaciatok - dlzkaCestyNasledovnik) / teplota)
            teplota = teplota - 10
            nahodneCislo = random.randint(0, 1)
            # nemusi do nasledovnika prejst na 100%
            if (pravdepodobnost < nahodneCislo):
                pravdepodobnost = pravdepodobnost * 100
                print("Presiel do horsieho na ", int(pravdepodobnost), "%")
                aktualnePoradie.append(nasledovnik)
                aktualny = nasledovnik
        else:
            # ak je nasledovnik lepsi prejde do neho na 100%
            print("Presiel do lepsieho na 100%")
            aktualnePoradie.append(nasledovnik)
            aktualny = nasledovnik
        print("\n\n")
    print("Vysledka cesta a dlzka cesty najkratsieho v celom programe")
    print(najmensie)
    print(najkratsiaCesta)
    return najkratsiaCesta


zoznamMiest = []
#nahodne vygenerujem pocet miest
pocetMiest = random.randint(20,40)

#potom prepisat na 0,pocetMiest
for i in range(0, pocetMiest):
    suradnicaX = int(random.random() * 200)
    suradnicaY = int(random.random() * 200)
    zoznamMiest.append(Mesto(suradnicaX, suradnicaY))

vysledok = simulovaneZihanie(zoznamMiest, pocetMiest, 1000)


