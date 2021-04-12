#define MAXN 1000
#define MAXM 10000
#define HASH 20003
#define MAXW 10
typedef long long ll;

struct Mail {
	int num_word;
	ll name[MAXW];
} mail[MAXM];
int mail_cnt;

struct User {
	int num, start, end;
	int mid[MAXM];  // mail id
} user[MAXN];

struct Node {
	int mid;
	Node* next;
} node_pool[MAXW * MAXM], table[HASH];  // 단어별로 생성
ll word[HASH];  // 단어 이름 목록
int node_cnt;
#define node() &node_pool[node_cnt++]

int K;
void init(int N, int _K)
{
	mail_cnt = node_cnt = 0;
	K = _K;
	for (register int i = 0; i < N; i++) user[i].num = user[i].start = user[i].end = 0;
	for (register int i = 0; i < HASH; i++) table[i].next = 0;
}

ll name[MAXW];
int str2ll(char* subject) {
	register ll index = 0;
	for (register int i = 0, num = 0;; i++) {
		if (subject[i] < 'a') name[num++] = index, index = 0;  // ' ' or '\0'
		else index = (index << 5) + subject[i] - 'a' + 1;
		if (!subject[i]) return num;
	}
}

void sendMail(char subject[], int uID, int cnt, int rIDs[])
{
	int num = str2ll(subject);
	mail[mail_cnt].num_word = num;
	for (register int i = 0; i < num; i++) mail[mail_cnt].name[i] = name[i];
	register int id;
	for (register int i = 0; i < cnt; i++) {
		id = rIDs[i];
		user[id].mid[user[id].end++] = mail_cnt;
		if (user[id].num == K) {
			register int &j = ++user[id].start;
			while (user[id].mid[j] < 0) j++;  // 삭제한 mail은 skip
		}
		else user[id].num++;
	}
	for (register int i = 0; i < num; i++) {
		for (register int j = name[i] % HASH;; j = (j + 1) % HASH) {
			if (table[j].next && word[j] != name[i]) continue;
			if (!table[j].next) word[j] = name[i];  // 처음
			register Node* node = node();
			node->mid = mail_cnt;
			node->next = table[j].next;
			table[j].next = node;
			break;
		}
	}
	mail_cnt++;
}

int getCount(int uID)
{
	return user[uID].num;
}

int deleteMail(int id, char subject[])
{
	int num = str2ll(subject);
	register int ans = 0, mid;
	for (register int i = user[id].start; i < user[id].end; i++) {
		mid = user[id].mid[i];
		if (mid == -1 || num != mail[mid].num_word) continue;  // 삭제했거나 단어 개수 다르면 skip
		for (register int j = 0;; j++) {
			if (j == num) {  // 일치
				user[id].mid[i] = -1;
				ans++;
				break;
			}
			if (mail[mid].name[j] != name[j]) break;
		}
	}
	user[id].num -= ans;
	return ans;
}

bool check[MAXM];  // mid별 text 포함 여부 (중복 가능)
int searchMail(int id, char text[])
{
	str2ll(text);
	register int ans = 0, mid, key;
	for (key = name[0] % HASH;; key = (key + 1) % HASH) {
		if (table[key].next && word[key] != name[0]) continue;
		for (register Node* node = table[key].next; node; node = node->next) check[node->mid] = true;
		break;
	}
	for (register int i = user[id].start; i < user[id].end; i++) {
		mid = user[id].mid[i];
		if (mid == -1) continue;  // 삭제했으면 skip
		if (check[mid]) ans++;
	}
	for (register Node* node = table[key].next; node; node = node->next) check[node->mid] = false;  // 원상복귀
	return ans;
}
