#include <stdio.h>
#include <stdlib.h>

typedef struct uzol{
	int** aktualny;
	struct uzol *predchadzajuci;
	int hlbka; //g
}UZOL;

//funkcia volana z fukncie vratNajmensieF
//hamming heuristika vrati pocet policok, ktore nie su na svojom mieste, okrem medzery
int hamming(int **pole, int m, int n)
{
	int pomocnePole[100][100];
	int nieSuNaMieste = 0;
	int i,j; 
	int pocitadlo = 1;
	
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			pomocnePole[i][j] = pocitadlo;
			pocitadlo++;
		}
	}		
	//spocitam, tie ktore nie su na svojej pozicii a medzeru neratam
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			if ((pomocnePole[i][j] != pole[i][j]) && pole[i][j] != m*n)
			{
				nieSuNaMieste++;
			}
		}
	}	
	return nieSuNaMieste;
}


int vratiRiadok(int cislo, int m)
{
	return (cislo -1) / m;
}

int vratiStlpec(int cislo, int n)
{
	return (cislo -1) % n;
}

//funkcia volana z vratNajmensieF
//manhattan heuristika vrati sucet vzdialenosti policok od ich cielovej pozicie
int manhattan(int **pole, int m, int n)
{
	int i,j;
	int aktualnaHodnota;
	int riadok,stlpec,riadokKdeSom, stlpecKdeSom, pomocnyRiadok, pomocnyStlpec, medzivysledok;
	int vysledok = 0;
	
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			if (pole[i][j] == m*n)
			{
				continue;
			}
			else
			{
				medzivysledok = 0;
				//vrati mi cielove pozicie riadka a stlpca
				riadok = vratiRiadok(pole[i][j], m);
				stlpec = vratiStlpec(pole[i][j], n);
				
				//vrati mi aktualne pozicie riadka a stlpca
				riadokKdeSom = i + 1 / m;
				stlpecKdeSom = vratiStlpec(j + 1 + n, n);	
				
				pomocnyRiadok = abs(riadok - riadokKdeSom);
				pomocnyStlpec = abs(stlpec - stlpecKdeSom);
			
				medzivysledok = pomocnyRiadok + pomocnyStlpec;			
				vysledok = vysledok + medzivysledok; 			
			}
		}
	}
	return vysledok;
}


//funkcia volana z funkcie aStar
//vrati mi pole s najnizsou heuristickou hodnotou f, f = g(hlbka) + heuristika fukncia(manhattan/hamming)
UZOL* vratNajmensieF(UZOL **openedPole, int m, int n, int openedPocet)
{
	int i,j = 0;
	int index;
	int *pomocnePole = malloc(m * n * sizeof(int));
	UZOL *polenajmensieF = (UZOL*)(malloc(sizeof(UZOL)));
	
	//do pomocnePole ukladam hodnoty f
	for (i = 0; i < openedPocet; i++)
	{
		pomocnePole[i] = openedPole[i]->hlbka + manhattan(openedPole[i]->aktualny, m, n);
	}
	
	//prejdem cele pomocnePole a ulozim si index najmensieho
	int min = 1000000;
	for (i = 0; i < openedPocet; i++)
	{
		if (pomocnePole[i] < min)
		{
			min = pomocnePole[i];
			index = i;
		}
	}
	//vratim openedpole s indexom na ktorom bola najmensia f hodnota
	return openedPole[index];
}

//fukncia volana z aStar
//pozriem sa, ci som uz nebola niekedy v tom stave, ak ano vrati 1 - porovnam dve polia ci sa nerovnaju
int porovnajPolia(int **pole1, int **pole2, int m, int n) 
{
	int i,j;
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			if (pole1[i][j] != pole2[i][j])
			{
				return 0;
			}
		}
	}
	return 1;
}

//fukncia volana z dalsieStavy
//vrati mi pole, kde je vymenena medzera so znakom, ktory bol nalavo od medzery
int **dolava(int **pole, int m, int n, int poziciaX, int poziciaY)
{
	int i,j;
	int hodnota = 0;
	int **novePole = (int **) malloc(m * sizeof(int *));
	for (i = 0; i < m; i++)
	{
		novePole[i] = (int *) malloc(n * sizeof(int));
	}
	
	for (i = 0 ; i < m; i++)
	{
		for (j = 0; j < n ; j++)
		{
			if (poziciaX == i && poziciaY-1 == j)
			{
				hodnota = pole[i][j];
				novePole[i][j] = m*n;
				novePole[poziciaX][poziciaY] = hodnota;
			}
			else if (i == poziciaX && j == poziciaY)
			{
				continue;
			}
			else 
			{
				novePole[i][j] = pole[i][j];
			}				
		}
	}	
	/*
	for (i = 0 ; i < m; i++)
	{
		for (j = 0; j < n ; j++)
		{
			printf("%d ", novePole[i][j]);			
		}
	}	
	printf("\n");
	*/
	return novePole;
}


//fukncia volana z dalsieStavy
//vrati mi pole, kde je vymenena medzera so znakom, ktory bol napravo od medzery
int **doprava(int **pole, int m, int n, int poziciaX, int poziciaY)
{
	int i,j;
	int hodnota = 0;
	int **novePole = (int **) malloc(m * sizeof(int *));
	for (i = 0; i < m; i++)
	{
		novePole[i] = (int *) malloc(n * sizeof(int));
	}
	
	for (i = 0 ; i < m; i++)
	{
		for (j = 0; j < n ; j++)
		{
			if (poziciaX == i && poziciaY+1 == j)
			{
				hodnota = pole[i][j];
				novePole[i][j] = m*n;
				novePole[poziciaX][poziciaY] = hodnota;
			}
			else if (i == poziciaX && j == poziciaY)
			{
				continue;
			}
			else 
			{
				novePole[i][j] = pole[i][j];
			}				
		}
	}	
	/*
	for (i = 0 ; i < m; i++)
	{
		for (j = 0; j < n ; j++)
		{
			printf("%d ", novePole[i][j]);			
		}
	}
	printf("\n");
	*/
	return novePole;
}

//fukncia volana z dalsieStavy
//vrati mi pole, kde je vymenena medzera so znakom, ktory bol dole od medzery
int **dole(int **pole, int m, int n, int poziciaX, int poziciaY)
{
	int i,j;
	int hodnota = 0;
	int **novePole = (int **) malloc(m * sizeof(int *));
	for (i = 0; i < m; i++)
	{
		novePole[i] = (int *) malloc(n * sizeof(int));
	}
	
	for (i = 0 ; i < m; i++)
	{
		for (j = 0; j < n ; j++)
		{
			if (poziciaX+1 == i && poziciaY == j)
			{
				hodnota = pole[i][j];
				novePole[i][j] = m*n;
				novePole[poziciaX][poziciaY] = hodnota;
			}
			else if (i == poziciaX && j == poziciaY)
			{
				continue;
			}
			else 
			{
				novePole[i][j] = pole[i][j];
			}				
		}
	}	
	/*
	for (i = 0 ; i < m; i++)
	{
		for (j = 0; j < n ; j++)
		{
			printf("%d ", novePole[i][j]);			
		}
	}	
	printf("\n");
	*/
	return novePole;
}

//fukncia volana z dalsieStavy
//vrati mi pole, kde je vymenena medzera so znakom, ktory bol hore od medzery
int **hore(int **pole, int m, int n, int poziciaX, int poziciaY)
{
	int i,j;
	int hodnota = 0;
	int **novePole = (int **) malloc(m * sizeof(int *));
	for (i = 0; i < m; i++)
	{
		novePole[i] = (int *) malloc(n * sizeof(int));
	}
	
	for (i = 0 ; i < m; i++)
	{
		for (j = 0; j < n ; j++)
		{
			if (poziciaX-1 == i && poziciaY == j)
			{
				hodnota = pole[i][j];
				novePole[i][j] = m*n;
				novePole[poziciaX][poziciaY] = hodnota;
			}
			else if (i == poziciaX && j == poziciaY)
			{
				continue;
			}
			else 
			{
				novePole[i][j] = pole[i][j];
			}				
		}
	}	
	/*
	for (i = 0 ; i < m; i++)
	{
		for (j = 0; j < n ; j++)
		{
			printf("%d ", novePole[i][j]);			
		}
	}	
	printf("\n");
	*/
	return novePole;
}

//funkcia volana z fukncie aStar
//vygeneruje mi a vrati vsetky mozne dalsie stavy (polia), do ktorych sa da da dostat z aktualneho pola
UZOL **dalsieStavy(int **pole, int m, int n, int *pocetDalsichStavov, int hlbka)
{
	int i,j;
	int medzera = m*n;
	int poziciaX, poziciaY;
	UZOL **polia = (UZOL**)(malloc(sizeof(UZOL*)*4));
	for (i = 0; i < 4; i++)
	{
		polia[i] = (UZOL *) malloc(sizeof(UZOL));
	}
	//zistim si x-ovu a y-ovu poziciu medzery
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			if (pole[i][j] == medzera)
			{
				poziciaX = i;
				poziciaY = j;
			}
		}
	}
	
	//ak je medzera v rohu - vrati 2 stavy - moze sa posunut iba do 2 smerov
	if ((poziciaX == 0 && poziciaY == 0) || 
		(poziciaX == 0 && poziciaY+1 == n) ||
		(poziciaX+1 == m && poziciaY == 0) ||
		(poziciaX+1 == m && poziciaY+1 == n))
	{
		//ak je medzera v lavom hornom rohu
		if (poziciaX == 0 && poziciaY == 0)
		{
			polia[0]->aktualny = dole(pole,m,n, poziciaX, poziciaY); 
			polia[0]->hlbka = hlbka;
			polia[1]->aktualny = doprava(pole,m,n, poziciaX, poziciaY); 
			polia[1]->hlbka = hlbka;
			*pocetDalsichStavov = 2;
		}
		//ak je medzera v pravom hornom rohu
		if (poziciaX == 0 && poziciaY+1 == n)
		{
			polia[0]->aktualny = dole(pole,m,n, poziciaX, poziciaY); 
			polia[0]->hlbka = hlbka;
			polia[1]->aktualny = dolava(pole,m,n, poziciaX, poziciaY); 
			polia[1]->hlbka = hlbka;
			*pocetDalsichStavov = 2;
		}
		//ak je medzera v lavom dolnom rohu
		if (poziciaX+1 == m && poziciaY == 0)
		{
			polia[0]->aktualny = hore(pole,m,n, poziciaX, poziciaY); 
			polia[0]->hlbka = hlbka;
			polia[1]->aktualny = doprava(pole,m,n, poziciaX, poziciaY); 
			polia[1]->hlbka = hlbka;
			*pocetDalsichStavov = 2;
		}
		//ak je medzera v pravom dolnom rohu
		if (poziciaX+1 == m && poziciaY+1 == n)
		{
			polia[0]->aktualny = hore(pole,m,n, poziciaX, poziciaY); 
			polia[0]->hlbka = hlbka;
			polia[1]->aktualny = dolava(pole,m,n, poziciaX, poziciaY); 
			polia[1]->hlbka = hlbka;
			*pocetDalsichStavov = 2;
		}
	}
	//ak je medzera na kraji ale nie v rohu - vrati 3 stavy - moze sa posunut do 3 roznych smerov
	else if (poziciaX == 0 || poziciaY == 0 || poziciaX+1 == m || poziciaY+1 == n)
	{
		//ak je medzera pri lavom okraji
		if (poziciaY == 0)
		{
			polia[0]->aktualny = hore(pole,m,n, poziciaX, poziciaY); 
			polia[0]->hlbka = hlbka;
			polia[1]->aktualny = dole(pole,m,n, poziciaX, poziciaY);
			polia[1]->hlbka = hlbka;
			polia[2]->aktualny= doprava(pole,m,n, poziciaX, poziciaY);
			polia[2]->hlbka = hlbka;
			*pocetDalsichStavov = 3;		
		}
		//ak je medzera pri hornom okraji
		if (poziciaX == 0)
		{
			polia[0]->aktualny = doprava(pole,m,n, poziciaX, poziciaY);
			polia[0]->hlbka = hlbka;
			polia[1]->aktualny =  dolava(pole,m,n, poziciaX, poziciaY);
			polia[1]->hlbka = hlbka;
			polia[2]->aktualny = dole(pole,m,n, poziciaX, poziciaY);
			polia[2]->hlbka = hlbka;
			*pocetDalsichStavov = 3;
		}
		//ak je medzera mi dolnom okraji
		if (poziciaX+1 == m)
		{
			polia[0]->aktualny = doprava(pole,m,n, poziciaX, poziciaY);
			polia[0]->hlbka = hlbka;
			polia[1]->aktualny =  dolava(pole,m,n, poziciaX, poziciaY);
			polia[1]->hlbka = hlbka;
			polia[2]->aktualny = hore(pole,m,n, poziciaX, poziciaY);
			polia[2]->hlbka = hlbka;
			*pocetDalsichStavov = 3;
		}
		//ak je medzera pri lavom okraji
		if(poziciaY+1 == n)
		{
			polia[0]->aktualny = hore(pole,m,n, poziciaX, poziciaY);
			polia[0]->hlbka = hlbka;
			polia[1]->aktualny = dole(pole,m,n, poziciaX, poziciaY);
			polia[1]->hlbka = hlbka;
			polia[2]->aktualny =  dolava(pole,m,n, poziciaX, poziciaY);
			polia[2]->hlbka = hlbka;
			*pocetDalsichStavov = 3;
		}
	}		
	//ak je medzera hocikde v strede - vrati 4 stavy - vie sa posunut do 4 smerov
	else
	{
		polia[0]->aktualny = hore(pole,m,n, poziciaX, poziciaY);
		polia[0]->hlbka = hlbka;
		polia[1]->aktualny = dole(pole,m,n, poziciaX, poziciaY);
		polia[1]->hlbka = hlbka;
		polia[2]->aktualny =  dolava(pole,m,n, poziciaX, poziciaY);
		polia[2]->hlbka = hlbka;
		polia[3]->aktualny =  doprava(pole,m,n, poziciaX, poziciaY);
		polia[3]->hlbka = hlbka;
		*pocetDalsichStavov = 4;
	}
	return polia;
}

//fukncia volana z fukncie aStar a main
//skontroluje ci uz nemam finalny stav - usporiadany hlavolam
int skontrolujeKoniec(int **akt, int m, int n)
{
	int poleCiel[m][n];
	int i, j, pocitadlo = 1;
	int ano = 0;
	int nie = 1;
	
	//vytvorim si pole, ako chcem aby dany hlavolam
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			poleCiel[i][j] = pocitadlo;
			pocitadlo++;
		}
	}
	//porovnavam ci sa to rovna s mojim aktualnym stavom hlavolamu
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			if (akt[i][j] != poleCiel[i][j])
			{
				return 1;
			}
		}
	}
	return 0;
}

//aStar algoritmus volany z fukncie main
//generujem si dalsie stavy do ktorych sa da ist, postupne vyberam z nich vzdy jeden, ktory ma najmensiu hodnotu,
//nastavim si vybrany na aktualny a takto s nimi pracujem v cykle, az kym to nie je finalny stav
//pri kazdom generovani stavu zvysim hlbku o 1 - na vypocet heuristiky
int aStar(int **pole, int m, int n)
{
	int closedPocet = 0; 
	int openedPocet = 0;
	int zhoda = 0;
	
	UZOL *uzol = (UZOL*)(malloc(sizeof(UZOL)));
	UZOL **openedPole = (UZOL**)(malloc(sizeof(UZOL)*m*n));
	UZOL **closedPole = (UZOL**)(malloc(sizeof(UZOL)*m*n));
	UZOL *pomocnyUzol = (UZOL*)(malloc(sizeof(UZOL)));
	
	uzol->aktualny = pole;
	uzol->predchadzajuci = NULL;
	uzol->hlbka = 0;
	
	//vygeneruje nasledujuce stavy, kam sa da ist
	while (1)
	{
		int pocetStavov = 0;
		int i,j;
		//ulozim si vsetky dalsie mozne stavy, do ktorych sa da ist
		UZOL **nasledujuciStav = dalsieStavy((uzol->aktualny),m,n,&pocetStavov,uzol->hlbka+1);
	
		for (i = 0; i < pocetStavov; i++)
		{
			int najdene = 0;
			for(j = 0; j < closedPocet; j++)
			{
				//skontrolujem a vyhodim tie stavy, v ktorych som uz bola
				if (porovnajPolia((nasledujuciStav[i]->aktualny),(closedPole[j]->aktualny),m,n) == 1)
				{
					najdene = 1;
				}
			}
			//ak som v tych stavoch este nebola, ulozim si ich do opened
			if (najdene == 0)
			{
				//tie, co ostanu dam do opened
				openedPole[openedPocet] = nasledujuciStav[i];
				openedPocet++;
			}
		}
		
		//potom z opened vyberiem ten stav, ktory ma najnizsie f (heuristicka fukncia f = g + h) a dam do closed
		pomocnyUzol = vratNajmensieF(openedPole, m, n, openedPocet);
		for (i = 0; i < m; i++)
		{
			for (j = 0; j < n; j++)
			{
				printf("%d ", pomocnyUzol->aktualny[i][j]);
			}
			printf("\n");
		}
		printf("\n");
		
		//treba skontrolvat ci uz neni v tom closed?
		for (i = 0; i < closedPocet; i++)
		{
			if (porovnajPolia((pomocnyUzol->aktualny),(closedPole[i]->aktualny),m,n))
			{
				zhoda = 1;
			}
		}
		//s najnizsim f vyberiem z open a dam do close a z opened vymazem a nastavim ten vybrany na novy aktualny, ktory treba spracovat
		if (zhoda == 0)
		{
			closedPole[closedPocet]= pomocnyUzol;
			closedPocet++;
			pomocnyUzol->predchadzajuci = uzol;
			uzol = pomocnyUzol;
			//uzol->aktualny = pomocnyUzol->aktualny;
		}
		zhoda = 0;
		
		//este skontrolujem ten vybrany ci uz nie je dobre zoradeny - ak je zoradeny koncim, inak pokracujem
		if (skontrolujeKoniec(uzol->aktualny,m,n) == 0)
		{
			break;
		}	
		
		//vymazem vsetko z opened, uz tie stavy nepotrebujem a zaroven uvolnim pamat, aby som zybtocne nebrala vela miesta
		for (i = 0; i < openedPocet; i++)
		{
			free(openedPole[i]);
		}	
		free(openedPole);
		openedPocet = 0;
		UZOL **openedPole = (UZOL**)(malloc(sizeof(UZOL)*m*n));
	}
	return 0;
}


int main(int argc, char *argv[]) {
	int m,n,i,j;
	UZOL *mojUzol = (UZOL*)(malloc(sizeof(UZOL)));
	
	//nacitam velkost hlavolamu
	printf("Nacitaj 2 cisla (m,n):\n");
	scanf("%d %d", &m,&n);
	printf("Zadaj %d cisel do hlavolamu, medzera je %d\n", m*n,m*n);
	
	//urobim si miesto v pamati, podla velkosti hlavolamu
	int **pole = (int **) malloc(m * sizeof(int *));
	for (i = 0; i < m; i++)
	{
		pole[i] = (int *) malloc(n * sizeof(int));
	}
	//nacitam cisla hlavolamu
	for (i = 0 ;i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			scanf("%d", &pole[i][j]);
		}
	}
	//vypisem pociatocny stav hlavolamu
	for (i = 0 ;i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			printf("%d ", pole[i][j]);
		}
		printf("\n");
	}
	
	//na zaciatku sa pozriem ci uz nahodou nacitany hlavolam nie je aj finalny hlavolam
	if (skontrolujeKoniec(pole,m,n) == 1)
	{
		//ak to nie je konecny stav, musim ho cez aStar algortimus najst
		printf("\nhlavolam treba usporiadat\n\n");
		aStar(pole,m,n);
	}
	else
	{
		//ak to je konecny stav, koncim
		printf("\n8-hlavolam je uz usporiadany\n");
		return 0;
	}
	return 0;
}
