## Programa Linux para capturar código de barras gerados para pagamentos no sistema bancário do Brasil
<br>
Este programa possibilita a leitura de códigos de barras em exibição no <i>desktop</i> Linux (i.e. captura o código de barras à partir de um <i>sreenshot</i>), se os códigos capturados forem válidos para pagamento no sistema bancário brasileiro (boletos ou convênios) o programa retorna o valor da linha digitável conforme o documento Especificações Técnicas para Confecção de Boleto de Pagamento do Banco do Brasil (<href>https://www.bb.com.br/docs/pub/emp/mpe/espeboletobb.pdf</href>).

O binário pode ser compilado a partir do código fonte C++ utilizando o comando: g++-11 main.cpp -o main -lX11 -lZXing.

Este program utiliza as bibliotecas ZXing-C++ (<href>https://github.com/nu-book/zxing-cpp</href>) e Xlib (<href>https://www.x.org/releases/current/doc/libX11/libX11/libX11.html</href>).

Agradecimentos a Erlend Hamberg pelo código disponibilizado em  <href>https://hamberg.no/erlend/posts/2011-01-06-read-current-x-background-to-jpeg.html</href>.


