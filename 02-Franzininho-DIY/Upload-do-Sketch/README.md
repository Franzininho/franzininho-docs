---
title: Upload do Sketch
---


## Upload do sketch

Dependendo do bootloader gravado na placa, há duas formas de fazer upload do sketch:


## Arduino Gemma


Para fazer o upload para a Franzininho é necessário seguir os passos abaixo:

1 - Certifique que a placa está conectada ao computador:

![](./image1.png)

2 - Pressione o botão de Reset:

![](./image2.png)

3 - Imediatamente em seguida clique no botão de upload na IDE:

![](./image3.png)

4 - É iniciada a programação:

![](./image4.png)

5 - Após alguns instantes a sua Franzininho estará programada:

![](./image5.png)

Maravilha, sua Franzininho foi programada.



## Micronucleus - Disgispark

O processo de gravação para o Micronucleus é um pouco diferente.

1 - Certifique que a placa está conectada ao computador:

![](./image1.png)

2 - Clique no botão de upload na IDE:

![](./image3.png)

 3 - Após a compilação será exibidada seguinte mensagem:

![](./image6.PNG)

Você tem até 60 segundos para inserir a placa na USB do computador.

4 - Plugue a Franzininho na porta USB do seu computador

 Após plugar a placa será iniciado o upload do código:

![](./image7.PNG)

Pronto, a sua Franzininho está programada.

## Solução de problemas:

### (Micronucleus) Erro ao fazer upload:

Caso o erro abaixo (ou parecido) apareça:

```
Warning: device with unknown new version of Micronucleus detected.
This tool doesn't know how to upload to this new device. Updates may be available.
Device reports version as: 2.4
```

Significa que a ferramenta que faz o upload do computador para a placa com o Micronucleos esta desatualizada.

Caso não tenha instalado gcc e o make, instale-os:

> sudo apt install build-essential git

Clone o repositório do micronucleos:

>  git clone git@github.com:micronucleus/micronucleus.git

Compile e substitua o binário antigo pelo atualizado:

```
cd micronucleus/commandline/
make
cp micronucleus /home/<SEU_USUARIO>/.arduino15/packages/digistump/tools/micronucleus/2.0a4/micronucleus
```

Tudo pronto! Deve estar funcionando agora,Tente compilar e fazer o upload novamente.