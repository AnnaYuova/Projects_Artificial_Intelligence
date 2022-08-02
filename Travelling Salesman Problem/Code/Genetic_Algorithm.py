import math
import random

#v triede priradim cez konstruktor x a y suradnice  a vratim v tvare (x,y)
class Mesto:
    def __init__(self, suradnicaX, suradnicaY):
        self.x = suradnicaX
        self.y = suradnicaY

    def __repr__(self):
        x = str(self.x)
        y = str(self.y)
        retazec = "(" + x + "," + y + ")"
        return retazec


#pri vytvoreni jedinca si rovno vypocitam aj jeho fitnes hodnotu a vazeny priemer ma zatial 0
class Jedinec:
    def __init__(self, zoznamMiest):
        self.zoznamMiest = zoznamMiest
        self.fitness = vypocetFitness(zoznamMiest)
        self.vazenyPriemer = 0


#nova populacia obsahuje vsetky generacie okrem pociatocnej
class NovaPopulacia:
    def __init__(self, skrizeneDieta):
        self.zoznamMiest = skrizeneDieta
        self.cesta = vypocetFitnessBezPrevratenejHodnoty(skrizeneDieta)


#vypocet vzdialenosti medzi mestami pytagorovou vetou = odmocnica z x na druhu + y na druhu
#fukncia je volana z funkcie vypocetFitness a vracia dlzku cesty medzi 2 miestami vypocitanu pytagorovou vetou
def vzdialenostMiest(mesto1, mesto2):
    suradnicaX = abs(mesto1.x - mesto2.x)
    suradnicaY = abs(mesto1.y - mesto2.y)
    pytagorovaVeta = math.sqrt((suradnicaX * suradnicaX) + (suradnicaY * suradnicaY))
    return pytagorovaVeta


#vypocet fitnes hodnot - prevratena hodnota dlzky cesty
#fukncia je volana z triedy Jedinec a vracia fitness hodnotu pre jednotliveho jedinca
def vypocetFitness(jedinec):
    dlzkaCesty = 0
    dlzkaJedinca = len(jedinec)
    for i in range(-1,dlzkaJedinca-1):
        dlzkaCesty += vzdialenostMiest(jedinec[i], jedinec[i+1])
    #print(dlzkaCesty)
    vysledok = 1/dlzkaCesty
    return vysledok


#samotna dlzka cesta bez prevratenej hodnoty
#fukncia je volana z funkcie geneticky algoritmus a vracia vysledku dlzku cesty medzi miestami
def vypocetFitnessBezPrevratenejHodnoty(jedinec):
    dlzkaCesty = 0
    dlzkaJedinca = len(jedinec)
    for i in range(-1,dlzkaJedinca-1):
        dlzkaCesty += vzdialenostMiest(jedinec[i], jedinec[i+1])
    return dlzkaCesty


#vytvorim pociatocnu populaciu - nahodne premesiam zoznam miest
#fukncia je volana z funkcie geneticky algoritmus a vracia premiesany zoznam miest
def vytvorPociatocnuPopulaciu(zoznamMiest, populaciaPocet):
    from random import sample
    populacia = []
    for i in range(0, populaciaPocet):
        pocetZoznamMiest = len(zoznamMiest)
        #sample nahodne premiesa miesta
        populacia.append(Jedinec(sample(zoznamMiest, pocetZoznamMiest)))
        #print(populacia[i].zoznamMiest)
    return populacia


#vyber rodica turnajom - nahodny vyber 3 jedincov a z tych 3 vyberiem jedneho, ktory ma najlepsiu fitness hodnotu
#fukncia je volana z funkcie geneticky algoritmus a vracia vybraneho rodica, ktory sa pojde krizit
def turnaj(populacia):
    indexy = []
    vybrany = []
    najmensie = 1000000
    for i in range(0, 3):
        while(1):
            nahodnyIndex = random.randint(0, len(populacia)-1)
            if nahodnyIndex not in indexy:
                indexy.append(nahodnyIndex)
                break

    for i in range(0, 3):
        index = indexy[i]
        vybrany = populacia[index].zoznamMiest
        if (populacia[index].fitness < najmensie):
            vybrany = populacia[index].zoznamMiest
            najmensie = populacia[index].fitness
    return vybrany


#vyber jedinca na krizenie vazenym priemerom - ak je nahodne cislo < sucty priemerov, vyberem ho
#fukncia je volana z funkcie geneticky algoritmus a vracia vybraneho rodica, ktory sa pojde krizit
def vyberJedincaNaKrizenie(populacia):
    nahodneCislo = random.random()
    sucetVazenychPriemerov = populacia[0].vazenyPriemer
    for i in range(0, len(populacia)):
        if (nahodneCislo < sucetVazenychPriemerov):
            return populacia[i]
        sucetVazenychPriemerov += populacia[i].vazenyPriemer
    return populacia[-1]


#mutacia skrizeneho potomka - ak je nahodne cislo < pravdepodobonost mutacie - mutujem, inak neurobi nic
#mutovanie - vymenim 2 nahodne miesta vedla seba
#fukncia je volana z funkcie krizenie a vracia bud zmutovaneho potomka alebo nezmeneneho potomka
def mutacia(dieta, pravdepodobnostMutacie):
    mutovaneDieta = []
    dlzkaDietata = len(dieta)
    nahodneCislo = random.randint(0,1)
    nahodnaPoziciaNaVymenu = random.randint(0,3)
    vymenitS = nahodnaPoziciaNaVymenu + 1

    if (nahodneCislo < pravdepodobnostMutacie):
        for i in range(0, dlzkaDietata):
            if (i == nahodnaPoziciaNaVymenu):
                mutovaneDieta.append(dieta[vymenitS])
            elif (i == nahodnaPoziciaNaVymenu + 1):
                mutovaneDieta.append(dieta[nahodnaPoziciaNaVymenu])
            else:
                mutovaneDieta.append(dieta[i])
        return mutovaneDieta
    else:
        return dieta

#vyberiem nahodny usek z prveho rodica a zvysok doplnim z druheho rodica, z dietata volam rovno aj mutovanie
#fukncia je volana z funkcie geneticky algoritmus a vracia skrizeneho a popripade aj zmutovaneho potomka
def krizenie(prvyJedinec, druhyJedinec, pravdepodobnostMutacie):
    dieta = []
    prvaCastDietata = []
    druhaCastDietata = []
    indexy = []

    prvy = prvyJedinec.zoznamMiest
    druhy = druhyJedinec.zoznamMiest
    #print(prvy)
    #print(druhy)

    #nahodne vyberem nejake cisla z prveho rodica, z ktoreho v tych cisla - indexoch vyberem jeho cast
    dlzkaPrvy = int(random.randint(1, len(prvy)))
    dlzkaPrvyy = int(random.randint(1, len(prvy)))
    #ak su nahodou rovnake obe cisla, hladam dovtedy kym nenajde ine druhe cislo
    if dlzkaPrvy == dlzkaPrvyy:
        while(1):
            dlzkaPrvyy = int(random.randint(1, len(prvy)))
            if dlzkaPrvy != dlzkaPrvyy:
                break

    #najdem odkial bude ta jeho zacinat a koncit, od mensieho po vacsie cislo
    zaciatok = min(dlzkaPrvy, dlzkaPrvyy)
    koniec = max(dlzkaPrvy, dlzkaPrvyy)
    #do prvaCastDietata si vlozim cast z prveho rodica
    for i in range(zaciatok, koniec):
        prvaCastDietata.append(prvy[i])
        indexy.append(i)

    #do druhej casti dam tie, ktore nie su v rodicovi1 a doplnim z rodica2
    druhaCastDietata = [item for item in druhy if item not in prvaCastDietata]
    #print(prvaCastDietata)
    #print(druhaCastDietata)
    pocitadloPrveho = 0
    pocitadloDruheho = 0
    indexyPocitadlo = 0
    indexyDlzka = len(indexy)
    countIndex = 0
    #v dieta mam zkrizeneho potomka z 2 rodicov
    for i in range(0, len(prvy)):
        if i == indexy[indexyPocitadlo]:
            dieta.append(prvaCastDietata[pocitadloPrveho])
            countIndex +=1
            pocitadloPrveho += 1
            if (countIndex != indexyDlzka):
                indexyPocitadlo +=1
        else:
            dieta.append(druhaCastDietata[pocitadloDruheho])
            pocitadloDruheho += 1

    vysledneDieta = mutacia(dieta, pravdepodobnostMutacie)
    #print("Vysledne zmutovanie dieta")
    #print(vysledneDieta)
    return vysledneDieta


#funkcia na geneticky algoritmus volana z mainu a vracia jedneho jedinca z populacia, ktory ma najkratsiu dlzku cesty
def genetickyAlgoritmus(zoznamMiest, pravdepodobnostMutacie, populaciaPocet, generacie, vyberNajlepsich):
    vysledok = []
    novaPopulacia = []
    # vytvorenie pociatocnej populacie
    populacia = vytvorPociatocnuPopulaciu(zoznamMiest, populaciaPocet)

    for i in range(0, generacie):
        #suma vsetkych fitness hodnota
        sumaFitHodnot = sum(jedinec.fitness for jedinec in populacia)
        #kazdemu jedincovi pridelim vazeny priemer
        for i in range(0, len(populacia)):
            populacia[i].vazenyPriemer = populacia[i].fitness/sumaFitHodnot

        #zoradim priamo populaciu podla vazeneho priemeru
        populacia.sort(key=lambda jedinec: jedinec.vazenyPriemer)

        #vygenerujem random cislo od 0-1 kym nenajdem jedinca, ktory ma vazeny priemer v tom rozsahu
        for i in range(0, len(populacia)):
            #prvyJedinec = turnaj(populacia)
            #druhyJedinec = turnaj(populacia)
            prvyJedinec = vyberJedincaNaKrizenie(populacia)
            druhyJedinec = vyberJedincaNaKrizenie(populacia)
            if(prvyJedinec == druhyJedinec):
                while(1):
                    #druhyJedinec = turnaj(populacia)
                    druhyJedinec = vyberJedincaNaKrizenie(populacia)
                    if(prvyJedinec != druhyJedinec):
                        break
            # do pola nova populacia si ulozim vzdy skrizeneho a zmutovaneho potomka
            novaPopulacia.append(NovaPopulacia(krizenie(prvyJedinec, druhyJedinec,pravdepodobnostMutacie)))

    # na konci populacie vypis najlepsie, dlzku jeho cesty
    najmensi = 100000000
    for i in range(0,len(novaPopulacia)):
        if (novaPopulacia[i].cesta < najmensi):
            najmensi = novaPopulacia[i].cesta
            vysledok = novaPopulacia[i].zoznamMiest
    print("vysledna cesta a hodnota")
    print(najmensi)
    print(vysledok)
    return vysledok


#main
#vygenerujem nahodne suradnice miest
zoznamMiest = []
populaciaPomocna = []
#nahodne vygenerujem pocet miest
pocetMiest = random.randint(20, 40)

#potom prepisat na range do pocetMiest
for i in range(0, pocetMiest):
    #nahodne vygenerujem x a y suradnice miest
    suradnicaX = int(random.random() * 200)
    suradnicaY = int(random.random() * 200)
    zoznamMiest.append(Mesto(suradnicaX, suradnicaY))

pravdepodobnostMutacie = 0.6
populaciaPocet = 5
generacie = 10
vyberNajlepsich = 5

#genetickyAlgoritmus
genetickyAlgoritmus(zoznamMiest, pravdepodobnostMutacie, populaciaPocet, generacie, vyberNajlepsich)
