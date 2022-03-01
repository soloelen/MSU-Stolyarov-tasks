#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


struct node{
	char *word_pointer;
	struct node *next;
};


char * extend_buff(char *buff_pointer, int buff_size_new){
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

char * get_str(int* flag_pointer, int *flag_amp){
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
			flag_quote = 1 - flag_quote; /*0 if flag_quote == 1 else 1*/
			chr_code = getchar(); /*read next char,return to while*/
			continue;
		}
		if ((chr_code == '&') && (flag_quote == 1)) {
			printf("Wrong & position\n");
			*flag_amp = 1;
			return NULL;
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
	if ((flag_quote == 1) && (chr_code == '&')) {
		printf("Wrong & position\n");
		*flag_amp = 1;
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
	int i = 0;
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
		curr_node = head;
		printf("head\n");
		while (curr_node->next != NULL) {
			printf("we are in while\n");
			while (curr_node->word_pointer[i] != '\0') {
				printf("while != 0\n curr_node->word_pointer[i] = %c\n", curr_node->word_pointer[i]);
				if ((curr_node->word_pointer[i] == '&') && (curr_node->next != NULL)) {
					printf("Wrong position of &");
					//delete_list(head, 1);
					exit(1);	
				}
				i++;
			}
			curr_node = curr_node->next;
			printf("Pereshli na sled elem\n");
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


char **make_arr(struct node *pointer_to_l) 
{
	char ** arr = NULL;
	int currlen = 0, k = 0;
	struct node * first_pointer = pointer_to_l;
	if (pointer_to_l == NULL) {
		return NULL;
	}
	while (first_pointer != NULL) {
		k += 1;
		first_pointer = first_pointer->next;
	}
	/*printf("k = %d\n", k);*/
	arr = (char **)malloc((k + 1) * sizeof(char *));
	/*pointer_to_l = first_pointer;*/
	while (pointer_to_l != NULL) {
        /*printf("real str = %s\n", pointer_to_l->word_pointer);*/
		arr[currlen] = pointer_to_l->word_pointer;
		currlen++;
        /*printf("currlen = %d\n", currlen);*/
        /*printf("str = %s\n", arr[currlen]);*/
		pointer_to_l = pointer_to_l->next;
	}
	arr[currlen] = NULL;
	return arr;
}



int main()
{
	int flag = 0, p, len = 0, flag_amp = 0;
	char *word = NULL;
	struct node *head = NULL;
	char **argv;
	while (flag != -1){
		printf(">");
		while ((flag != 1) && (flag !=  -1)){
			word = get_str(&flag, &flag_amp);
			if (word != NULL){
				head = push_to_list(head, word);
				len++;
				/*printf("len = %d\n", len);*/
			}
		}
		if (flag == -1){
			continue;
		}
		if ((flag == 1) && (len == 0)){
			flag = 0;
			head = NULL;
			continue;
		}
		if (flag_amp == 1) {
			flag_amp = 0;
			continue;
		}
		argv = make_arr(head);
		if ((head->word_pointer[0] == 'c') && (head->word_pointer[1] == 'd')){
			/*printf("WE ARE IN CD\n");*/
			if (len != 2){
				perror("Wrong amount of parameters\n");
			} else {
				if (head->next->word_pointer[0] == '~') {
					chdir(getenv("HOME"));
				} else {
					chdir(head->next->word_pointer);
				}
			}
			delete_list(head, 1);
			free(argv);
		} else {
			p = fork();
			if(p == 0){
				/*delete_list(head, 0);*/
				execvp(argv[0], argv);
				perror(argv[0]);
				exit(1);
			}
			wait(NULL);
			delete_list(head, 1);
			free(argv);
		}
		len = 0;
		if (flag == 1){
			flag = 0;
			head = NULL;
		}
	}
	return 0;
}
