#include <math.h>

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

double getC(double rpm, double rpmTarget, double rpmPerAmp = 336)
{
    return (rpmTarget * log(rpm))/(rpmPerAmp * log(rpmTarget));
}
