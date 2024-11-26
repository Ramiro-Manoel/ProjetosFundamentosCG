# Trabalho Grau B

## Funcionamento

O programa funciona com uma interface TkInter, para gerar imagens com filtro e adicionar sticker sobre elas. É possível utilizar webcam(apenas filtro) e imagens locais (filtro e sticker), além de poder salvar as edições nas imagens em uma imagem .png local.

OBS: 
* Execute o programa pelo `filter.py`
* É necessário ter as libs: pillow, opencv-python e tkinter

### Opções de imagem

Ao executar o programa, começamos com o modo webcam ativado, porém com os 3 botões superiores da esquerda podemos:

* Carregar uma imagem do computador
* Voltar para o modo webcam
* Salvar a imagem no diretório local

A imagem central da interface gráfica, mostra a imagem que sem filtros que receberá as alterações

#### Imagem local

Podemos abrir uma imagem local pelo navegador de arquivos

#### Webcam

A webcam pode ser fechada apertando a tecla "Q" e salvar a imagem com a tecla "S"

### Filtros

Depois de escolher a utilizar imagem local ou a webcam, temos os 11 botões da esquerda de baixo, os quais aplicam os filtros na imagem selecionada. São eles:

* orginial image
* gray scale
* binarized
* inverted
* blue colored
* green colored
* red colored
* edged
* correlated
* max saturation
* thermal

Ao clicar em algum filtro, uma nova guia irá abrir, com a imagem com o filtro aplicado

### Stickers

Com a imagem aberta, podemos aplicar os sricker (apenas disponível para imagens locais), ao clicar com o botão esquerdo na imagem com o filtro, adicionaremos um sticker a nossa imagem.

É posível escolher entre 6 stickers nos botões da interface gráfica a direta, são eles:

* sunglasses
* wow
* whatsapp
* nerd
* top
* joia

## Sistema 

O sistem foi divido em 4 arquivos:

* [filters](TrabalhoGrauB/filters.py) - funções que aplicam os filtros nas imagens
* [graphic interface](TrabalhoGrauB/graphic_interface.py) - Gerencia a interface gráfica, tanto a do TkInter (inicial), quanto a do cv2 (imagens com filtro e sticker)
* [select_sticker](TrabalhoGrauB/select_sticker.py) - funções que retornam os sticker com o canal alfa
* [main](TrabalhoGrauB/main.py) - executa as a interface gráfica

