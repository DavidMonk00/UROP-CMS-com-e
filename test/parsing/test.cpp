#include <iostream>
#include <string>
#include <math.h>
#include <stdio.h>

std::string convertString(std::string str) {
   int index = str.find(" ")+1;
   std::string unit;
   char s[64];
   if (index) {
      unit = str.substr(str.find(" ")+1);
   } else {
      sprintf(s,"%.014f",std::stof(str));
      str = s;
      return str;
   }

   float x = std::stof(str);
   int p = ((int)log10(x))/3;
   float val = x/pow(1000,p);

   switch (p) {
      case -4:
         sprintf(s,"%.02f p", val);
         break;
      case -3:
         sprintf(s,"%.02f n", val);
         break;
      case -2:
         sprintf(s,"%.02f u", val);
         break;
      case -1:
         sprintf(s,"%.02f m", val);
         break;
      case 0:
         sprintf(s,"%.02f ", val);
         break;
      case 1:
         sprintf(s,"%.02f k", val);
         break;
      case 2:
         sprintf(s,"%.02f M", val);
         break;
      case 3:
         sprintf(s,"%.02f G", val);
         break;
      case 4:
         sprintf(s,"%.02f T", val);
         break;
      case 5:
         sprintf(s,"%.02f P", val);
         break;
      case 6:
         sprintf(s,"%.02f E", val);
         break;
   }
   str = s + unit;
   return str;
}

int main(void) {
   std::string str = convertString("1.33e+12");
   std::cout << str << '\n';
   return 0;
}
