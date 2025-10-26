# 🌱 Sistema ECOENG

Sistema desenvolvido como trabalho acadêmico para a disciplina de Engenharia de Software / Projeto Integrador.  
O objetivo é simular o funcionamento de um sistema de **gerenciamento para o evento ECOENG**, aplicando conceitos de **modularização em C**, **persistência de dados** e **organização de código com Makefile**.

---

## 🧩 Estrutura do Projeto

TRABALHO_ECOENG/
│
├── .vscode/                      # Configurações locais do VS Code
│
├── Arquivos_csv/                 # Base de dados do sistema (arquivos persistentes)
│   ├── equipes.csv               # Dados das equipes cadastradas
│   └── users.csv                 # Dados dos usuários (login/senha)
│
├── bin/                          # Executáveis gerados pela compilação (make)
│
├── Cadastro/                     # Módulos de cadastro do sistema
│   ├── Cadastro_Equipes/
│   │   ├── Cadastro_Equipes.c
│   │   └── Cadastro_Equipes.h
│   │
│   └── Cadastro_user/
│       ├── User.c
│       └── User.h
│
├── Desafio_robo_Pontuacao/       # Módulo responsável por pontuações e desafios
│   ├── Pontuacao.c
│   └── Pontuacao.h
│
├── Funcoes_cvs/                  # Funções genéricas para manipulação de arquivos CSV
│   ├── Dados.c
│   └── Dados.h
│
├── lib/                          # (Reservado) possíveis bibliotecas externas ou estáticas
│
├── Menus/                        # Interface textual e interação com o usuário
│   ├── Menu_Avaliadores/
│   ├── Menu_Equipes/
│   ├── Menu_principal/
│   └── Menu_professores/
│
├── obj/                          # Objetos de compilação (.o)
│
├── Utilidades/                   # Funções auxiliares e ferramentas extras
│   ├── FILES/
│   └── Rsult/
│
├── .gitignore                    # Regras para ignorar arquivos desnecessários no Git
│
├── Estrutura_Do_Projeto.txt      # Documento explicando a organização dos módulos
│
├── main.c                        # Função principal do sistema (ponto de entrada)
│
└── makefile                      # Script de build automatizado


---

## ⚙️ Compilação e Execução

### 🧱 Pré-requisitos
- [GCC (MinGW)](https://www.mingw-w64.org/) ou outro compilador C
- [Make](https://gnuwin32.sourceforge.net/packages/make.htm)
- [VS Code](https://code.visualstudio.com/) com a extensão **C/C++** da Microsoft instalada

---

### 🚀 Compilar o projeto
No terminal do VS Code (dentro da pasta `TRABALHO_ECOENG`):

```bash
make
Isso criará os arquivos objeto em obj/ e o executável em bin/.

▶️ Executar o sistema
Após compilar, execute:
./bin/sistema_ecoeng.exe
(ou apenas sistema_ecoeng.exe se estiver na raiz do projeto)

🧹 Limpar arquivos de compilação
Para remover os objetos e recompilar do zero:
make clean

🧠 Conceitos Aplicados
Modularização de código em C
Manipulação e leitura de arquivos CSV
Persistência de dados sem banco de dados relacional
Sistema de cadastro, login e pontuação
Organização de projeto com Makefile
Boas práticas de versionamento com Git e GitHub

👨‍💻 Autor
Luis Henrique (Henrique)
🎓 Estudante de Engenharia de Software — FUCAPI
🔗 GitHub - @LuisHenrique0909

🧾 Licença
Este projeto é de uso educacional.
Você pode estudar, modificar e reutilizar o código, desde que mantenha os devidos créditos.
