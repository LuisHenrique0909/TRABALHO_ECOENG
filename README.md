TRABALHO_ECOENG

Projeto desenvolvido para o trabalho da disciplina de EEstrutura de Dados, com foco na criação de um sistema modular em linguagem C, organizado em múltiplos menus e funcionalidades específicas para o gerenciamento de equipes, usuários e resultados de um desafio de robótica.

📂 Estrutura do Projeto
Abaixo está a organização principal do projeto conforme a estrutura de pastas:

TRABALHO_ECOENG/
│
├── bin/                        # Pasta para os arquivos compilados
│
├── Cadastro/
│   ├── Cadastro_Equipes/       # Módulo de cadastro de equipes
│   └── Cadastro_user/          # Módulo de cadastro de usuários
│
├── dados/                      # Base de dados em CSV
│   ├── equipes.csv
│   ├── resultados_robo.csv
│   └── users.csv
│
├── Desafio_robo/               # Lógica e dados relacionados ao desafio de robôs
│
├── Funcoes_admin/              # Funções administrativas e de controle
│
├── Menus/                      # Menus principais do sistema
│   ├── Menu_avaliador/
│   │   ├── Menu_avaliador.c
│   │   └── Menu_avaliador.h
│   │
│   ├── Menu_Equipes/
│   │   ├── Menu_Equipes.c
│   │   └── Menu_Equipes.h
│   │
│   ├── Menu_Ranking/
│   │   ├── Ranking.c
│   │   └── Ranking.h
│   │
│   ├── Menu_principal/         # Menu inicial e navegação geral
│   
│
├── Utilidades/                 # Funções auxiliares reutilizáveis
│   ├── files/
│   │   ├── files.c
│   │   └── files.h
│   │
│   └── Result/
│       ├── Result.c
│       └── Result.h
│
├── .gitignore                  # Arquivos ignorados pelo Git
├── main.c                      # Ponto de entrada do programa
├── makefile                    # Script de compilação
└── README.md                   # Documentação do projeto

🧩 Descrição dos Módulos
🟢 Cadastro
Contém os módulos responsáveis por cadastrar equipes e usuários.
Cadastro_Equipes: gerencia o registro e edição das equipes participantes.
Cadastro_user: gerencia o cadastro e autenticação dos usuários.

📊 dados
Contém os arquivos .csv usados como base de dados do sistema.
Eles armazenam informações persistentes como:
equipes.csv → dados das equipes
resultados_robo.csv → pontuações e desempenho
users.csv → usuários cadastrados

⚙️ Funcoes_admin
Módulo de funções administrativas, permitindo controle e manutenção dos registros.

🧭 Menus
Contém a interface textual do sistema, dividida em módulos:
Menu_principal: menu inicial e controle de fluxo.
Menu_Equipes: opções de gerenciamento de equipes.
Menu_avaliador: menu destinado aos avaliadores.
Menu_Ranking: exibe e organiza o ranking das equipes.
fluxograma.txt: documentação com o fluxo lógico das telas e menus.

🧰 Utilidades
Funções de suporte utilizadas em diversos módulos:
files: leitura e escrita de arquivos .csv
Result: manipulação de resultados e cálculos de pontuação.

⚡ Como Compilar

O projeto usa um Makefile para facilitar a compilação.
No terminal, dentro da pasta principal do projeto, execute: make
Isso irá compilar todos os arquivos .c e gerar o executável na pasta bin/.

Para limpar os arquivos compilados: make clean

▶️ Como Executar
Após compilar, execute o programa principal: ./bin/TRABALHO_ECOENG

👨‍💻 Tecnologias Utilizadas
Linguagem: C
Compilador: GCC
Organização: Estrutura modular com cabeçalhos (.h) e implementações (.c)
Persistência: Arquivos .csv

🧠 Objetivo do Projeto
O sistema tem como objetivo simular o gerenciamento de um Desafio de Robótica, permitindo:
Cadastro e controle de equipes;
Avaliação de desempenho;
Exibição de rankings;
Administração de usuários e resultados.

🧾 Autor
Nome: Luis Henrique
Curso: Engenharia de Software
Instituição: FUCAPI