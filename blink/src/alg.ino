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

//#include <LiquidCrystal.h>

int RS = 47;
int E = 46;
int d4 = 42;
int d5 = 43;
int d6 = 44;
int d7 = 45;

//LiquidCrystal lcd(RS, E, d4, d5, d6, d7);
*/

/*
 * Dados específicos do carro.
 *
 * Aqui, unidades em rpm referem-se à velocidade angular da RODA DO CARRO.
 * Unidades em A referem-se à amperagem fornecida para o MOTOR.
 *
 * RPM_MAX     :: velocidade angular máxima da roda
 * RPM_TARGET  :: velocidade angular alvo
 * RPM_PER_AMP :: velocidade angular obtida por cada ampere dado ao motor
 * AMP_MAX     :: amperagem máxima aplicável ao motor pelo driver
 *
 * RPM_TARGET é obtido a partir da velocidade alvo e do raio da roda.
 * RPM_PER_AMP foi coletado em 2015-10-16.
 */
const double RPM_MAX = 5000.0;
const double RPM_TARGET = 281.32;
const double RPM_PER_AMP = 336.0;
const double AMP_MAX = 6;

/*
 * Entrada(velocidade angular em rpm) e saída(amperagem) da placa.
 *
 * INPUT_MIN  :: leitura do velocímetro a 0 rpm
 * INPUT_MAX  :: leitura do velocímetro a RPM_MAX rpm
 * OUTPUT_MIN :: saída para o driver liberar 0 A
 * OUTPUT_MAX :: saída para o driver liberar AMP_MAX A
 *
 * Esses dados são importantes para a definição de intervalos de entrada e saída
 * que o Arduino não consegue prever.
 */
const double INPUT_MIN = 0;
const double INPUT_MAX = 1022;
const double OUTPUT_MIN = 0;
const double OUTPUT_MAX = 6;

const int PIN_VELOCIMETRO = 0; // pino do velocímetro (entrada)
const int PIN_DRIVER = 3;      // pino do driver (saída)

void setup()
{
  //lcd.begin(20, 4);
  pinMode(PIN_DRIVER, OUTPUT);
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

/*
	Le RPM da roda
*/
double rpmIn()
{
	return analogRead(PIN_VELOCIMETRO);
}

/*
	Normaliza Leitura de Entrada
*/
double normIn(double in)
{
	return (RPM_MAX*(in-INPUT_MIN))/(INPUT_MAX-INPUT_MIN);
}

/*
	Calcula amperagem a ser aplicada
*/
double algor(double in)
{
  double result = 0;
  // se dentro do dominio valido da funcao
  if(in<RPM_TARGET){
    result = (RPM_TARGET * log(in))/(RPM_PER_AMP * log(RPM_TARGET));
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
	return ((OUTPUT_MAX-OUTPUT_MIN)*in)/AMP_MAX;
}

/*
	Envia para driver
*/
void driv(double in)
{
	analogWrite(PIN_DRIVER, in);
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
