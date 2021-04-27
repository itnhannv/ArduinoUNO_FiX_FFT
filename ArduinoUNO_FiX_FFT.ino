#include "fix_fft.h"                                  // library to perfom the fixed-point in-place Fast Fourier Transform
#include "ST7565_homephone_esp8266.h"
#define SAMPLES 128
#define audioIn A0

char re[SAMPLES], im[SAMPLES]; // real and imaginary FFT result arrays
int ouput[SAMPLES];
byte ylim = 64;                                       // OLED y-axis drawing boundary limit
int maxOUTPUT = 0;
unsigned long newTime;
ST7565 lcd(7, 5, 6, 4);
void writeString(int x, int y, String chuoi)
{ //
  lcd.Asc_String(x, y, (char *)chuoi.c_str(), BLACK);
}
void display_wellcome()
{
  lcd.Clear();
  writeString(32, 45, "AUDIO ANANYZER");
  writeString(30, 55, "by Nhan.pro41");
  lcd.display();
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  lcd.ON();
  lcd.SET(20, 0, 0, 0, 4);
  //display_wellcome();
  analogReference(INTERNAL);
  delay(2000);
}
void loop() {
  newTime = micros();
  // put your main code here, to run repeatedly:
lcd.Clear();
  for (byte i = 0; i < SAMPLES; i++) {                    // read 128 analog input samples from ADC
    int sample = analogRead(audioIn);
    re[i] = sample / 4 - 128;                         // scale the samples to fit within a char variable
    im[i] = 0;                                        // there are no imaginary samples associated with the time domain so set to 0
  };

  fix_fft(re, im,7, 0);                              // send the samples for FFT conversion, returning the real/imaginary results in the same arrays
  for (byte i = 1; i < SAMPLES/2 ; i++) {
   // int dat = sqrt(re[i] * re[i] + im[i] * im[i]) + sqrt(re[i+1] * re[i+1] + im[i+1] * im[i+1]) + sqrt(re[i+2] * re[i+2] + im[i+2] * im[i+2]);  
   int dat = sqrt(re[i] * re[i] + im[i] * im[i]);
    // frequency magnitude is the square root of the sum of the squares of the real and imaginary parts of a vector
    
    dat  = (dat-1);
    // display.drawLine(i * 2, ylim, i * 2, ylim - dat, WHITE); // draw bars for each frequency bin from 0 Hz to 4.5 KHz
    lcd.DrawLine(i * 2, 64, i * 2, ylim - dat, BLACK);
    lcd.DrawLine(i * 2 - 1, 64, i * 2 - 1, ylim - dat, BLACK);
    //lcd.DrawLine(i, 64, i, ylim - dat, BLACK);
  };

  lcd.display();
  float conversionTime = micros() - newTime;

  Serial.print("Conversion time per cycle: ");
  Serial.print(conversionTime);
  Serial.println(" uS");
}
