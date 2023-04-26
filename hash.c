  #include "hash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

typedef struct sincronismo{
    pthread_mutex_t *alteracao;
    pthread_mutex_t *mutex_counter;
    pthread_cond_t *cond_counter;
    unsigned long int counter;
}sincronismo_t;


hash_t * ht_init(unsigned long tam){
    int x, i = 0;
    hash_t *ht = NULL;

    if(tam > 0){
        ht = calloc(1,sizeof(hash_t));
        ht->armazenamento = malloc(sizeof(entrada_hash_t)*tam);
        ht->numero_elementos = tam;

        //-----------------------------------------------------
        sincronismo_t *sincron = malloc(sizeof(sincronismo_t));             //Cria uma STRUCT para o sincronismo
        sincron->alteracao = malloc(sizeof(pthread_mutex_t)*ceil(tam*0.1)); //Aloca 10% em relação ao tamanho da tabela
        sincron->cond_counter = malloc(sizeof(pthread_cond_t));       //
        sincron->mutex_counter = malloc(sizeof(pthread_mutex_t));

        for(x=0;x<(int)ceil(tam*0.1);x++){
            pthread_mutex_init(&(sincron->alteracao[x]),NULL);  //NULL is default
        }
        pthread_cond_init(sincron->cond_counter,NULL);
        pthread_mutex_init(sincron->mutex_counter, NULL);
        //-----------------------------------------



        while(i < tam) // Inicializando todas entradas hash
        {
            ht->armazenamento[i].chave = 0;
            ht->armazenamento[i].conteudo = NULL;
            if(i == tam - 1){
                ht->armazenamento[i].proximo = NULL;
            }
            else{
                ht->armazenamento[i].proximo = &ht->armazenamento[i+1];
            }
            i++;
        }
        ht->sincronizacao = sincron;
    }
    return ht;
}

void ht_remove(hash_t * ht){
  int pos = 0;
  while(pos < ht->numero_elementos){
    free(&ht->armazenamento[pos]);
    pos++;
  }
  free(ht);
}

chave_t ht_hash(hash_t *ht, conteudo_t * conteudo){

    int i = 0; // inutilizado
    unsigned long int valor = *conteudo; // Indica a posĩção a ser salva

    valor = valor % ht->numero_elementos;
    return valor;
}

entrada_hash_t * ht_busca_chave(hash_t * ht, chave_t chave){
    unsigned long int indice;
    entrada_hash_t *entrada = NULL;
    //-------------------------------------------------------
    indice = (int)floor(chave*0.1);
    sincronismo_t *sincron = (sincronismo_t*)ht->sincronizacao;


    pthread_mutex_lock(&(sincron->alteracao[indice]));          //Verifica se está ocorrendo insert ou remove
    sincron->counter = sincron->counter + 1;
    pthread_mutex_unlock(&(sincron->alteracao[indice]));
    //--------------------------
    // -------- Incrementa contador de threads que estao fazendo
    // -------- leitura na tabela hash
    //pthread_mutex_lock(sincron->mutex_counter);                 //Usado p bloquear insert/remove
    //sincron->counter = sincron->counter + 1;                    //Ambos vão seguir após busca terminar
    //pthread_mutex_unlock(sincron->mutex_counter);

    //------------------------------------------------------
    // SAFE ZONE: Posso consultar a tabela hash
    //------------------------------------------------------
    if(ht->armazenamento[chave].chave){                     // Possui a chave e portanto o conteúdo
        pthread_mutex_unlock(&(sincron->alteracao[indice]));
        entrada = &ht->armazenamento[chave];
    }

    // -------- Decrementa contador de threads que estao fazendo
    // -------- leitura na tabela hash
    //pthread_mutex_lock(&(sincron->alteracao[indice]));
    sincron->counter = sincron->counter - 1;
    if(sincron->counter == 0){                                  //Enquanto tiver busca em processo, insert/remove
        //pthread_cond_signal(&(sincron->cond_counter[indice]));    //Não podem continuar
        pthread_cond_signal(sincron->cond_counter);
    }
   // pthread_mutex_unlock(&(sincron->alteracao[indice]));
    //------------------------------------------------------

    return entrada;
}

entrada_hash_t * ht_busca_conteudo(hash_t * ht, conteudo_t * conteudo){
    unsigned long int pos, indice, lock;
    entrada_hash_t *entrada = NULL;
    pos = ht_hash(ht, conteudo);

    //-----------------------------------
    indice = (int)floor(pos*0.1);                //Calcula o indice de qual mutex será utilizado
    sincronismo_t *sincron = (sincronismo_t*)ht->sincronizacao; //Usado pq consigo atribuir argumentos a um void, más n consigo acessar direto
    //--------------------------
    pthread_mutex_lock(&(sincron->alteracao[indice]));          //Verifica se está ocorrendo insert ou remove
    sincron->counter = sincron->counter + 1;
    pthread_mutex_unlock(&(sincron->alteracao[indice]));
    //--------------------------
    //pthread_mutex_lock(sincron->mutex_counter);                 //Usado p bloquear insert/remove
    //sincron->counter = sincron->counter + 1;                    //Ambos vão seguir após busca terminar
    //pthread_mutex_unlock(sincron->mutex_counter);

    //-----------------------------------
    // SAFE ZONE: Posso consultar a tabela hash
    //-----------------------------------
    if(&ht->armazenamento[pos] != NULL){
        if(*ht->armazenamento[pos].conteudo == *conteudo){
            entrada = &ht->armazenamento[pos];
        }
    }
    // -------- Decrementa contador de threads que estao fazendo
    // -------- leitura na tabela hash

    //pthread_mutex_lock(&(sincron->alteracao[indice]));
    sincron->counter = sincron->counter - 1;
    if(sincron->counter == 0){                                  //Enquanto tiver busca em processo, insert/remove
        //pthread_cond_signal(&(sincron->cond_counter[indice]));    //Não podem continuar
        pthread_cond_signal(sincron->cond_counter);
    }
   // pthread_mutex_unlock(&(sincron->alteracao[indice]));
    //------------------------------------------------------

    return entrada;
}

int ht_insere_conteudo(hash_t * ht, conteudo_t * conteudo){
    unsigned long int pos, indice;
    pos = ht_hash(ht, conteudo);

    //-----------------------------------
    indice = (int)floor(pos*0.1);                //Calcula o indice de qual mutex será utilizado
    sincronismo_t *sincron = (sincronismo_t*)ht->sincronizacao;//Usado pq consigo atribuir argumentos a um void, más n consigo acessar direto

    pthread_mutex_lock(&(sincron->alteracao[indice]));

    //pthread_mutex_lock(sincron->mutex_counter);
    //pthread_mutex_unlock(sincron->mutex_counter);

    //Se a tabela estiver sendo consultada, insere fica na fila de espera até terminar o processo de consulta
    if(sincron->counter != 0){
        //pthread_cond_wait(sincron->cond_counter,sincron->mutex_counter);
        //pthread_cond_wait(&(sincron->cond_counter[indice]), &(sincron->alteracao[indice]));
        pthread_cond_wait(sincron->cond_counter, &(sincron->alteracao[indice]));
    }

    //Salva os dados na tabela hash
    ht->armazenamento[pos].conteudo = conteudo;
    ht->armazenamento[pos].chave = pos;

    //printf("Thread Bloqueada\n");
    //pthread_mutex_unlock(sincron->mutex_counter);
    pthread_mutex_unlock(&(sincron->alteracao[indice]));

    return 1;
}

int ht_remove_elemento(hash_t * ht, entrada_hash_t * entrada){
    unsigned long int pos, indice;
    unsigned long int *da = NULL;
    int return_ind = 1;
    pos = ht_hash(ht, entrada->conteudo);

    //-----------------------------------
    indice = (int)floor(pos*0.1);                //Calcula o indice de qual mutex será utilizado
    sincronismo_t *sincron = (sincronismo_t*)ht->sincronizacao; //Usado pq consigo atribuir argumentos a um void, más n consigo acessar direto

    pthread_mutex_lock(&(sincron->alteracao[indice]));
    //pthread_mutex_lock(sincron->mutex_counter);


    //Se a tabela estiver sendo consultada, insere fica na fila de espera até terminar o processo de consulta
    if(sincron->counter != 0){
        pthread_cond_wait(sincron->cond_counter,sincron->mutex_counter);
        //pthread_cond_wait(&(sincron->cond_counter[indice]), &(sincron->alteracao[indice]));
    }

    if(entrada->conteudo){
        *entrada->conteudo = &da;
        return_ind = 0;
    }

    //pthread_mutex_unlock(sincron->mutex_counter);
    pthread_mutex_unlock(&(sincron->alteracao[indice]));
    return return_ind;
}

elemento_lista_t * ht_lista(hash_t * ht){
    elemento_lista_t *lista = NULL;
    elemento_lista_t *primeiro_lista = NULL;
    unsigned long int pos = 0;

    if(ht){ // if ht diferente de NULL
        lista = calloc(1, sizeof(elemento_lista_t));    // inicializa lista
        primeiro_lista = lista;
        lista->elemento = NULL;
        lista->proximo = NULL;
        while(pos < ht->numero_elementos)   // Enquanto não percorrer toda a hash table
        {
            if(&ht->armazenamento[pos] != NULL)     // Se armazenamento da pos não é NULL
            {
                if(lista->elemento != NULL){    // Qualquer elemento que não seja o primeiro precisa passar aqui para alocar memoria para o proximo e trocar ponteiros.
                    lista->proximo = calloc(1, sizeof(elemento_lista_t));
                    lista = lista->proximo;
                }
                lista->elemento = calloc(1, sizeof(entrada_hash_t)); // codigo genérico para o resto
                lista->elemento->conteudo = ht->armazenamento[pos].conteudo;
                lista->elemento->chave = ht->armazenamento[pos].chave;
                lista->elemento->proximo = NULL;
                lista->proximo = NULL;

            }
            pos++;
        }
    }

    return primeiro_lista;  // retorna ponteiro para lista ou NULL dependendo se ht exitir.

}
