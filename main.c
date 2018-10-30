#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node node;
typedef struct queue queue;
typedef struct auxNode auxNode;
typedef struct binaryTree binaryTree;

struct auxNode{
	int canibais;
	int missionarios;	
};

struct node{
	auxNode *mLeft, *mRight;
	node *next;
};

struct queue{
	node *first;
	node *last;
	int size;
};

struct binaryTree{
	auxNode *mLeft, *mRight;
	binaryTree *right;
	binaryTree *left;	
};

//-----FUNÇÕES DÉ NÓ(FILA)-----
node* createNode(int mLeft, int cLeft, int mRight, int cRight);
//-----FUNÇÕES DE FILA-----
queue *create_queue();
bool isEmpty(queue *q);
void enqueue(queue *q, int mLeft, int cLeft, int mRight, int cRight);
int search(queue *q, int mLeft, int cLeft, int mRight, int cRight);
void print_queue(queue *q);
//-----FUNÇÕES DE NÓ(ÁRVORE BINÁRIA)-----
auxNode* createAuxNode(int c, int m);
//-----FUNÇÕES DE ÁRVORE BINÁRIA-----
binaryTree* creatEmptyBtree();
binaryTree* createBtree(int mLeft, int cLeft, int mRight, int cRight, binaryTree *right, binaryTree *left);
int check(int mLeft, int cLeft, int mRight, int cRight);
void mapTree(int mLeft, int cLeft, int mRight, int cRight, binaryTree *tree, queue *q);
void findPath(binaryTree *tree, int flag, queue *q);

//-----MAIN-----
int main(){

	binaryTree *tree = creatEmptyBtree();
	queue *q = create_queue();
	queue *path = create_queue();
	int flag = 0;
	printf("-----CAMINHO-----\n");
	mapTree(3,3,0,0,tree,q);
	findPath(tree, flag, path);
	//print_queue(path);
	
	return 0;
}

//-----FUNÇÕES DÉ NÓ(FILA)-----
node* createNode(int mLeft, int cLeft, int mRight, int cRight){
	node *newNode = (node*)malloc(sizeof(node));

	newNode->mLeft = createAuxNode(cLeft, mLeft);
	newNode->mRight = createAuxNode(cRight, mRight);
	newNode->next = NULL;

	return newNode;
}

//-----FUNÇÕES DE FILA-----

queue *create_queue(){
	queue *q = (queue*)malloc(sizeof(queue));
	q->first = NULL;
	q->last = NULL;
	q->size = 0;

	return q;
}

bool isEmpty(queue *q){
	//printf("Testando fila\n");
	if(q->first == NULL){
		//printf("Vazia\n");
		return true;
	}else{
		//printf("Nao vazia\n");
		return false;
	}
}

void enqueue(queue *q, int mLeft, int cLeft, int mRight, int cRight){
	node *new_node = createNode(mLeft, cLeft, mRight, cRight);
	if(isEmpty(q)){
		q->last = new_node;
		q->first = q->last;
	}else{
		q->last->next = new_node;
		q->last = new_node;
	}
	q->size++;
}

int search(queue *q, int mLeft, int cLeft, int mRight, int cRight){
	//printf("Buscando...\n");
	if(isEmpty(q)){
		//printf("Fila vazia!\n");
		return -1;
	}else{
		node *aux = q->first;
		while(aux!=NULL){
			if(aux->mLeft->missionarios == mLeft && aux->mLeft->canibais == cLeft && aux->mRight->missionarios == mRight && aux->mRight->canibais == cRight){
				//printf("Item encontrado!\n");
				return 0;
			}
			aux = aux->next;
		}
		//printf("Item não encontrado\n");
		return -1;
	}
}

void print_queue(queue *q){
	if(isEmpty(q)){
		//printf("Fila Vazia!!\n");
	}else{
		queue *aux = (queue*)malloc(sizeof(queue));
		aux->first = q->first;
		printf("((M,C),(M,C))\n");
		while(aux->first!=NULL){
			printf("(%d,%d),(%d,%d)\n", aux->first->mLeft->missionarios, aux->first->mLeft->canibais, aux->first->mRight->missionarios, aux->first->mRight->canibais);
			aux->first = aux->first->next;
		}
	}
}

//-----FUNÇÕES DE NÓ(ÁRVORE BINÁRIA)-----

auxNode* createAuxNode(int c, int m){
	auxNode *new_node = (auxNode*)malloc(sizeof(auxNode));

	new_node->canibais = c;
	new_node->missionarios = m;

	return new_node;
}

//-----FUNÇÕES DE ÁRVORE BINÁRIA-----

binaryTree* creatEmptyBtree(){
	return NULL;
}

binaryTree* createBtree(int mLeft, int cLeft, int mRight, int cRight, binaryTree *right, binaryTree *left){
	binaryTree *new_tree = (binaryTree*) malloc(sizeof(binaryTree));
	
	new_tree->mLeft = createAuxNode(cLeft, mLeft);

	new_tree->mRight = createAuxNode(cRight, mRight);

	new_tree->right = right;
	new_tree->left = left;

	return new_tree;
}

int check(int mLeft, int cLeft, int mRight, int cRight){
	if((mLeft<cLeft && mLeft!=0)||(mRight<cRight && mRight!=0)){
		return -1;
	}
	if(mLeft>3 || cLeft>3 || mRight>3 || cRight>3){
		return -1;
	}
	if(((mLeft+mRight)!=3)||((cLeft+cRight)!=3)){
		return -1;
	}
	if(mLeft<0 || mRight<0 || cLeft<0 || cRight<0){
		return -1;
	}
	return 0;
}

void mapTree(int mLeft, int cLeft, int mRight, int cRight, binaryTree *tree, queue *q){
	//printf("Checando...\n");
	if(check(mLeft, cLeft, mRight, cRight)==0){
		//printf("Checagem feita\n");
		if(search(q, mLeft,cLeft, mRight, cRight)!=-1){//Procurar se o no já existe na fila
			//printf("Buscando na fila\n");
		}else{
			//printf("Prosseguindo\n");
			//Se não, add na árvore e cria
			tree = createBtree(mLeft,cLeft, mRight, cRight, NULL, NULL);
			enqueue(q, mLeft, cLeft, mRight, cRight);

			//Calcular os próximos caminhos
				//Da esquerda para a direita
					//Dois missionarios
				mapTree(mLeft-2, cLeft, mRight+2, cRight, tree->right, q);
					//Dois canibais
				mapTree(mLeft, cLeft-2, mRight, cRight+2, tree->right, q);
					//Um de cada
				mapTree(mLeft-1, cLeft-1, mRight+1, cRight+1, tree->right, q);

				//Da direita para a esquerda
					//Um missionario
				mapTree(mLeft+1, cLeft, mRight-1, cRight, tree->left, q);
					//Um canibal
				mapTree(mLeft, cLeft+1, mRight, cRight-1, tree->left, q);
					//Dois missionarios
				mapTree(mLeft+2, cLeft, mRight-2, cRight, tree->left, q);
					//Dois canibais
				mapTree(mLeft, cLeft+2, mRight, cRight-2, tree->left, q);
					//Um de cada
				mapTree(mLeft+1, cLeft+1, mRight-1, cRight-1, tree->right, q);
			}//Fim else próximos caminhos
		}//Fim if check		
	}//Fim função

void findPath(binaryTree *tree, int flag, queue *q){
	//printf("FindPath\n");
	if(tree->mLeft->missionarios == 0 && tree->mLeft->canibais == 0 && tree->mRight->missionarios == 3 && tree->mRight->canibais == 3){
		//enqueue(queue *q, int mLeft, int cLeft, int mRight, int cRight)
		printf("PrevPath\n");
		enqueue(q, tree->mLeft->missionarios, tree->mLeft->canibais, tree->mRight->missionarios, tree->mRight->canibais);
		flag = 1;
		printf("Flag ativada\n");
	}else{
		printf("Path\n");
		findPath(tree->left, flag, q);
		printf("Path esquerdo\n");
		if(flag == 1){
			enqueue(q, tree->left->mLeft->missionarios, tree->left->mLeft->canibais, tree->left->mRight->missionarios, tree->left->mRight->canibais);
			printf("Path esquerdo -qFlag\n");
			return;
		}
		findPath(tree->right, flag, q);
		printf("Path direito\n");
		if(flag == 1){
			enqueue(q, tree->right->mLeft->missionarios, tree->right->mLeft->canibais, tree->right->mRight->missionarios, tree->right->mRight->canibais);
			printf("Path direito -qFlag\n");
			return;
		}
	}
}
