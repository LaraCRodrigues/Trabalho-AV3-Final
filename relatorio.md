# 📚 Portfólio de Estrutura de Dados em C

Repositório contendo os projetos práticos desenvolvidos para a disciplina de Estrutura de Dados. Todos os programas utilizam a linguagem C e aplicam conceitos de persistência de dados em arquivos CSV.

## 👥 Identificação
* **Integrantes:** [Lara Cibele Rodrigues de Jesus], [Carlos Vinicius Dias Souza],[Ruan Alberto Santana Cirqueira] e [Aquiles Maciel Amaral].
* **Turma:** [Turma C]

---

## 🎯 Temas Escolhidos

| Trabalho | Estrutura de Dados | Tema Aplicado |
| :--- | :--- | :--- |
| **Trabalho A** | Lista Linear Simples | [Playlist de Musica] |
| **Trabalho B** | Pilha Simples | [ Bandeja de E-mails Não Lidos] |
| **Trabalho C** | Fila Linear e Fila Circular | Simulador de Spool de Impressão Corporativo |

---

## 📖 Explicação Breve de Cada Programa

### Trabalho A (Lista Simples)
O programa gerencia uma lista linear estática baseada no tema escolhido. Permite a inserção de novos registros, listagem de todos os dados armazenados, busca específica e remoção física reorganizando o vetor.

### Trabalho B (Pilha Simples)
Implementa a lógica LIFO (Last In, First Out). O sistema permite empilhar novos registros, desempilhar o elemento do topo e visualizar a pilha atual, simulando perfeitamente o comportamento do tema escolhido com limites estáticos.

### Trabalho C (Filas de Impressão)
O simulador gerencia duas estruturas dinâmicas e encadeadas em paralelo. A Fila Prioritária (implementada como lista circular encadeada) possui precedência absoluta sobre a Fila Normal (implementada como lista linear encadeada). O programa conta com menu interativo, cancelamento por ID e geração de estatísticas em tempo real.

---

## 🚀 Instruções para Compilar

Certifique-se de ter o compilador GCC instalado no seu ambiente (Windows com MinGW ou Linux/macOS nativo). Abra o terminal na pasta raiz do repositório e execute os comandos de compilação separadamente para cada trabalho:

* **Trabalho A:** `gcc trabalho_a.c -o trabalho_a`
* **Trabalho B:** `gcc trabalho_b.c -o trabalho_b`
* **Trabalho C:** `gcc main.c -o trabalho_c`

---

## ▶️ Instruções para Executar

Após a compilação bem-sucedida, inicie os programas pelo terminal utilizando os comandos abaixo correspondentes ao seu sistema operacional:

* **No Windows:**
  * Trabalho A: `trabalho_a.exe`
  * Trabalho B: `trabalho_b.exe`
  * Trabalho C: `trabalho_c.exe`

* **No Linux / macOS:**
  * Trabalho A: `./trabalho_a`
  * Trabalho B: `./trabalho_b`
  * Trabalho C: `./trabalho_c`

---

## ⚙️ Observações sobre o Funcionamento

* **Persistência de Dados:** Todos os três programas geram e consomem arquivos `.csv` automaticamente na mesma pasta do executável para salvar o estado entre as execuções.
* **Alocação Dinâmica (Extra):** O Trabalho C foi inteiramente refatorado para utilizar alocação dinâmica de memória com ponteiros (`malloc` e `free`), removendo limites físicos de tamanho da fila.
* **Interface e Cores (Extra):** O Trabalho C utiliza códigos ANSI de escape para colorir o terminal e proporcionar uma experiência mais amigável, além de painéis gerenciais e limpeza automática de tela.

---

## 🚧 Dificuldades Encontradas

* [A maior dificuldade do grupo foi adequar a lógica de remoção na Fila Circular dinâmica, garantindo que o ponteiro do último elemento sempre voltasse para o início sem gerar vazamento de memória.]
* [Também enfrentamos desafios na manipulação de strings durante a leitura dos arquivos CSV usando `fscanf`, necessitando de ajustes nos delimitadores, e a aplicacao de propriedades pra que o codigo consiga ser reproduzido nas plataformas linux.]
