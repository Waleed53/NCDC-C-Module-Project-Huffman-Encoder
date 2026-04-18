#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 256
#define MAX_LEN     100

struct tnode
{
    struct  tnode* left; /*used when in tree*/
    struct  tnode*right; /*used when in tree*/  
    int     isleaf;
    unsigned char     symbol;
};

struct code
{
	int		symbol;
	char	strcode[MAX_LEN];
};

/*global variables*/
struct tnode* root=NULL; /*tree of symbols*/

/*
    @function   talloc
    @desc       allocates new node 
*/
struct tnode* talloc()
{
    struct tnode* p=(struct tnode*)malloc(sizeof(struct tnode));
    if(p!=NULL)
    {
        p->left=p->right=NULL;
        p->symbol=0;
		p->isleaf=0;
    }
    return p;
}

/*
    @function build_tree
    @desc     builds the symbol tree given the list of symbols and code.h
	NOTE: alters the global variable root that has already been allocated in main
*/
void build_tree(FILE* fp)
{
    int symbol_int;
    char strcode[MAX_LEN];

    while (fscanf(fp, "%d %s", &symbol_int, strcode) == 2)
    {
        unsigned char symbol = (unsigned char)symbol_int;
        struct tnode* curr = root;

        for (int i = 0; strcode[i]; i++) {
            if (strcode[i] == '0') {
                if (!curr->left) curr->left = talloc();
                curr = curr->left;
            } else if (strcode[i] == '1') {
                if (!curr->right) curr->right = talloc();
                curr = curr->right;
            }
        }

        curr->isleaf = 1;
        curr->symbol = symbol;
    }
}

/*
	function decode
*/
void decode(FILE* fin,FILE* fout)
{
	char c;
	struct tnode* curr=root;
	while((c=getc(fin))!=EOF)
	{
		/*TODO:
			traverse the tree
			print the symbols only if you encounter a leaf node
		*/
		if (c == '0') {
		    if (!curr->left) {
		        fprintf(stderr, "Invalid bit sequence (left)\n");
		        return;
		    }
		    curr = curr->left;
		} else if (c == '1') {
		    if (!curr->right) {
		        fprintf(stderr, "Invalid bit sequence (right)\n");
		        return;
		    }
           	 curr = curr->right;
       		 } else {
            continue; // Skip whitespace or stray characters
        }

		if (curr->isleaf) {
		    fputc(curr->symbol, fout);
		    curr = root;
		}
	}
}


/*
	@function freetree
	@desc	  cleans up resources for tree
*/

void freetree(struct tnode* root)
{
	if(root==NULL)
		return;
	freetree(root->right);
	freetree(root->left);
	free(root);
}
int main()
{
	const char* IN_FILE="encoded.txt";
	const char* CODE_FILE="code.txt";
	const char* OUT_FILE="decoded.txt";
	FILE* fout;
	FILE* fin;
	/*allocate root*/
	root=talloc();
	fin=fopen(CODE_FILE,"r");
	/*build tree*/
	build_tree(fin);
	fclose(fin);

	/*decode*/
	fin=fopen(IN_FILE,"r");
	fout=fopen(OUT_FILE,"w");
	decode(fin,fout);
	fclose(fin);
	fclose(fout);
	/*cleanup*/
	freetree(root);
	getchar();
	return 0;
}

