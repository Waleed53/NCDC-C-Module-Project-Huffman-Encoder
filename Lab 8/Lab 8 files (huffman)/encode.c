#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 256
#define MAX_LEN     100

struct tnode
{
    struct  tnode* left; /*used when in tree*/
    struct  tnode*right; /*used when in tree*/  
    struct  tnode*parent;/*used when in tree*/
    struct  tnode* next; /*used when in list*/ 
    float   freq;
    int     isleaf;
    unsigned char     symbol;
};


/*global variables*/
char code[MAX_SYMBOLS][MAX_LEN];
struct tnode* root=NULL; /*tree of symbols*/
struct tnode* qhead=NULL; /*list of current symbols*/
struct cnode* chead=NULL;/*list of code*/

/*
    @function   talloc
    @desc       allocates new node 
*/
struct tnode* talloc(int symbol,float freq)
{
    struct tnode* p=(struct tnode*)malloc(sizeof(struct tnode));
    if(p!=NULL)
    {
        p->left=p->right=p->parent=p->next=NULL;
        p->symbol=symbol;
        p->freq=freq;
		p->isleaf=1;
    }
    return p;
}

/*
    @function display_tnode_list
    @desc     displays the list of tnodes during code construction
*/
void pq_display(struct tnode* head)
{
    struct tnode* p=NULL;
    printf("list:");
    for(p=head;p!=NULL;p=p->next)
    {
        printf("(%c,%f) ",p->symbol,p->freq);
    }
    printf("\n");
}

/*
    @function pq_insert
    @desc     inserts an element into the priority queue
    NOTE:     makes use of global variable qhead
*/
void pq_insert(struct tnode* p)
{
    struct tnode* curr=qhead;
    struct tnode* prev=NULL;
   printf("inserting:%c,%f\n",p->symbol,p->freq);
   if(qhead==NULL) /*qhead is null*/
   {
   		/*TODO: write code to insert when queue is empty*/
   		qhead = p;
        	return;
   }
   /*TODO: write code to find correct position to insert*/
   while (curr != NULL && curr->freq < p->freq) {
        prev = curr;
        curr = curr->next;
    }

   if(curr==qhead)
   {
   	 	/*TODO: write code to insert before the current start*/
   	 	p->next = qhead;
        	qhead = p;
   }
   else /*insert between prev and next*/
   {
	 	/*TODO: write code to insert in between*/
	 	prev->next = p;
        	p->next = curr;
   }
}

/*
    @function pq_pop
    @desc     removes the first element
    NOTE:     makes use of global variable qhead
*/

void freetree(struct tnode* root) {
    if (!root) return;
    freetree(root->left);
    freetree(root->right);
    free(root);
}

struct tnode* pq_pop()
{
    
    /*TODO: write code to remove front of the queue*/
    if (qhead == NULL)
        return NULL;
    struct tnode* p = qhead;
    qhead = qhead->next;
    p->next = NULL;
    printf("popped:%c,%f\n",p->symbol,p->freq);
    return p;
}

/*
	@function build_code
	@desc     generates the string codes given the tree
	NOTE: makes use of the global variable root
*/
void generate_code(struct tnode* node,int depth)
{
    if (node->isleaf) {
        int symbol = node->symbol;
        code[symbol][depth] = '\0';
        struct tnode* curr = node;
        for (int i = depth - 1; i >= 0; i--) {
            code[symbol][i] = (curr->parent->right == curr) ? '1' : '0';
            curr = curr->parent;
        }

        if (symbol == '\n')
            printf("built code: \\n %s\n", code[symbol]);
        else if (symbol == '\t')
            printf("built code: \\t %s\n", code[symbol]);
        else if (symbol == ' ')
            printf("built code: [space] %s\n", code[symbol]);
        else
            printf("built code: %c %s\n", symbol, code[symbol]);
        return;
    }

    if (node->left)
        generate_code(node->left, depth + 1);
    if (node->right)
        generate_code(node->right, depth + 1);
}

/*
	@func	dump_code
	@desc	output code file
*/
void dump_code(FILE* fp)
{
	int i=0;
	for(i=0;i<MAX_SYMBOLS;i++)
	{
		if(code[i][0]) /*non empty*/
			fprintf(fp,"%d %s\n",i,code[i]);
	}
}

/*
	@func	encode
	@desc	outputs compressed stream
*/
void encode(char* str,FILE* fout)
{
	while(*str)
	{
		fprintf(fout,"%s",code[*str]);
		str++;
	}
}
/*
    @function main
*/
int main()
{
    /*test pq*/
    struct tnode* p=NULL;
    struct tnode* lc,*rc;
    int freq_table[MAX_SYMBOLS] = {0};
    int total_chars = 0;
    int ch;
	const char *CODE_FILE="code.txt";
	const char *OUT_FILE="encoded.txt";
	const char *BOOK_FILE="book_ascii.txt";
	
	FILE* fout=NULL;
	FILE *fbook=NULL;
	/*zero out code*/
	memset(code,0,sizeof(code));

	fbook = fopen(BOOK_FILE, "r"); // open the book
	while ((ch = fgetc(fbook)) != EOF) { // count characters
        freq_table[ch]++;
        total_chars++;
    	}
	fclose(fbook); // close the book

	qhead=NULL;
    /*initialize with freq*/
    for (int i = 0; i < MAX_SYMBOLS; i++) {
        if (freq_table[i] > 0) {
            pq_insert(talloc(i, (float)freq_table[i] / total_chars));
        }
    }
    /*build tree*/
    while ((lc = pq_pop()) && (rc = pq_pop())) {
        p = talloc(0, lc->freq + rc->freq);
        p->isleaf = 0;
        p->left = lc;
        p->right = rc;
        lc->parent = rc->parent = p;
        pq_insert(p);
    }
    /*get root*/
    root = p;
	/*build code*/
	memset(code, 0, sizeof(code));
	generate_code(root,0);
	/*output code*/
	puts("code:");
	fout=fopen(CODE_FILE,"w");
	dump_code(stdout);
	dump_code(fout);
	fclose(fout);

	fbook = fopen(BOOK_FILE, "r");
        fout = fopen(OUT_FILE, "w");
        while ((ch = fgetc(fbook)) != EOF) {
           fprintf(fout, "%s", code[ch]);
        }
        fclose(fbook);
        fclose(fout);

        fout = fopen(OUT_FILE, "r");
    	fseek(fout, 0, SEEK_END);
    	long encoded_bits = ftell(fout);
    	fclose(fout);

    	printf("Original bits: %ld\n", total_chars * 8L);
    	printf("Compressed bits: %ld\n", encoded_bits);
    	printf("Compression ratio: %.2f%%\n", (100.0 * encoded_bits) / 							(total_chars * 8L));

	/*TODO: clear resources*/
	freetree(root);
    return 0;
}
