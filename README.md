# Math2Tex

Dedicado a realização de decodificação de arquivos Mathcad prime 5 e a tradução via scripts lua para arquivos de Tex/LaTex. Visando a facilitação de escrita de documentos se utilizando do poder do Mathcad.

### Idéia

O programa tem em mente que o usuário faça uma folha de cálculo mathcad, salve a, e então chame o programa especificando um script escrito em lua que contém as definições de como o usuário deseja que programa escreva o arquivo latex de saída. O programa interpreta o arquivo original e gera expressões intermediárias que seram lidas pelo script e então interpretadas e escritas em código latex para uso. Esse arquivo pode então ser inserido em outros arquivos latex, ou o script permita que se gere um latex completo na saída.

O script pode ser editado em lua e contém definições de como cada operação interna deve ser interpretada. Essas operações internas ou espressões intermediárias são definidas no arquivo [function_def.txt](./function_def) disponível no repositório acima. Explicação mais a funda pendente...

O programa foi escrito em C com o uso de duas bibliotecas, [zuba--/zip](https://github.com/kuba--/zip) disponível no github, usada para descompactação programática dos arquivos e biblioteca [lua 5.3](http://www.lua.org/), domínio público licenciada. Foi compilado estáticamente e por isso é portátil.

# Downloads

[Clique aqui para downloads!](https://github.com/Joao-Peterson/Math2Tex/releases)

Na aba do github "Releases" há em cada versão, mais abaixo, uma caixa de nome assets como um arquivo, ".tar.gz", compactado que pode ser baixado e descompactado com o programa pré compilado e pronto para uso.

Disponível para:

* Windows 64 bits

# Como utilizar

O programa é de linha de comando, o que significa que você deve abrir um terminal como CMD do windows, Git Bash, MSYS, ou outro e chamar o programa repassando os argumentos, o programa deve estar presente nas váriaveis de ambiente, ou copiado para o diretório onde deseja se realizar as operações. As opção que devem ser repassadas podem ser encontradas chamando o programa com a opção de ajuda "-h". Ex:

```c
    >> m2tex.exe -h
```

Com a opção "-f" pode se especificar quais arquivos mathcad você deseja usar, "-s" para especificar o script lua desejado, assim produindo um arquivo latex de saída para cada entrada. Opcionalmente pode se incluir com -o um arquivo específico de saída para todos os arquivos de entrada, traduzindo todos em apenas um arquivos de saída latex, ".tex".

Se deseja se que imagens sejam incorporadas no latex, como gráficos, plotagens, imagens inseridas no mathcad, deve se utilziar a opção "-i" que indica que deseja se utilizar imagens, porém deve se tomar cuidado, uma vez que se faz necessário que um arquivo rich text format ".rtf" esteja presente e de memso nome que o arquivo mathcad, para isso, salve seu arquivo mathcad, agora vá em: salvar como -> rich text format, e só então deve se utilziar o programa.

### Exemplo

Deseja se que duas folhas de cálculo gerem um arquivo de saída com uso de imagens, após salvar ambos arquivos e também salvar ambos em formato .rtf sobre o memso diretório, pode se invocar o programa como:

```c
    >> m2tex.exe -f folha1.mcdx -f folha2.mcdx -s scripts_latex/type.lua -o saida_latex.tex
```

Onde o diretório e arquivo "scripts_latex/type.lua" são de escolha livre do usuário, bem como os nomes e diretório dos arquivos.

Caso o processo acima seja repetitivo demais ao longo da criação e salvamento de documentos, é indicado o uso de um arquivo ".bat" como incluido com o pacote de releases [_RUN_M2TEX_.bat](./scripts/_RUN_M2TEX_.bat), onde escrevendo as opções desejadas na linha de texto, pode se invocar o comando com dois cliques sobre o arquivo.

### Script

O programa já vem com um corpo de script, [type.lua](scripts/type.lua) pronto para ser usado e modificado.

# Bugs e erros

Caso experiemente incoveniêniacias e erros de uso, favor criar um tópicos na aba [Issues](https://github.com/Joao-Peterson/Math2Tex/issues), existe uma opção no programa chamada "-v" onde pode se observar o progresso em tempo real do programa, recomendado utilizar quando em linha de comando, e não por chamada como no arquivo ".bat". Também existem "-d" e "-c", "-d" que produz um arquivo ".log" de saída que pode ser anexado ao cadastrar o tópico e possui informações triviais ao longo do processo do programa, "-c" deixa para trás uma pasta de mesmo nome que o arquivo mathcad e é o próprio arquivo descompactado, também pode ser anexado ao tópico caso o usuário consentir com o fato de o conteúdo de sua folha de cálculo será exposto publicamente na internet.

# Contribuição

Para contribuições leia "CONTRIBUTE.md". Obs. A se escrever.