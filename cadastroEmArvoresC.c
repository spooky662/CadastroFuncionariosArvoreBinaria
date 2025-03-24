#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//informacoes sobre o funcionario para o cadastro
struct funcionario {
	int matricula;
	char nome[100];
	char cargo[50];
	float salario;
	struct funcionario *esq;
	struct funcionario *dir;
};

typedef struct funcionario Funcionario;

//funcao para criar um funcionario e criar arvore para primeiro funcionario
Funcionario* create(int matricula, const char *nome, const char *cargo, float salario){
	Funcionario *f = malloc(sizeof(Funcionario));
	f->matricula = matricula;
	strcpy(f->nome, nome);
	strcpy(f->cargo, cargo);
	f->salario = salario;
	f->esq = NULL;
	f->dir = NULL;
	return f; //primeiro ponteiro e a raiz da arvore
}

//funcao para adicionar um funcionario a arvore
void add(Funcionario *f, int matricula, const char *nome, const char *cargo, float salario){
	if(matricula < f->matricula){ //teste para saber em qual lado adicionar o funcionario com base no numero da matricula
		if(f->esq == NULL)
			f->esq = create(matricula, nome, cargo, salario);
		else
			add(f->esq, matricula, nome, cargo, salario); // teste para saber se nodo solicitado esta vazio e deve seguir para o proximo ou se adiciona funcionario
	}else{
		if(f->dir == NULL)
			f->dir = create(matricula, nome, cargo, salario);
		else
			add(f->dir, matricula, nome, cargo, salario); // teste para saber se nodo solicitado esta vazio e deve seguir para o proximo ou se adiciona funcionario
	}
}

//funcao para buscar um funcionario, necessaria para a funcao att de atualizacao funcionar
Funcionario* buscar(Funcionario *f, int matricula){
	if (f == NULL || f->matricula == matricula){
		return f;
	}
		
	if (matricula < f->matricula){
		return buscar(f->esq, matricula);
	}
	
	return buscar(f->dir, matricula);
}

//funcao para atualizar um funcionario
void att(Funcionario *f, int matricula, const char *nome, const char *cargo, float salario){
	Funcionario *func = buscar(f, matricula); //usa a funcao buscar para encontrar o funcionario e usa um ponteiro auxiliar para nao perder a referencia do original
	if (func != NULL){ //atualiza os usando o ponteiro auxiliar
		strcpy(func->nome, nome);
		strcpy(func->cargo, cargo);
		func->salario = salario;
		printf("Dados atualizados com sucesso!");
	} else {
		printf("Funcionario nao encontrado");	
	}
}

void imprimir(Funcionario *f){ //imprime na pre ordem (primeiro da esquerda ao ultimo da direita)
	if(f == NULL){
		return;
	}
	imprimir(f->esq);
	printf("\nMatricula: %d\nNome: %s\nCargo: %s\nSalario: %.2f\n", f->matricula, f->nome, f->cargo, f->salario);
	imprimir(f->dir);
}

//funcao para remover um funcionario da arvore
void rem(Funcionario *f, int matricula)
{
    Funcionario *filho = f;
    Funcionario *pai = NULL;
    while (filho != NULL && filho->matricula != matricula)
    {
        pai = filho;
        if (matricula < filho->matricula)
            filho = filho->esq;
        else
            filho = filho->dir;
    }

    if (filho != NULL)
    {
        if (filho->esq == NULL && filho->dir == NULL)
        {
            printf("\nFuncionario %d foi removido.\n", matricula); //nodo é folha e é removido sem complicações
            if (pai->esq == filho) pai->esq = NULL;
            if (pai->dir == filho) pai->dir = NULL;
        }
        else if (filho->esq != NULL && filho->dir == NULL)
        {
            printf("\nFuncionario %d foi removido.\n", matricula); //nodo é pai e precisa alocar o filho da esquerda em seu lugar
            if (pai->esq == filho) pai->esq = filho->esq;
            if (pai->dir == filho) pai->dir = filho->esq;
        }
        else if (filho->esq == NULL && filho->dir != NULL)
        {
            printf("\nFuncionario %d foi removido.\n", matricula); //nodo é pai e precisa alocar o filho da direita em seu lugar
            if (pai->esq == filho) pai->esq = filho->dir;
            if (pai->dir == filho) pai->dir = filho->dir;
        }
        else
        {
            printf("\nFuncionario %d foi removido.\n", matricula); //nodo é pai de dois filhos e precisa ser feita realocacao dos nodos
            Funcionario *p = filho->esq; //nodo sera substituido pelo filho da esquerda, busca pela subarvore da esquerda
            Funcionario *aux = p; //nodo auxiliar para buscar o maior nodo da esquerda
            while (p->dir != NULL) //busca pelo maior nodo na subarvore esquerda, que nao pode ter filhos
            {
                aux = p;
                p = p->dir;
            }
            aux->dir = NULL; //remove p da arvore e substitui pelo nodo que vai ser excluido
            filho->matricula = p->matricula;
            strcpy(filho->nome, p->nome);
            strcpy(filho->cargo, p->cargo);
            filho->salario = p->salario;
        }
    }
    else
    {
        printf("\nFuncionario com matricula %d nao foi encontrado.\n", matricula);
    }
}

int main(void) {
	
    //criando arvore
	Funcionario *root = create(1001, "Joao", "Gerente", 5000.00);
	
    //adicionando funcionarios
	add(root, 1002, "Maria", "Caixa", 1500.00);
	add(root, 1003, "Sergio", "Repositor", 1700.00);
	add(root, 1004, "Egeu", "Vendedor", 2500.00);
	
    //listando funcionarios
	printf("Lista de funcionarios:\n");
	imprimir(root);

    //buscando funcionario matricula 1002
    int matriculabusca = 1002;
    Funcionario *func = buscar(root, matriculabusca);
    if (func != NULL){
        printf("\nFuncionario encontrado: %s, Cargo: %s", func->nome, func->cargo);
    } else {
        printf("\nFuncionario com matricula %d nao encontrado.\n", matriculabusca);
    }

    //atualizando funcionario e exibindo atualizacao
    att(root, 1003, "Carlos", "Supervisor", 4000.00);

    printf("\nLista de Funcionarios apos atualizacao:\n");
    imprimir(root);

    //removendo um funcionario
    rem(root, 1002);

    //exibindo novamente os funcionarios
    printf("\nLista de funcionarios após remocao:\n");
    imprimir(root);

    return 0;
}