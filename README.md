# jantar-filosofos
Implementação do Problema do Jantar dos Filósofos em C utilizando a biblioteca Pthreads para concorrência e semáforos para sincronização. A solução implementa um mecanismo de controle de estados (THINKING, HUNGRY, EATING) coordenado por um semáforo mutex para prevenir deadlock e starvation, garantindo o uso compartilhado e seguro dos garfos.
