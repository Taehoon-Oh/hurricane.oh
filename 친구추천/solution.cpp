#define NULL    0
#define MAXL    5
#define MAXF    10

struct Node {
	int id;
	Node* next;
}; //   ��ü �Լ� ȣ�� Ƚ���� 50,000 ����, ģ�� ������ �� �ִ� 500,000 �� => ��� 1,000,000 �� �ʿ�
Node node_pool[1000000];
int last_alloc = 0;
#define new_Node() (&node_pool[last_alloc++])

#define MAX_N 10000
#define TBL_SIZE 11
struct User {
	Node friend_list[TBL_SIZE];
};
User user[MAX_N + 1];

int gN;
void init(int N) { // N ������� ��, ����� ID <= N, N <= 10,000
	last_alloc = 0;
	gN = N;
	for (register int id = 1; id <= N; id++) // �ʱ�ȭ
		for (register int key = 0; key < TBL_SIZE; key++)
			user[id].friend_list[key].next = NULL;
}

void add(int id, int F, int ids[MAXF]) {  // 1 <= F <= 10
	register int fid;
	register int key;
	register Node* node;
	while (F--) {
		fid = ids[F]; // id �� ģ�� fid �߰�
		key = fid % TBL_SIZE;
		node = new_Node();
		node->id = fid;
		node->next = user[id].friend_list[key].next; // ����Ʈ�� �� �տ� �߰��Ѵ�.
		user[id].friend_list[key].next = node;

		key = id % TBL_SIZE; // fid �� ģ�� id �߰�
		node = new_Node();
		node->id = id;
		node->next = user[fid].friend_list[key].next;
		user[fid].friend_list[key].next = node;
	}
}

void del(int id1, int id2) { // ������ ģ�� ��Ͽ��� �����Ѵ�.
	register int key = id2 % TBL_SIZE; // id1 �� ģ����Ͽ��� id2 �� �����.
	register Node* node = &(user[id1].friend_list[key]);
	while (node->next != NULL) {
		if (node->next->id == id2) { // ��Ͽ��� id �� ã�� �����.
			node->next = node->next->next;
			break;
		}
		node = node->next;
	}

	key = id1 % TBL_SIZE; // id2 �� ģ����Ͽ��� id1 �� �����.
	node = &(user[id2].friend_list[key]);
	while (node->next != NULL) {
		if (node->next->id == id1) {
			node->next = node->next->next;
			break;
		}
		node = node->next;
	}
}

int num_mutual[MAX_N + 1]; // id �Բ� �ƴ� ģ���� ���� ī��Ʈ�ϱ� ���� �뵵
int recommend(int id, int list[MAXL]) {
	/*
		�Բ� �ƴ� ģ���� ���� ���� ���� ������ �ִ� 5 ���� ��õ�Ѵ�.
		�Բ� �ƴ� ģ���� ���� ���� ��� ����� ID �� ���� ���� ������ ����ڸ� �켱�Ѵ�.
		�ִ� ȣ�� Ƚ���� 40 ���� <= 50000
	*/
	for (register int i = 1; i <= gN; i++)
		num_mutual[i] = 0;

	register Node* node;
	register int key;
	for (register int k = 0; k < TBL_SIZE; k++) {
		Node* friend_node; // �� ģ������ ��ȸ�ϸ鼭
		for (friend_node = user[id].friend_list[k].next; friend_node != NULL; friend_node = friend_node->next) {
			int fid = friend_node->id;
			for (key = 0; key < TBL_SIZE; key++) { // ģ������ ģ������ �湮�ϸ鼭
				for (node = user[fid].friend_list[key].next; node != NULL; node = node->next)
					num_mutual[node->id]++; // ģ������ ģ���� ���� Ƚ���� ����.
			}
		}
	}

	num_mutual[id] = 0; // ��õ���� �ڽŰ� �ڽ��� ģ���� ���� �ؾ��Ѵ�.
	for (key = 0; key < TBL_SIZE; key++)
		for (node = user[id].friend_list[key].next; node != NULL; node = node->next)
			num_mutual[node->id] = 0;

	register int cnt = 0; // ��õ�� ģ���� �ִ� 5�� ã�ƺ��ô�.
	for (; cnt < MAXL; cnt++) {
		int max_mutual_num = 0;
		int max_mutual_id = 0;
		for (register int i = 1; i <= gN; i++) { // �Բ� �ƴ� ģ���� ���� ���� ���� �̴´�.
			if (num_mutual[i] > max_mutual_num) {
				max_mutual_num = num_mutual[i];
				max_mutual_id = i;
			}
		}
		if (max_mutual_num == 0) break;
		list[cnt] = max_mutual_id; // ������ �ϰ� ���� ���ÿ��� �����ǵ���
		num_mutual[max_mutual_id] = 0; // num_mutual �� 0 ���� ����� �ݴϴ�.
	}
	return cnt;
}
