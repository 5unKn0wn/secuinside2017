#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

typedef struct bug {
	char bug_used;
	size_t bug_idx;
	char bug_name[32];
	size_t bug_size;
	size_t dummy;
	char* bug_content;
	struct bug* next;
} _bug;

_bug* bug_list;
int already_edited = 0;

void init(void);
void init_bug(void);
void menu(void);
size_t read_int(void);
void* my_malloc(size_t);
void add_bug(void);
void view_bug(void);
void edit_bug(void);
void delete_bug(void);
void random_malloc(void);
void handler(void);
int main(void) {
	size_t choice;
	
	init();
	
	init_bug();
	while (1) {
		menu();
		choice = read_int();
		switch (choice) {
			case 1:
				add_bug();
				break;
			case 2:
				view_bug();
				break;
			case 3:
				edit_bug();
				break;
			case 4:
				delete_bug();
				break;
			case 5:
				return 0;
			default:
				puts("that's nono~~");
		}
	}
}

void init(void) {
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	signal(SIGALRM, (void*)handler);
	alarm(60);
}

void init_bug(void) {
	bug_list = (_bug*)malloc(sizeof(_bug));
	bug_list->bug_used = 1;
	bug_list->bug_idx = 0;
	strcpy(bug_list->bug_name, "5unKn0wn's buffer overflow");
	bug_list->bug_size = 32;
	bug_list->dummy = 0;
	bug_list->bug_content = malloc(bug_list->bug_size);
	strcpy(bug_list->bug_content, "buffer overflow is not die...");
	bug_list->next = 0;
}

void menu(void) {
	puts("+-----------------------------+");
	puts("|                             |");
	puts("|     bug manage system       |");
	puts("|                             |");
	puts("|        1. add bug           |");
	puts("|        2. view bug          |");
	puts("|        3. edit bug          |");
	puts("|        4. delete bug        |");
	puts("|        5. exit              |");
	puts("|                             |");
	puts("+-----------------------------+");
	printf("choice : ");
}

size_t read_int(void) {
	char buf[16];
	read(0, buf, sizeof(buf));
	return (size_t)atoi(buf);
}

void* my_malloc(size_t size) {
	void* buffer;

	random_malloc();
	buffer = malloc(size);
	random_malloc();

	return buffer;
}

void add_bug(void) {
	_bug* i;

	_bug* bug = (_bug*)my_malloc(sizeof(_bug));
	for (i = bug_list; i->next != 0; i = i->next);
	bug->bug_used = 1;
	bug->bug_idx = i->bug_idx + 1;
	printf("what is your bug name? : ");
	read(0, bug->bug_name, sizeof(bug->bug_name));
	printf("what is your bug size? : ");
	bug->bug_size = read_int();
	bug->dummy = 0;
	bug->bug_content = my_malloc(bug->bug_size);
	printf("what is your bug content? : ");
	read(0, bug->bug_content, bug->bug_size);
	i->next = bug;
	bug->next = 0;
	puts("done!");
}

void view_bug(void) {
	_bug* i = bug_list;

	do {
		puts("==========================================");
		printf("bug used : %s\n", i->bug_used ? "using" : "non using");
		printf("bug idx : %d\n", (int)i->bug_idx);
		printf("bug name : %s\n", i->bug_name);
		printf("bug size : %d\n", (int)i->bug_size);
		printf("bug content : %s\n", i->bug_content);
		puts("==========================================");
		i = i->next;
	} while(i);
}

void edit_bug(void) {
	_bug* i = bug_list;
	size_t bug_idx, choice, where;

	if (already_edited) {
		printf("you already edited bug\n");
		return;
	}

	printf("what is your bug idx? : ");
	bug_idx = read_int();

	do {
		if (i->bug_idx == bug_idx)
			break;
		i = i->next;
	} while (i);

	if (!i) {
		puts("can't find bug!!");
		return;
	}

	if (!i->bug_used) {
		puts("this bug was deleted...");
		return;
	}

	printf("what do you want to edit?\n");
	printf("1. bug name 2. bug_size 3. bug content\n");
	printf("choice : ");
	choice = read_int();

	switch (choice) {
		case 1:
			printf("what is your new bug name? : ");
			memset(i->bug_name, 0, sizeof(i->bug_name));
			read(0, i->bug_name, sizeof(i->bug_name));
			break;

		case 2:
			printf("what is your new bug size? : ");
			i->bug_size = read_int();
			break;

		case 3:
			printf("what is your new bug content?\n");
			printf("you can change only 4 bytes\n");
			printf("where do you want to change? : ");
			where = read_int();
			printf("what do you want to change? : ");
			read(0, &i->bug_content[where], 4);
			break;

		default:
			puts("that's nono~~");
			break;
	}

	puts("done!");
	already_edited = 1;
}

void delete_bug(void) {
	_bug* i = bug_list;
	size_t bug_idx;

	printf("what is your bug idx? : ");
	bug_idx = read_int();

	do {
		if (i->bug_idx == bug_idx)
			break;
		i = i->next;
	} while (i);

	if (!i) {
		puts("can't find bug!!");
		return;
	}

	if (!i->bug_used) {
		puts("this bug was deleted...");
		return;
	}

	strcpy(i->bug_name, "bug was deleted...");
	i->bug_used = 0;
	i->bug_size = 0;
	free(i->bug_content);
	i->bug_content = 0;
}

void random_malloc(void) {
	char rand[2];
	int fd = open("/dev/urandom", O_RDONLY);
	read(fd, rand, sizeof(rand));
	close(fd);
	malloc((*(size_t*)rand));
}

void handler(void) {
	puts("time is up~");
	exit(0);
}
