#include <Keypad.h>     //ma librairies destiné a l'usage du clavier 4 * 4
#define VITESSE 340     //vitesse du son 340 m/s
const int       relai = 13;     //pin de mon relai

//commencement des declaration pour le clavier
const int       USTrig = 11; // Déclencheur sur la broche 11
const int       USEcho = 12; // Réception sur la broche 12

const int       ROW_NUM = 4; //4 lignes
const int       COLUMN_NUM = 4; //4 collones
char keys[ROW_NUM][COLUMN_NUM] = {
	{'1','2','3', 'A'},
	{'4','5','6', 'B'},
	{'7','8','9', 'C'},
	{'*','0','#', 'D'}
};
byte pin_rows[ROW_NUM] = {9, 8, 7, 6}; //connecter au ligne pinouts du clavier
byte pin_column[COLUMN_NUM] = {5, 4, 3, 2}; //connecter au collonne pinouts du clavier
Keypad clavier = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );
// fin des declaration neccessaire au fonctionnement du clavier

float ft_distance(void)
{
	// 1. Un état haut de 10 microsecondes est mis sur la broche "Trig"
	digitalWrite(USTrig, HIGH);
	delayMicroseconds(10); //on attend 10 µs
			       // 2. On remet à l’état bas la broche Trig
	digitalWrite(USTrig, LOW);
	// 3. On lit la durée d’état haut sur la broche "Echo"
	unsigned long duree = pulseIn(USEcho, HIGH);
	// 4. On divise cette durée par deux pour n'avoir qu'un trajet
	duree = duree/2;
	// 5. On calcule la distance avec la formule d=v*t
	float temps = duree/1000000.0; //on met en secondes
	float distance = temps*VITESSE; //on multiplie par la vitesse, d=t*v
	Serial.println(distance);
	return(distance);
}

char *ft_mdp(int mode)
{
	int i;
	char touche = clavier.getKey();
	char mdp[10];

	i = 0;
	while(i < 10 && touche != '#')
	{
		if(mode == 1 && ft_distance() < 2)
			return(0);
		touche = clavier.getKey();
		if(touche != 0)
		{
			mdp[i] = touche;
			i++;
			Serial.println(touche);
		}
	}
	Serial.println(mdp);
	return(mdp);
}

void setup()
{
	pinMode(USTrig, OUTPUT);
	pinMode(USEcho, INPUT);
	pinMode (relai, OUTPUT);
	Serial.begin(9600);
}

void loop()
{
	char *mdp;
	char touche = clavier.getKey();
	Serial.println("entrez mots de passe");
	mdp = ft_mdp(0);
	Serial.println(mdp);
	while(true)
	{
		touche = clavier.getKey();
		if(touche == '#')
		{
			if(ft_mdp(1) == mdp)
			{
				Serial.println(mdp);
				delay(5000);
			}
			else
			{
				digitalWrite(13, HIGH);
				delay(100);
				digitalWrite(13, LOW);
				delay(100);
				digitalWrite(13, HIGH);
				delay(100);
				digitalWrite(13, LOW);
			}
		}
		else if(ft_distance() < 2)
		{
			digitalWrite(13, HIGH);
			delay(100);
			digitalWrite(13, LOW);
			delay(100);
			digitalWrite(13, HIGH);
			delay(100);
			digitalWrite(13, LOW);
		}

	}
}
