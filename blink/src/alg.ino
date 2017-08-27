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

/*
 * Para que o processador não precise fazer essas sempre...
 */
const double PID_CONST = RPM_TARGET / (RPM_PER_AMP * log(RPM_TARGET));
const double AMP_NORM = (OUTPUT_MAX - OUTPUT_MIN) / AMP_MAX;
const double RPM_NORM = RPM_MAX / (INPUT_MAX - INPUT_MIN);

/*
 * Funções de normalização de entrada e saída. Por se tratarem de cálculos
 * breves retornados diretamente, é melhor deixar claro para o compilador que as
 * rotinas podem ser substituídas `inline'.
 */
static inline double rpm_norm(double rpm)
{
	return RPM_NORM * (rpm - INPUT_MIN);
}

static inline double amp_norm(double amp)
{
	return AMP_NORM * amp;
}

/*
 * Calcula a amperagem a ser aplicada ao motor baseando-se na atual velocidade
 * angular da roda, medida externamente. Fundamenta-se em um controlador PID,
 * aplicando uma corrente em função de um erro.
 *
 * A fórmula obtida para esse caso é:
 *
 * RPM_ALVO * log(RPM_ATUAL)/(RPM_POR_AMPERE * log(RPM_ALVO))
 */
double pid(double rpm)
{
	if (rpm < RPM_TARGET)
		return PID_CONST * log(rpm);
	else
		return 0;
}

void setup()
{
	pinMode(PIN_VELOCIMETRO, INPUT);
	pinMode(PIN_DRIVER, OUTPUT);
}

void loop()
{
	double amp;

	amp = rpm_norm(analogRead(PIN_VELOCIMETRO));
	amp = amp_norm(pid(amp));

	analogWrite(PIN_DRIVER, amp);
}
