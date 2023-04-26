#include "hash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

hash_t * ht_init(unsigned long tam){
	int i = 0;
	hash_t *ht = NULL;

	if(tam > 0){
		ht = calloc(1,sizeof(hash_t));

		//ht->armazenamento = calloc(tam, sizeof(* entrada_hash_t)); //aloca n elementos do tipo
		ht->armazenamento = malloc(sizeof(entrada_hash_t)*tam);
		ht->numero_elementos = tam;
		
		while(i < tam) // Inicializando todas entradas hash
		{
			ht->armazenamento[i].chave = 0;
			ht->armazenamento[i].conteudo = NULL;
			if(i == tam - 1)
			{
				ht->armazenamento[i].proximo = NULL;
			}
			else
			{
				ht->armazenamento[i].proximo = &ht->armazenamento[i+1];
			}	
			i++;
		}
	}
	
	return ht;	
}

void ht_remove(hash_t * ht){
	//free(ht);
	//printf("\nMemória liberada!!!\n");
	// O que eu acho que seria correto
	
	int pos = 0;
	while(pos < ht->numero_elementos){
		free(&ht->armazenamento[pos]);
		pos++;
	}
	free(ht);
	
}

chave_t ht_hash(hash_t *ht, conteudo_t * conteudo){ // ------  precisa ser revista -------	

	int i = 0; // inutilizado
	unsigned long int valor = *conteudo; // ALTERADO -- VALOR ENTRAVA SEMPRE EM 0 NA FUNCAO E SEMPRE(EXCLUINDO ZERO) DAVA O MESMO RESULTADO	// Indica a posĩção a ser salva
	/*
	while(valor < *conteudo * 1000) { // dava problema com o codigo do professor
		valor = valor << 8;
		valor++;
	}
	*/
//	printf("%ld------------\n", ht->numero_elementos);
	valor = valor % ht->numero_elementos;
	//printf("%lu \n", valor);
	return valor;
}

entrada_hash_t * ht_busca_chave(hash_t * ht, chave_t chave){

	if(ht->armazenamento[chave].chave){						// Possui a chave e portanto o conteúdo	
		//printf("\n");
		//printf("%lu\n%lu\n", *ht->armazenamento[chave].conteudo, chave);

		//printf("\nBusca de Chave\n");
		//printf("Chave: %lu    Conteúdo: %lu\n", ht->armazenamento[chave].chave, *ht->armazenamento[chave].conteudo);
		
		return &ht->armazenamento[chave];
	}
	else{
		//printf("\nBusca de Chave\n");
		//printf("Nenhuma CHAVE encontrada por: %lu\n", ht->armazenamento[chave].chave);
		
		return NULL;
	}
}

entrada_hash_t * ht_busca_conteudo(hash_t * ht, conteudo_t * conteudo){
	unsigned long int pos = ht_hash(ht, conteudo);
	//printf("Conteúdo: %lu\n", *ht->armazenamento[pos].conteudo);
	if(&ht->armazenamento[pos] != NULL){
		//return &ht->armazenamento[pos];
		
		if(*ht->armazenamento[pos].conteudo == *conteudo){
			return &ht->armazenamento[pos];
		}
		
		return NULL;
	}
	else{
		return NULL;
	}
	//printf("\nBusca de Conteúdo\n");
	//printf("Chave: %lu    Conteúdo: %lu\n", ht->armazenamento[pos].chave, *ht->armazenamento[pos].conteudo);


/*------------------------------Usar em caso de tratar colisões---------------------------------------
	pos = ht_hash(ht, conteudo);
	entrada_hash_t *next = NULL;

	next = ht->armazenamento[pos].proximo;

	while(next != NULL){
				printf("%lu\n", *next->conteudo);
				printf("%lu\n", next->chave);
				next = next->proximo;
			}
-------------------------------------------------------------------------------------------------------------*/
}

int ht_insere_conteudo(hash_t * ht, conteudo_t * conteudo){
	unsigned long int pos;
	pos = ht_hash(ht, conteudo); //Verificar
	//printf("Chave: %lu    Conteúdo: %lu\n", pos, *conteudo);
	

//------------------------------------------------------------------------------------------------------------
	
	ht->armazenamento[pos].conteudo = conteudo;
	ht->armazenamento[pos].chave = pos;

	if(ht->armazenamento[pos].conteudo == conteudo){	// Verifica se conseguiu inserir o dado 
		return 1;										// Retornando o TRUE ou FALSE
	}
	else{
		return 0;
	}
	

/*------------------------------Usar em caso de precisar tratar colisões---------------------------------------
	entrada_hash_t *proximo = NULL;
	entrada_hash_t *last = NULL;
	entrada_hash_t *next = NULL;

	if(ht->armazenamento[pos].conteudo == NULL){   	// Coloca o CONTEÚDO na TABELA em referência a posição 
		ht->armazenamento[pos].conteudo = conteudo;
		ht->armazenamento[pos].chave = pos;
		return 1;
	}
	else{											// Tratamento de colisão, encadiando na posição da tabela

		last = ht->armazenamento[pos].proximo;

		if(last == NULL){							// Insere o dado na primeira posição do escadeamento
			last = malloc(sizeof(entrada_hash_t));
			last->conteudo = conteudo;
			last->chave = pos;
			ht->armazenamento[pos].proximo = last;
			return 1;
		}
		else{										// Tratamento de colisão, vai encadeando os dados
			while(last != NULL){					
				next = last;
				last = last->proximo;
			}
			last = malloc(sizeof(entrada_hash_t));
			last->conteudo = conteudo;
			last->chave = pos;
			next->proximo = last;					// Salva struct com dados no (ponteiro || endereço) ???
			return 1;
		}
	}
	------------------------------------------------------------------------------------------------------------*/
	//return 0;
}

int ht_remove_elemento(hash_t * ht, entrada_hash_t * entrada){
	unsigned long int *da = NULL;
	//entrada_hash_t * elem = malloc(sizeof(entrada_hash_t));
	if(entrada->conteudo){
		//ht->armazenamento(pos) = NULL;
		*entrada->conteudo = &da;
		return 0;
	}
	else{
		return 1;	
	}
	
}

elemento_lista_t * ht_lista(hash_t * ht){
	elemento_lista_t *lista = NULL;
	elemento_lista_t *primeiro_lista = NULL;
	unsigned long int pos = 0; 
	
	if(ht){ // if ht diferente de NULL
		lista = calloc(1, sizeof(elemento_lista_t)); 	// inicializa lista
		primeiro_lista = lista;
		lista->elemento = NULL;
		lista->proximo = NULL;
		while(pos < ht->numero_elementos)	// Enquanto não percorrer toda a hash table
		{
			if(&ht->armazenamento[pos] != NULL)		// Se armazenamento da pos não é NULL
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
	
	return primeiro_lista;	// retorna ponteiro para lista ou NULL dependendo se ht exitir.

}