/*------------------------------------------*/
/* A database system */
/* Programmer : Cha Jeong Won */
/* Date : 1994.1.7 */
/* File name : pdb.c */
/*------------------------------------------*/
#include <stdio.h>
#include <ctype.h>
#include <dos.h>
#include <conio.h>
#include <string.h>
#include <stdio.h>
#include <bios.h>
#define MAX_FIELDS 10
#define NAME_SIZE 40
#define MAX_LINES 24
#define LINE_LEN 79
#define FNAME_SIZE 8
enum field_type { STRING, DOUBLE, INT, CHAR };
struct desriptor {
	int position; /*position of field in record(레코드에서 필드의 위치 */
	enum field_type type; /* type of field 필드의 타입(string, double, int ,char) */
	char name[NAME_SIZE]; /* name of field 필드의 이름 */
	int size; /* size of field in bytes 필드의 크기(Bytes) */
	int x, y; /* location on screen 화면 위의 좌표*/
} fields[MAX_FIELDS];
/*----------------------------------------------------*/
int num_fields; /* number of fields in a record 한 레코드의 필드 수  */
int record_size; /* record size in bytes 레코드의 크기(Bytes) */
int sort_field; /* number of field to sort on 정렬할 필드의 수 */
int num_recs; /* number of records in file 필드의 레코드 수*/
/*----------------------------------------------------*/
/* This sturcture manages the database information 이 구조는 DB 정보를 관리한다*/
/*----------------------------------------------------*/
struct db_type {
	char* data; /* pointer to data 데이터의 주소*/
	struct db_type* prior; /* pointer to previous record 이전 레코드에 대한 주소 */
	struct db_type* next; /* pointer to next record 다음 레코드에 대한 주소*/
} *first, * last, * cur_rec;
/*----------------------------------------------------*/
void Db(void);
void DbGets(char* str, int maxlen);
/*void gotoxy(int x, int y);*/
void ClrEol(int x, int y);
/*void clrscr(void);*/
void Define(void);
void DisplayRec(char* p);
void Init(void);
void DisplayFields(void);
void DlsStore(struct db_type* i,
	struct db_type** first,
	struct db_type** last);
void Delete(struct db_type** start,
	struct db_type** last);
void Prompt(char* s);
void Print(void);
void Change(char* data, int field);
int Save(char* fname);
int Load(char* fname);
int Menu(void);
int Enter(void);
int SelectField(char* str);
struct db_type* Find(char* key,
	struct db_type* from, int field);
struct db_type* Search(void);
struct db_type* Modify(void);
struct db_type* Browse(void);
/**********************************************************/
/* */
/* */
/**********************************************************/
main()
{
	Db();
}
/**********************************************************/
/* */
/* */
/**********************************************************/
void
Db(void)
{
	int choice;
	char fname[80];
	clrscr();
	Init(); /* initialize 초기화*/
	do { /*------------------------------*/
		choice = Menu();
		switch (choice) {
		case 1: /* define structure of database DB구조 정의*/
			Define();
			break;
		case 2: /* enter info 정보 입력*/
			Enter();
			break;
		case 3: /* browse in the file 파일에서 찾아보기 */
			cur_rec = Browse();
			break;
		case 4: /* search 검색*/
			cur_rec = Search();
			break;
		case 5: /* modify 수정*/
			cur_rec = Modify();
			break;
		case 6: /* delete 삭제 */
			Delete(&first, &last);
			break;
		case 7: /* print 출력*/
			Print();
			break;
		case 8: /* save data 데이터 저장*/
			Prompt("Enter file to save (ENTER to skip): ");
			DbGets(fname, FNAME_SIZE);
			if (*fname)
				Save(fname);
			break;
		case 9: /* load data 데이터 읽기*/
			Prompt("Enter file to load (ENTER to skip): ");
			DbGets(fname, FNAME_SIZE);
			if (*fname)
				Load(fname);
			break;
		} /* ---- end switch() ------*/
	} while (choice != 0); /*-------------*/
}
/**********************************************************/
/* */
/* initialize 초기화 부분*/
/**********************************************************/
void
Init(void)
{
	num_fields = 0;
	num_recs = 0;
	sort_field = 0;
	first = cur_rec = last = NULL;
	/* 필드수, 레코드수, 정렬할 필드수, 링크드리스트 초기화*/
}
/**********************************************************/
/* */
/* Prompt the user for selection 사용자의 선택을 위한 메세지 표시*/
/* */
/**********************************************************/
int
Menu(void)
{
	char ch;
	clrscr();
	gotoxy(0, 0);
	printf(" 0. quit \n");
	printf(" 1. Define Database\n");
	printf(" 2. Enter Information\n");
	printf(" 3. Browse\n");
	printf(" 4. Search\n");
	printf(" 5. Modify\n");
	printf(" 6. Delete\n");
	printf(" 7. Print\n");
	printf(" 8. Save Database\n");
	printf(" 9. load Database\n");
	printf("(%d records in file)", num_recs);
	do {
		gotoxy(0, 12);
		printf(" Enter number of your choice: ");
		ch = getch();
	} while (ch < '0' || ch > '9');
	return(ch - '0');
}
/**********************************************************/
/* */
/* define the structure of the database DB구조 정의*/
/* */
/**********************************************************/
void
Define(void)
{
	char ch;
	int i;
	struct db_type* temp;
	clrscr();
	/* free any previously allocated memory 이전에 할당된 메모리 헤제*/
	while (first) {
		temp = first->next;
		free(temp->data); /* free the data */
		free(temp); /* free the control structure */
		first = temp;
	} /* --- end while -----*/
	Init();
	record_size = 0;
	do {
		gotoxy(0, 0);
		printf("number of fields(1-%d) (0 to skip): ", MAX_FIELDS);
		scanf("%d%*c", &num_fields);
		if (!num_fields) return;
	} while (num_fields<0 || num_fields>MAX_FIELDS);
	for (i = 0; i < num_fields; i++) {
		clrscr();
		gotoxy(0, 0);
		fields[i].position = record_size;
		printf("Enter name of field %d (1 - 40 chars): ", i + 1);
		DbGets(fields[i].name, 40);
		/* the filed type is not used by any code in this
		 example, but it may be needed by your application
		 이 예제에서는 파일 형식이 사용되지않는다 하지만 응용프로그램에는 필요할 수도 있다.*/
		printf("\nfield types are: (S)tring, (D)ouble, (I)nt");
		printf(" and (C)har \n");
		do {
			printf("Enter field type (use first letter): ");
			ch = getche();
			ch = tolower(ch);
		} while (!strchr("sdic", ch));
		switch (ch) {
		case 's': fields[i].type = STRING;
			break;
		case 'd': fields[i].type = DOUBLE;
			break;
		case 'i': fields[i].type = INT;
			break;
		case 'c': fields[i].type = CHAR;
			break;
		} /*------ end switch() -----*/
		printf("\n");
		do {
			printf("enter length of field: ");
			scanf("%d%*c", &fields[i].size);
		} while (fields[i].size<0 || fields[i].size>LINE_LEN);
		fields[i].size++; /* make room for null terminator null terminator를 위한 공간 확보*/
		do {
			printf("enter X,Y coordinates of field [EX: 10 10]: ");
			scanf("%d%d%*c", &fields[i].x, &fields[i].y);
		} while (fields[i].x < 0 ||
			fields[i].x + fields[i].size >= LINE_LEN ||
			fields[i].y < 0 ||
			fields[i].y >= MAX_LINES);
		printf(" Is this field defined OK? (Y/N): ");
		ch = getch();
		if (tolower(ch) != 'y') i--; /* reenter 재입력 */
		else
			record_size += fields[i].size;
	} /*------- end for() -------*/
	printf("\n Press a key to see fields and screen layout");
	getch();
	clrscr();
	DisplayFields();
	sort_field = SelectField("Select field to sort on");
}
/***********************************************************/
/* */
/* Enter information into the database DB에 정보입력*/
/* */
/***********************************************************/
Enter(void)
{
	register int i, field;
	char ch;
	struct db_type* p;
	for (;;) {
		/* allocate memory for control structure 제어구조를 위한 메모리 할당*/
		p = (struct db_type*)malloc(sizeof(struct db_type));
		if (!p) {
			printf(" out of memory!\n");
			return 0;
		}
		/* allocate memory for actual data 실제 데이터를 위한 메모리 할당*/
		p->data = (char*)malloc(record_size);
		if (!p->data) {
			printf(" out of memory!\n");
			return 0;
		}
		clrscr();
		DisplayFields();
		/*input the data 데이터 입력*/
		for (i = 0; i < num_fields; i++) {
			gotoxy(fields[i].x + strlen(fields[i].name) + 1, fields[i].y);
			DbGets(&(p->data)[fields[i].position], fields[i].size - 1);
			if (!(*p->data + fields[0].position))
				return 1;
		} /*------ end for ------*/
		num_recs++;
		Prompt("Entry OK? (Y/N): ");
		ch = getch();
		ch = tolower(ch);
		while (ch != 'y') {
			field = SelectField("Select field to modify");
			Change(p->data, field); /* change field 필드 변경*/
			Prompt("Entry OK? (Y/N): ");
			ch = getch();
			ch = tolower(ch);
		} /*---- end while ------*/
		DlsStore(p, &first, &last); /* store it 저장*/
	} /*--------- end for ----------*/
}
/****************************************************/
/* */
/* Browse through the list and return pointer to */
/* the last record examined */
/* 목록을 탐색하고 검사한 마지막 레코드에 대한 포인터(주소값)를 반환*/
/****************************************************/
struct db_type
	* Browse(void)
{
	union key {
		int i;
		char ch[1];
	}k;
	struct db_type* info;
	clrscr();
	ClrEol(0, MAX_LINES);
	gotoxy(1, MAX_LINES);
	printf("Down Arrow: forward Up Arrow: backward");
	printf(" Home: beginning End: end ");
	/* If a current location has been defined,
	 start at that point; otherwise, start at
	 the top of the file.
	 현재 위치가 정의 된 경우 그 지점에서 시작, 그렇지 않으면 파일의 상단에서 시작*/
	if (cur_rec)
		info = cur_rec;
	else
		info = first;
	while (info) {
		DisplayRec(info->data); /* display record 디스플레이 레코드*/
		k.i = bioskey(0);
		if (!k.ch[0]) {
			switch (k.ch[1]) {
			case 71: /* home */
				info = first;
				break;
			case 72: /* up */
				if (info->prior)
					info = info->prior; /* get previous rec 이전 기록 받기*/
				break;
			case 79: /* end */
				info = last;
				break;
			case 80: /* down */
				if (info->next)
					info = info->next; /* get next rec 다음 기록 받기*/
				break;
			} /* ---- end switch -----*/
		}
		else {
			return info; /* return current location 현재 위치 반환*/
		}
	} /*------- end while ---------*/
	return NULL;
}
/**********************************************************/
/* Modify a record in the list and return a pointer to */
/* that record 목록에서 레코드를 수정하고 해당 레코드에 대한 포인터 반환*/
/**********************************************************/
struct db_type
	* Modify(void)
{
	struct db_type* info;
	char key[80], ch;
	register int field;
	clrscr();
	/* find field to use for searching 검색에 사용할 필드 찾기*/
	DisplayFields();
	field = SelectField("Select field");
	/* find record to search for 검색할 레코드 찾기*/
	Prompt("Enter key: ");
	DbGets(key, fields[field].size);
	/* find first match, if any 첫번째 일치 항목 찾기*/
	info = Find(key, first, field);
	while (info) {
		DisplayRec(info->data);
		Prompt("Modify this record? (Y/N): ");
		ch = getch();
		ch = tolower(ch);
		if (ch != 'y') /* find next match, if any 다음 일치 항목 찾기*/
			info = Find(key, info->next, field);
		else
			break;
	} /*---- end while(info) ----*/
	if (!info)
		return NULL; /* not found 찾기 실패*/
		/* which field to modify? 수정할 필드는?*/
	field = SelectField("Select field to modify");
	Change(info->data, field); /* change data */
	return info;
}
/**********************************************************/
/* */
/* Remone an element from file list 파일 목록에서 요소 제거*/
/* */
/**********************************************************/
void
Delete(struct db_type** start, struct db_type** last)
{
	struct db_type* info;
	char key[80], ch;
	int field;
	clrscr();
	DisplayFields();
	field = SelectField("Select field");
	Prompt("Enter key: ");
	DbGets(key, fields[field].size);
	info = Find(key, first, field);
	while (info) {
		DisplayRec(info->data);
		Prompt("Delete this record? (Y/N): ");
		ch = getch();
		ch = tolower(ch);
		if (ch != 'y')
			info = Find(key, info->next, field);
		else
			break;
	} /*-- end while(info) -- */
	if (!info)
		return; /*not found */
		/* if deleting current rec, set cur_rec to NULL 현재 레코드를 삭제하는 경우 cur_rec을 NULL로*/
	if (cur_rec == info)
		cur_rec = NULL;
	num_recs--;
	if (*start == info) { /* deleting first record 첫번째 레코드 삭제*/
		*start = info->next;
		if (*start) (*start)->prior = NULL;
		else *last = NULL;
	}
	else {
		info->prior->next = info->next;
		if (info != *last) /* deleting from middle 중간에서 삭제*/
			info->next->prior = info->prior;
		else
			*last = info->prior; /* deleting last record 마지막 레코드 삭제*/
	}
	free(info->data); /* free data 데이터 해제*/
	free(info); /* free control structure 통제 구조 해제*/
}
/*********************************************************/
/* Print the file either selective or in full 파일 선택적 또는 전체 출력*/
/* */
/* Note : */
/* this fuction works only DOS, so you may need */
/* to change it for other environment.이 기능은 DOS에서만 작동함 */
/*********************************************************/
void
Print(void)
{
	register int i, j, field;
	int blank_lines;
	struct db_type* info;
	char* p, selective, key[80], s[80], ch;
	clrscr();
	Prompt("Selective search? (Y/N): ");
	ch = getch();
	if (tolower(ch) == 'y') {
		DisplayFields();
		field = SelectField("Select field");
		Prompt("Enter key: ");
		DbGets(key, fields[field].size);
		selective = 1;
	}
	else
		selective = 0;
	/* Number of blank lines between records 레코드 사이의 빈 줄수*/
	Prompt("Enter number of blank lines: ");
	DbGets(s, 20);
	blank_lines = atoi(s);
	Prompt(" Printing....");
	if (selective)
		info = Find(key, first, field);
	else
		info = first;
	while (info) {
		/* Display each record as it prints 출력시 각 레코드에 출력*/
		DisplayRec(info->data);
		for (i = 0; i < num_fields; i++) {
			p = info->data + fields[i].position;
			/* print the field 필드를 출력*/
			while (*p) {
				biosprint(0, *p, 0);
				p++;
			} /* -- end while() -- */
			/* output a CR-LF CR-LF출력*/
			biosprint(0, '\r', 0);
			biosprint(0, '\n', 0);
		} /* -- end for(i) -- */
		/* output blank lines between records 레코드 사이 빈줄 출력*/
		for (j = 0; j < blank_lines; j++) {
			biosprint(0, '\r', 0);
			biosprint(0, '\n', 0);
		} /* -- end for(j) -- */
/* get next record 다음 레코드 가져오기*/
		if (selective)
			info = Find(key, info->next, field);
		else
			info = info->next;
	} /* -- end while(info) -- */
	biosprint(0, '\f', 0); /* form feed feed공급*/
}
/****************************************************/
/* */
/* Save both data and description to disk 데이터와 그에대한 설명을 디스크에 저장*/
/* */
/****************************************************/
Save(char* fname)
{
	struct db_type* info;
	register int i;
	FILE* fp;
	char name[80];
	strcpy(name, fname);
	strcat(name, ".dat");
	fp = fopen(name, "wb");
	if (!fp) {
		printf("cannot open data file\n");
		return 0;
	}
	Prompt("Saving data...");
	info = first;
	while (info) { /* save each record 각 레코드 저장*/
		fwrite(info->data, record_size, 1, fp);
		info = info->next; /* get next db_type 다음 db_type을 가져옴*/
		if (ferror(fp)) {
			printf("error writing file\n ");
			fclose(fp);
			return 0;
		}
	} /* -- end while(info) -- */
	fclose(fp);
	strcpy(name, fname);
	strcat(name, ".def");
	fp = fopen(name, "wb");
	if (!fp) {
		printf(" cannot open definition file \n");
		fclose(fp);
		return 0;
	}
	putw(record_size, fp);
	putw(num_fields, fp);
	putw(sort_field, fp);
	for (i = 0; i < num_fields; i++) {
		fwrite(&fields[i], sizeof(fields[i]), 1, fp);
		if (ferror(fp)) {
			printf("error writing file\n ");
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	return 1;
}
/**********************************************************/
/* */
/* Load the data and definition files. 데이터 로드*/
/* */
/**********************************************************/
Load(char* fname)
{
	struct db_type* info;
	FILE* fp, * fp2;
	register int i;
	char name[80];
	strcpy(name, fname);
	strcat(name, ".dat");
	fp = fopen(name, "wb");
	if (!fp) {
		printf(" Cannot open file\n");
		return 0;
	}
	strcpy(name, fname);
	strcat(name, ".def");
	fp2 = fopen(name, "wb");
	if (!fp2) {
		printf(" Cannot open file\n");
		return 0;
	}
	/* free any preniously allocated memory 미리 할당된 메모리 해제*/
	while (first) {
		info = first->next;
		free(info->data); /* free the data 데이터 해제*/
		free(info); /* free the control structure 통제 구조 해제*/
		first = info;
	}
	/* reset top and bottom pointers 상단과 하단 포인터 재설정*/
	first = last = cur_rec = NULL;
	num_recs = 0;
	Prompt("loading file...");
	/* load DEF file first 먼저 DEF파일 로드*/
	record_size = getw(fp2);
	num_fields = getw(fp2);
	sort_field = getw(fp2);
	for (i = 0; i < num_fields; i++) {
		if (1 != fread(&fields[i], sizeof(fields[i]), 1, fp2)) {
			printf("Error reading definition file \n");
			fclose(fp);
			fclose(fp2);
			getch();
			return 0;
		}
	}
	fclose(fp2);
	while (!feof(fp)) {
		/* allocate memory for control structure 통제 구조를 위한 메모리 할당*/
		info = (struct db_type*)malloc(sizeof(struct db_type));
		if (!info) {
			printf(" out of memory ");
			fclose(fp);
			return 0;
		}
		/* allocate memory for actual data 실제 데이터를 위한 메모리 할당*/
		info->data = (char*)malloc(record_size);
		if (!info->data) {
			printf(" out of memory ");
			fclose(fp);
			return 0;
		}
		/* read records 레코드 읽기*/
		if (1 != fread(info->data, record_size, 1, fp)) {
			if (ferror(fp)) {
				printf("error reading data file \n");
				fclose(fp);
				return 0;
			}
			fclose(fp);
			cur_rec = first;
			return 1;
		}
		DlsStore(info, &first, &last);
		num_recs++;
	} /* end while(!feof(fp)) */
	fclose(fp);
	cur_rec = first;
	return 0;
}
/***********************************************************/
/* */
/* display the database fields. db필드 디스플레이*/
/* */
/***********************************************************/
void
DisplayFields(void)
{
	register int i, j;
	for (i = 0; i < num_fields; i++) {
		gotoxy(fields[i].x, fields[i].y);
		printf("%s ", fields[i].name);
		for (j = 0; j < fields[i].size - 1; j++)
			putch('.');
	} /* end for */
}
/***********************************************************/
/* */
/* Display one record's information */
/* */
/***********************************************************/
void
DisplayRec(char* p)
{
	register int i;
	DisplayFields();
	for (i = 0; i < num_fields; i++) {
		gotoxy(fields[i].x + strlen(fields[i].name) + 1, fields[i].y);
		printf("%s ", p + fields[i].position);
	}
}
/***********************************************************/
/* */
/* Return the number of the field the user selects */
/* using the arrow key */
/* */
/***********************************************************/
SelectField(char* str)
{
	union {
		int i;
		char ch[2];
	}k;
	register int i, len;
	Prompt(str); /* display prompting message */
	for (i = 0;;) {
		/* position cursor at start of each field */
		len = strlen(fields[i].name) + 1;
		gotoxy(fields[i].x + len, fields[i].y);
		k.i = bioskey(0);
		if (!k.ch[0])
			switch (k.ch[1]) {
			case 72: /* up */
				if (i) i--;
				break;
			case 80: /* down */
				if (i < num_fields - 1) i++;
				break;
			}
		else
			if (k.ch[0] == '\r') return i;
	}
}
/**********************************************************/
/* */
/* Create a doubly linked list in sorted oeder. */
/* */
/**********************************************************/
void
DlsStore(
	struct db_type* i, /* new element */
	struct db_type** first, /* first element in list */
	struct db_type** last /* last element in list */
)
{
	struct db_type* old, * p;
	if (*last == NULL) { /* first element in list */
		i->next = NULL;
		i->prior = NULL;
		*last = i;
		*first = i;
		return;
	}
	p = *first; /* first at top of list */
	old = NULL;
	while (p) { /* find location to insert record */
		if (strcmp(p->data + fields[sort_field].position,
			i->data + fields[sort_field].position) < 0) {
			old = p;
			p = p->next;
		}
		else {
			if (p->prior) { /* goes into middle of list */
				p->prior->next = i;
				i->next = p;
				i->prior = p->prior;
				p->prior = i;
				return;
			} /* end if(p->prior) */
			i->next = p; /* new first elemet */
			i->prior = NULL;
			p->prior = i;
			*first = i;
			return;
		} /* end else */
	}
	old->next = i; /* put on end */
	i->next = NULL;
	i->prior = old;
	*last = i;
}
/**********************************************************/
/* */
/* Change the specified record and field */
/* */
/**********************************************************/
void
Change(char* data, int field)
{
	int i;
	DisplayRec(data);
	Prompt("Enter new data ");
	/* Display record, position cursor at beginning of
	 the field, and input new data */
	gotoxy(fields[field].x, fields[field].y);
	printf("%s ", fields[field].name);
	for (i = 0; i < fields[field].size - 1; i++)
		putch('.');
	i = strlen(fields[field].name) + 1;
	gotoxy(fields[field].x + 1, fields[field].y);
	DbGets(data + fields[field].position, fields[field].size - 1);
}
/**********************************************************/
/* */
/* Find an entry and return a pointer to it. */
/* */
/**********************************************************/
struct db_type
	* Find(char* key,
		struct db_type* from,
		int field)
{
	struct db_type* info;
	info = from;
	while (info) {
		if (strcmp(key,
			info->data + fields[field].position))
			return info;
		info = info->next; /* get next db_type */
	}
	return NULL; /* not found */
}
/**********************************************************/
/* */
/* Look for a record in the list and return a pointer */
/* to the last match found. */
/* */
/**********************************************************/
struct db_type
	* Search(void)
{
	int field;
	char key[80], ch;
	struct db_type* info, * temp;
	clrscr();
	DisplayFields();
	field = SelectField("Select field to search on");
	Prompt("Enter key: "); /* search string */
	DbGets(key, fields[field].size);
	ch = '\0';
	/* find first match, if any */
	info = Find(key, first, field);
	while (info && ch != 'q') {
		/* save pointer in case no other matches are found */
		temp = info;
		DisplayRec(info->data);
		/* find next match, if any */
		info = Find(key, info->next, field);
		/* tell user i more matches exit */
		if (info) Prompt("\nMore, ");
		else printf("\n No more match, ");
		printf("Press a key ('q' to quit): ");
		ch = getch();
		ch = tolower(ch);
	}
	return temp; /* return pointer to last match */
}
/**********************************************************/
/* */
/* Display a message on the bottom line of the screen. */
/* */
/**********************************************************/
void
Prompt(char* s)
{
	ClrEol(0, MAX_LINES);
	gotoxy(1, MAX_LINES);
	printf(s);
}
/***********************************************************/
/* */
/* Read a string from the keyboard, but dio not scroll */
/* display when a RETURN is entered */
/* */
/***********************************************************/
void
DbGets(char* str, int maxlen)
{
	char* p;
	int i;
	p = str;
	for (i = 0;;) {
		*str = getch();
		if (*str == '\r') { /* return when RETURN entered */
			*str = '\0'; /* NULL terminate */
			return;
		}
		if (*str == '\b') { /* backspace */
			if (str > p) {
				str--;
				putch('\b');
				putch('.');
				putch('\b');
				i--;
			}
		}
		else if (i < maxlen) {
			putch(*str);
			str++;
			i++;
		}
	}
}
/**********************************************************/
/* */
/* Clear to end of spacified line */
/* */
/**********************************************************/
void
ClrEol(int x, int y)
{
	gotoxy(x + 1, y);
	for (; x < LINE_LEN; x++) {
		printf(" ");
	}
}