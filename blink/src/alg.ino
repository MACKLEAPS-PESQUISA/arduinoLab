#include <math.h>

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

double getC(double rpm, double rpmTarget, double rpmPerAmp = 336)
{
  // se dentro do dominio valido da funcao
  if(rpm<rpmTarget){
    return (rpmTarget * log(rpm))/(rpmPerAmp * log(rpmTarget));
  } else {
    return 0;
  }
}
