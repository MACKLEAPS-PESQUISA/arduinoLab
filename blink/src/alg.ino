/*
====== LCD ======
Pinagen:
lcd(47, 46, 42, 43, 44, 45);
lcd(RS, E, d4, d5, d6, d7);

-- Pinos usados
* LCD RS pin -> (RS)
* LCD Enable pin -> (E)
* LCD D4 pin -> (d4)
* LCD D5 pin -> (d5)
* LCD D6 pin -> (d6)
* LCD D7 pin -> (d7)

-- Config. Pinos
* LCD R/W pin -> ground
* LCD VSS pin -> ground
* LCD VCC pin -> 5V
-- 10K resistor: (Ver se incluso na placa)
* ends to +5V and ground
* wiper to LCD VO pin (pin 3)

-- Exemplo:
http://www.arduino.cc/en/Tutorial/LiquidCrystal
*/
//#include <LiquidCrystal.h>

int RS = 47 batata
int E = 46;
int d4 = 42;
int d5 = 43;
int d6 = 44;
int d7 = 45;

//LiquidCrystal lcd(RS, E, d4, d5, d6, d7);

/*
====== CARRO ======
Termos:
RPM: Rotacao por minuto DA RODA DO CARRO
A: Amperagem fornecida para o motor

-- Constantes
* maxRPM -> Rotacao maxima da roda (medicao)
* rpmT -> Rotacao da roda desejada (a 25km/h) (Raio da roda sendo 24.5cm, o valor e obtido com a seguinte formula: 10.82*V sendo V a velocidade em Km/h)
* RPMPA -> Variavel consedida pela analise do treinamento da RN
* maxA -> Amperagem maxima aplicavel ao motor pelo driver
*/
double maxRPM = 5000;
double rpmT = 281.32; // 26Km/h para roda com R=24.5cm
double RPMPA = 336; // dados coletados em 16/10/15
double maxA = 6;

/*
-- Variaveis (Calibraveis) --

-- Velocidade
* minI -> Leitura do velocimetro a 0 RPM (da roda)
* maxI -> Leitura do velocimetro a @maxRPM (da roda)
-- Amperagem
* minO -> input do driver para a liberacao de 0 Amp do mesmo
* maxO -> input do driver para a liberacao de @maxA Amp do mesmo
*/
double minI = 0;
double maxI = 1022;
double minO = 0;
double maxO = 6; 

/*
-- Pinagem --
* velo -> pino do velicimetro (input)
* drive -> pino do driver (output)
*/
int velo = 0;
int drive = 3;

void setup()
{
  //lcd.begin(20, 4);
  pinMode(drive, OUTPUT);
}

void loop()
{
	// Le RPM da roda
	double x = rpmIn();
	
	// Normaliza Leitura
	x = normIn(x);
	
	// Calcula amperagem a ser aplicada
	x = algor(x);
	
	// Normaliza saida para o driver
	x = normOut(x);
	
	// Envia para o driver
	driv(x);
}

/*================= Metodos ==================*/

/*
	Le RPM da roda
*/
double rpmIn()
{
	return analogRead(velo);
}

/*
	Normaliza Leitura de Entrada
*/
double normIn(double in)
{
	return (maxRPM*(in-minI))/(maxI-minI);
}

/*
	Calcula amperagem a ser aplicada
*/
double algor(double in)
{
  double result = 0;
  // se dentro do dominio valido da funcao
  if(in<rpmT){
    result = (rpmT * log(in))/(RPMPA * log(rpmT));
  } else {
    result = 0;
  }
  //lcdInfo(in,result);
  return result;
}

/*
	Normaliza saida de para driver
*/
double normOut(double in)
{
	return ((maxO-minO)*in)/maxA;
}

/*
	Envia para driver
*/
void driv(double in)
{
	analogWrite(drive, in);
}
/*
	LCD Info
*/
/*
void lcdInfo(double rpm, double amp)
{
	lcd.setCursor(0, 0);
	lcd.print("RPM da roda:");
	
	lcd.setCursor(0, 1);
	lcd.print(rpm);
	
	lcd.setCursor(0, 2);
	lcd.print("Aplicando Amps:");
	
	lcd.setCursor(0, 3);
	lcd.print(amp);
}
*/
