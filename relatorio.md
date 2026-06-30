# Trabalho-AV3-Final
Trabalho da Av3 de estrutura de dados pela Instituicao Unijorge

# Simulador de Spool de Impressão Corporativo

Trabalho prático desenvolvido para a disciplina de Estrutura de Dados em Linguagem C. O sistema simula o gerenciamento de uma fila de impressão de escritório dividida entre trabalhos normais e prioritários.

## 👥 Integrantes do Grupo
* [Lara Cibele Rodrigues de Jesus] - [2250100827]
* [Carlos Vinicius Dias Souza] - []
* [] - []
* [] - []
  
## 📌 Detalhes de Implementação por Trabalho

### 📂 Trabalho A — Lista Simples
* **Conceito:** Armazenamento linear onde a inserção, busca e remoção podem ocorrer em qualquer posição (índice) da estrutura, dependendo da lógica do negócio.
* **Funcionalidades:** Cadastro de registros, consulta detalhada, alteração, listagem completa de dados e remoção física com rearranjo do vetor.
* **Tratamento de Limites:** Validação de lista cheia (`MAX`) e lista vazia.

### 📂 Trabalho B — Pilha Simples (LIFO - Last In, First Out)
* **Conceito:** Estrutura restrita onde toda inserção (`Push`) e toda remoção (`Pop`) ocorrem exclusivamente no topo da estrutura.
* **Funcionalidades:** Empilhar novos registros, desempilhar o elemento mais recente, consultar o elemento que está no topo e listar a pilha do topo até a base.
* **Tratamento de Limites:** Controle de *Stack Overflow* (pilha cheia) e *Stack Underflow* (pilha vazia).

### 📂 Trabalho C — Filas de Impressão (FIFO - First In, First Out)
* **Conceito:** Duas estruturas distintas trabalhando em paralelo para gerenciar prioridades de hardware:
  * **Fila Normal:** Fila linear estática com deslocamento de memória a cada remoção.
  * **Fila Prioritária:** Fila circular usando aritmética modular (`% MAX`) para reaproveitamento de índices.
* **Regra de Negócio:** Trabalhos na Fila Prioritária possuem precedência absoluta de processamento sobre a Fila Normal.

---

## 💾 Padrão de Persistência (CSV)
Todos os programas salvam e carregam seus estados automaticamente ao iniciar e fechar o sistema. Os dados são estruturados usando o caractere ponto e vírgula (`;`) como delimitador padrão:
```text
ID;Nome_do_Registro;Quantidade;Tipo_ou_Status
101;Arquivo_Teste.pdf;12;P
