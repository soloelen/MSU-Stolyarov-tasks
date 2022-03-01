#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


struct node
{
	char *word_pointer;
	struct node *next;
};


char * extend_buff(char *buff_pointer, int buff_size_new)
{
	char *buff_pointer_new = NULL;
	int count = 0, i;
	if (buff_pointer == NULL){
		buff_pointer = malloc(buff_size_new);
		for (count = 0; count < buff_size_new; count++){
			buff_pointer[count] = '\0';
		}
	} else {
		buff_pointer_new = malloc(buff_size_new);
		for (i = 0; i < buff_size_new - 10; i++)
			buff_pointer_new[i] = buff_pointer[i];
		free(buff_pointer);
		buff_pointer = buff_pointer_new;
	}
	return buff_pointer;
}


char * get_str(int* flag_pointer)
{
	int chr_code;
	int curr_len = 0;
	int buff_size = 10;
	int flag_quote = 0;
	char *buff_pointer = NULL;
	chr_code = getchar();
	while (((flag_quote != 0) || (chr_code != ' ')) &&
		(chr_code != EOF) && (chr_code != '\n'))
	{
		if (chr_code == '\''){
			flag_quote = 1 - flag_quote; /*0 if flag_quote=1 else 1*/
			chr_code = getchar();  /*read next char,back to while*/
			continue;
		}
		if (curr_len == 0){
			buff_pointer = extend_buff(buff_pointer, buff_size);
		} else if (curr_len == buff_size){
			buff_size += 10;
			buff_pointer = extend_buff(buff_pointer, buff_size);
		}
		buff_pointer[curr_len] = chr_code;
		curr_len++;
		chr_code = getchar();
	}
	if ((flag_quote == 1) && (chr_code == '\n')){
		printf("Quotes unbalanced\n");
		*flag_pointer = 1;
		return NULL;
	}
	if (chr_code == EOF)
		*flag_pointer = -1;
	if (chr_code == '\n')
		*flag_pointer = 1;
	return buff_pointer;
}


struct node *push_to_list(struct node *head, char *curr_word)
{
	/* Params: struct node *head, char *curr_word;
	 * Function adds curr_word to list, returns pointer to head
	 * */
	struct node *new_node;
	struct node *curr_node;
	if (head == NULL){
		new_node = (struct node *) malloc(sizeof(struct node));
		new_node->word_pointer = curr_word;
		new_node->next = NULL;
		head = new_node;
	}
	else{
		new_node = (struct node *) malloc(sizeof(struct node));
		new_node->next = NULL;
		new_node->word_pointer = curr_word;
		curr_node = head;
		while (curr_node->next != NULL){
			curr_node = curr_node->next;
		}
		curr_node->next = new_node;
	}
	return head;
}


void print_list(struct node *head)
{
	/* Params: struct node *head;
	 * Function prints list if it isn't empty
	 * Otherwise prints that the list is empty 
	 * */
	struct node *node;
	node = head;
	printf("\nPrint list:\n");
	if (node == NULL){
		printf("List is empty\n");
		return;
	}
	while (node != NULL){
		printf("%s\n", node->word_pointer);
		node = node->next;
	}
}


void delete_list(struct node *head, int flag_delete_word)
{
	/* Params: struct node *head
	 * Function delete list if it isn't empty
	 * */
	struct node *curr_node = head;
	if (curr_node != NULL){
		while (curr_node->next != NULL){
			curr_node = curr_node->next;
			if (flag_delete_word != 0) {
				free(head->word_pointer);
			}
			free(head);
			head = curr_node;
		}
		free(head->word_pointer);
		free(head);
	}
}


int main()
{
	int flag = 0;
	char *word = NULL;
	struct node *head = NULL;
	while (flag != -1){
		while ((flag != 1) && (flag !=  -1)){
			word = get_str(&flag);
			if (word != NULL){
				head = push_to_list(head, word);
			}
		}
		print_list(head);
		delete_list(head, 1);
		if (flag == 1){
			flag = 0;
			head = NULL;
		}
	}
	return 0;
}
