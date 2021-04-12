#define NAME_MAXLEN 6
#define PATH_MAXLEN 1999
#define MAX_C 30
typedef struct _list_head {
	struct _list_head *next;
} list_head;

typedef struct _directory {
	struct _directory* p;
	struct _directory* c[MAX_C];
	int c_cnt;
	long long id;
} directory;

#define MAX_D 100000

int cnt_d;
directory d[MAX_D];
directory root;

directory *get_directory() {
	directory *temp = &d[cnt_d];
	temp->id = 0;
	temp->c_cnt = 0;
	temp->p = 0;
	cnt_d++;
	return temp;
}

void init(int n) {
	cnt_d = 0;
	root.p = 0;
	root.c_cnt = 0;
}

long long to_digit(char **s) {
	int idx = 0;
	long long digit = 0;
	while (**s && **s != '/') {
		digit += **s << (idx * 8);
		++idx; ++(*s);
	}
	return digit;
}

directory *find(char *path) {
	++path;
	if (*path == 0)
		return &root;

	long long id = to_digit(&path);
	directory *temp = &root;
	while (id > 0) {
		for (int i = 0; i < temp->c_cnt; i++)
			if (temp->c[i]->id == id) {
				temp = temp->c[i];
				break;
			}
		path++;
		id = to_digit(&path);
	}
	return temp;
}

void cmd_mkdir(char path[PATH_MAXLEN + 1], char name[NAME_MAXLEN + 1]) {
	directory *pos = find(path);
	directory *item = get_directory();

	item->id = to_digit(&name);
	item->p = pos;
	pos->c[pos->c_cnt] = item;
	pos->c_cnt++;
}

void cmd_rm(char path[PATH_MAXLEN + 1]) {
	directory *pos = find(path);
	directory *p = pos->p;
	for (int i = 0; i < MAX_C; i++) {
		if (p->c[i]->id == pos->id) {
			p->c[i] = p->c[p->c_cnt - 1];
			--p->c_cnt;
			break;
		}
	}
}

void cp_directory(directory *dst, directory *src) {
	dst->id = src->id;
	dst->c_cnt = src->c_cnt;
	for (int i = 0; i < src->c_cnt; i++) {
		directory *temp = get_directory();
		dst->c[i] = temp;
		temp->p = dst;
		cp_directory(temp, src->c[i]);
	}
}

void cmd_cp(char srcPath[PATH_MAXLEN + 1], char dstPath[PATH_MAXLEN + 1]) {
	directory *src = find(srcPath);
	directory *dst = find(dstPath);

	directory *item = get_directory();
	cp_directory(item, src);

	dst->c[dst->c_cnt] = item;
	item->p = dst;
	++dst->c_cnt;
}

void cmd_mv(char srcPath[PATH_MAXLEN + 1], char dstPath[PATH_MAXLEN + 1]) {
	directory *src = find(srcPath);
	directory *dst = find(dstPath);

	directory *p = src->p;
	for (int i = 0; i < MAX_C; i++) {
		if (p->c[i]->id == src->id) {
			p->c[i] = p->c[p->c_cnt - 1];
			--p->c_cnt;
			break;
		}
	}
	dst->c[dst->c_cnt] = src;
	src->p = dst;
	++dst->c_cnt;
}

directory *q[MAX_D];
int cmd_find(char path[PATH_MAXLEN + 1]) {
	int answer = 0;
	directory *src = find(path);

	int front = 0, rear = 0;
	q[rear++] = src;
	while (front < rear) {
		directory *temp = q[front++];
		++answer;
		for (int i = 0; i < temp->c_cnt; ++i) {
			q[rear] = temp->c[i];
			++rear;
		}
	}
	return answer - 1;
}
