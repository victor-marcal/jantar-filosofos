# Jantar dos Filósofos

O Problema do Jantar dos Filósofos é um clássico exercício de concorrência proposto por E. W. Dijkstra. Consiste em cinco filósofos sentados em uma mesa redonda, alternando entre pensar e comer. Entre cada filósofo há um garfo compartilhado. Para comer, um filósofo precisa de dois garfos: o da sua esquerda e o da sua direita.

Este problema ilustra a necessidade de sincronização em sistemas concorrentes, pois uma implementação ingênua pode levar a dois problemas críticos:

- **Deadlock:** Todos os filósofos pegam um garfo e esperam infinitamente pelo segundo, impedindo o avanço de qualquer um;
- **Starvation:** Um ou mais filósofos nunca conseguem os dois garfos, sendo constantemente preteridos.

## A Solução Implementada

Esta implementação em C, utilizando Pthreads e Semáforos, resolve o problema de concorrência com base na estratégia de controle de estados. Um filósofo só pode começar a comer se ambos os vizinhos não estiverem comendo.

Recursos de Sincronização

1. **sem_t mutex (Semáforo Binário):** Usado para garantir o acesso exclusivo à região crítica (as funções take_forks e put_forks). Atua como um lock global para operações que modificam o estado.
2. **sem_t s[N] (Semáforo por Filósofo):** Um vetor de semáforos, onde cada semáforo é associado a um filósofo. É usado para bloquear um filósofo (com sem_wait) se ele estiver HUNGRY mas não puder comer (porque os vizinhos estão comendo). É liberado (com sem_post) somente quando a função test() indica que ele pode comer.

### Estados dos Filósofos

O estado de cada filósofo é mantido no vetor **state[N]**:

| Constante | Valor | Descrição |
| :--- | :--- | :--- |
| THINKING | 0 | O filósofo está pensando e liberou seus garfos. |
| HUNGRY | 1 | O filósofo está com fome e tenta pegar os garfos. |
| EATING | 2 | O filósofo está comendo e possui ambos os garfos. |

### Funções Principais

| Função | Descrição | Prevenção de Problemas |
| :--- | :--- | :--- |
| `take_forks(i)` | Entra na região crítica (`sem_wait(&mutex)`), muda o estado para **HUNGRY** e chama `test(i)`. Se o `test` falhar, o filósofo é bloqueado em seu semáforo individual (`sem_wait(&s[i])`). | Evita deadlock ao garantir que um filósofo só come se os vizinhos estiverem livres. |
| `test(i)` | **Condição de Comer:** Verifica se o filósofo `i` está **HUNGRY** e se seus vizinhos (`LEFT` e `RIGHT`) **não** estão **EATING**. Se a condição for verdadeira, o estado muda para **EATING** e o semáforo individual `s[i]` é liberado (`sem_post`). | É o coração da prevenção de deadlock e starvation (ajudando vizinhos a comerem quando os garfos estão livres). |
| `put_forks(i)` | Entra na região crítica, muda o estado para **THINKING**, e chama `test(LEFT)` e `test(RIGHT)` para verificar se os vizinhos podem comer agora. | Permite que os vizinhos bloqueados sejam liberados assim que os garfos são devolvidos. |


## Como Compilar e Executar

Este projeto requer o compilador GCC e a biblioteca Pthreads.

1. Compilação:

```bash
    gcc jantar-filosofos.c -o jantar-filosofos -pthread
```

2. Execução:

```bash
    ./jantar-filosofos
```



