#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "BinarySearchTree.h"
#include "LinkedList.h"

/*------------------------------STRUCTURES-----------------------------------*/

// Déclaration de la structure d'un noeud de l'arbre
typedef struct node_t{
	const void* key;
	const void* value;
	struct node_t* parent;
	struct node_t* left;
    struct node_t* right;
}Node;


// Déclaration de la strcuture de l'arbre binaire de recherche
struct tree_t{
	Node* root;
	size_t size;
	int (*comparison_fn_t)(const void *, const void *);
};



/*-----------------------------PROTOTYPES-------------------------------------*/

/*----------------------------InOrderTreeWalk----------------------------------------*
*
* Parcoure l'arbre dans le sens infixe et insère dans une liste liée les éléments
* compris entre deux clés d'un arbre binaire.
*
* ARGUMENTS :
*	node 				Un pointeur valide vers un noeud d'un arbre binaire
*	ll 					Un pointeur valide vers une liste chainée
*	keyMin 				La clé minimum de l'intervalle recherché
*	keyMax 				La clé maximum de l'intervalle recherché
*	comparison_fn_t		Un pointeur valide vers la focntion de comparaison.
*-----------------------------------------------------------------------------------*/
static bool InOrderTreeWalk (Node* node, LinkedList* ll, void* keyMin, void* keyMax, 
					 		  int (*comparison_fn_t)(const void*, const void*));


/*----------------------------------freeNode------------------------------------*
 *
 * Libère la mémoire allouée à un noeud de l'arbre.
 *
 * ARGUMENT:
 *	node				Un pointeur valide vers un noeud.
 *	freecontent         Si oui ou non on doit libérer le contenu aussi.
 * 
 *-----------------------------------------------------------------------------*/
static void freeNode(Node* node, bool freeContent);




/*------------------------------IMPLEMENTATION----------------------------------*/

BinarySearchTree* newBST(int comparison_fn_t(const void *, const void *))
{
	BinarySearchTree* bst = malloc(sizeof(BinarySearchTree));
	if(!bst)
		return NULL;
	bst->root = NULL;
	bst->size = 0;
	bst->comparison_fn_t = comparison_fn_t;
	return bst;
}



void freeBST(BinarySearchTree* bst, bool freeContent)
{	
	freeNode(bst->root, freeContent);
	free(bst);
	return;
}


static void freeNode(Node* node, bool freeContent)
{
	// Cas de base
	if(!node)
		return;
	// Efface le noeud de gauche
	freeNode(node->left, freeContent);
	// Efface le noeud de droite
	freeNode(node->right, freeContent);
	// Efface les données en fonction de la valeur de freeContent
	if (freeContent == true)
	{
		free((void*)node->value);
		free((void*)node->key);
	}
	free(node);
}



size_t sizeOfBST(const BinarySearchTree* bst)
{
	return bst->size;
}



bool insertInBST(BinarySearchTree* bst, const void* key, const void* value)
{
	// Création du nouvel élément
	Node* newNode = malloc(sizeof(Node));
	if(!newNode)
		return false;

	// Recherche du parent
	Node* parent = NULL;
	Node* current = bst->root;
	while(current)
	{
		parent = current;
		if(bst->comparison_fn_t(key,current->key) < 0)
			current = current->left;
		else
			current = current->right;
	}
	newNode->parent = parent;
	if(parent == NULL)
		bst->root = newNode;
	else if(bst->comparison_fn_t(key,parent->key) < 0)
		parent->left = newNode;
	else
		parent->right = newNode;
	// Remplissage du nouvel élément
	newNode->key = key;
	newNode->value = value;
	newNode->left = NULL;
	newNode->right = NULL;
	// Augmentation de la taille de l'arbre d'une unité
	bst->size++;

	return true;
}



const void* searchBST(BinarySearchTree* bst, const void* key)
{
	Node* current = bst->root;
	// Boucle de recherche
	while(current)
	{
		if(bst->comparison_fn_t(current->key, key) == 0)
			return current->value;
		
		if(bst->comparison_fn_t(key,current->key) < 0)
			current = current->left;
		else
			current = current->right;
		
	}
	// Si on sort de la boucle, clé non trouvée
	return NULL;
}



LinkedList* getInRange(const BinarySearchTree* bst, void* keyMin, void* keyMax)
{	
	//Creation d'une liste chainée
	LinkedList* sortedList = newLinkedList();
	if(sortedList == NULL)
		return NULL;

	// Parcours infixe de l'arbre et insertion des éléments compris entre les deux clés
	if(!InOrderTreeWalk(bst->root, sortedList, keyMin, keyMax, bst->comparison_fn_t)){
		freeLinkedList(sortedList, false);
		return NULL;
	}

	return sortedList;
}


static bool InOrderTreeWalk (Node* node, LinkedList* ll, void* keyMin, void* keyMax, 
					 		  int (*comparison_fn_t)(const void*, const void*))
{
	if(node == NULL)
		return true;

	if(comparison_fn_t(node->key, keyMin)>0){
		if(!InOrderTreeWalk(node->left, ll, keyMin, keyMax, comparison_fn_t))
			return false;
	}

	if(comparison_fn_t(node->key,keyMin)>=0 && comparison_fn_t(node->key,keyMax)<=0){
		if(!insertInLinkedList(ll, node->value))
			return false;
	}

	if(comparison_fn_t(node->key, keyMax) <= 0){
		if(!InOrderTreeWalk(node->right, ll, keyMin, keyMax, comparison_fn_t))
			return false;
	}
	return true;
}