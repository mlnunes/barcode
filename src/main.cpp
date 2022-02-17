//Lê e decodifica um código de barras visível na tela
// g++-11 main.cpp -o main -lX11 -lZXing


#include <ZXing/ReadBarcode.h>
#include <ZXing/TextUtfEncoding.h>
#include <X11/Xutil.h>
#include <iostream>
#include <string.h>

using namespace std;
using namespace ZXing;
using namespace TextUtfEncoding;


char calc_dv(string campo, int mul){
    int acc = 0;
    int dv = 0;

    for (char i : campo){
        acc = ((int)(i) - 48) * mul;
        if (acc < 10){
            dv += acc;
        } else{
            dv += ((acc - acc % 10) / 10) + acc % 10;
        }
        if (mul == 2){ mul = 1;} else { mul = 2;};
    }
    if (dv > 10){
        dv = ((((dv / 10) + 1) * 10) - dv % 10) % 10 + 48;
    } else {
        dv = (10 - dv)  + 48;
    }
    return ((char)(dv));
}

int main(void){
  unsigned long pixel;
  int x, y;
  string barras;
  char dig;
  string campo;
  unsigned char* buffer;
  DecodeHints hints;
  XWindowAttributes attr;

  Display *dsp = XOpenDisplay(NULL);
  Window root = RootWindow(dsp, DefaultScreen(dsp));
  XGetWindowAttributes(dsp, root, &attr);
  hints.setEanAddOnSymbol(EanAddOnSymbol::Read);

  auto width = attr.width;
  auto height = attr.height;
  
  //Captura os dados da imagem da tela
  XImage* imgmn = XGetImage(dsp, root, 0 , 0, width, height, AllPlanes, ZPixmap);
  
  //Cria um buffer de imagem com a imagem da tela
  buffer = (unsigned char*) malloc(sizeof(char)*3*imgmn->width*imgmn->height);
  for (y = 0; y < imgmn->height; y++) {
          for (x = 0; x < imgmn->width; x++) {
                      pixel = XGetPixel(imgmn,x,y);
                      buffer[y*imgmn->width*3+x*3+0] = (char)(pixel>>16);
                      buffer[y*imgmn->width*3+x*3+1] = (char)((pixel&0x00ff00)>>8);
                      buffer[y*imgmn->width*3+x*3+2] = (char)(pixel&0x0000ff);
          }
  }

  ImageView img{buffer, imgmn->width, imgmn->height, ImageFormat::RGB};
  auto results = ReadBarcodes(img, hints);
  for (auto&& result : results){
    if (result.isValid()){
      barras = ToUtf8(result.text());
      if (barras.size() != 44){
        cout << "Código de barras identificado não é válido para o sistema bancário brasileiro.\nNão possui o número de dígitos necessários."  << endl;
      }else if (barras[0] != '8'){
        //Campo1
        campo = barras.substr(0,3) + barras[3] + barras[20] + barras.substr(21,4);
        dig = calc_dv(campo, 2);
        string campo1 = campo.substr(0,5) + "." + campo.substr(5, 4) + dig;
        //Campo2
        campo = barras.substr(24,5) + barras.substr(29,5);
        dig = calc_dv(campo, 1);
        string campo2 = campo.substr(0,5) + "." + campo.substr(5, 5) + dig;
        //Campo3
        campo = barras.substr(34,5) + barras.substr(39,5);
        dig = calc_dv(campo, 1);
        string campo3 = campo.substr(0,5) + "." + campo.substr(5, 5) + dig;
        //Campo4
        auto campo4 = barras[4];
        //Campo5
        string campo5 =  barras.substr(5,4) + barras.substr(9,10);
        cout  << "Linha digitável: "
              << campo1 << " "
              << campo2 << " "
              << campo3 << " "
              << campo4 << " "
              << campo5 << endl;
      } else{
        string campo_convenio;
        cout  << "Linha digitável: ";
        for (int i = 0; i <= 33; i= i + 11){
          campo_convenio = barras.substr(i, 11);
          dig = calc_dv(campo_convenio, 2);
          cout << campo_convenio << "-" << dig << " ";
        }
        cout << endl;
      }
      std::cout << "Código de barras: ";
      std::cout << ToUtf8(result.text()) << endl;
    }
  }

  free(buffer);
  return 0;
}