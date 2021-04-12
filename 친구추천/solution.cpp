#define NULL    0
#define MAXL    5
#define MAXF    10

struct Node {
	int id;
	Node* next;
}; //   전체 함수 호출 횟수는 50,000 이하, 친구 관계의 수 최대 500,000 개 => 노드 1,000,000 개 필요
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
void init(int N) { // N 사용자의 수, 사용자 ID <= N, N <= 10,000
	last_alloc = 0;
	gN = N;
	for (register int id = 1; id <= N; id++) // 초기화
		for (register int key = 0; key < TBL_SIZE; key++)
			user[id].friend_list[key].next = NULL;
}

void add(int id, int F, int ids[MAXF]) {  // 1 <= F <= 10
	register int fid;
	register int key;
	register Node* node;
	while (F--) {
		fid = ids[F]; // id 에 친구 fid 추가
		key = fid % TBL_SIZE;
		node = new_Node();
		node->id = fid;
		node->next = user[id].friend_list[key].next; // 리스트의 맨 앞에 추가한다.
		user[id].friend_list[key].next = node;

		key = id % TBL_SIZE; // fid 에 친구 id 추가
		node = new_Node();
		node->id = id;
		node->next = user[fid].friend_list[key].next;
		user[fid].friend_list[key].next = node;
	}
}

void del(int id1, int id2) { // 서로의 친구 목록에서 삭제한다.
	register int key = id2 % TBL_SIZE; // id1 의 친구목록에서 id2 를 지운다.
	register Node* node = &(user[id1].friend_list[key]);
	while (node->next != NULL) {
		if (node->next->id == id2) { // 목록에서 id 를 찾아 지운다.
			node->next = node->next->next;
			break;
		}
		node = node->next;
	}

	key = id1 % TBL_SIZE; // id2 의 친구목록에서 id1 을 지운다.
	node = &(user[id2].friend_list[key]);
	while (node->next != NULL) {
		if (node->next->id == id1) {
			node->next = node->next->next;
			break;
		}
		node = node->next;
	}
}

int num_mutual[MAX_N + 1]; // id 함께 아는 친구의 수를 카운트하기 위한 용도
int recommend(int id, int list[MAXL]) {
	/*
		함께 아는 친구의 수가 가장 많은 순서로 최대 5 명을 추천한다.
		함께 아는 친구의 수가 같은 경우 사용자 ID 가 작은 값을 가지는 사용자를 우선한다.
		최대 호출 횟수는 40 이하 <= 50000
	*/
	for (register int i = 1; i <= gN; i++)
		num_mutual[i] = 0;

	register Node* node;
	register int key;
	for (register int k = 0; k < TBL_SIZE; k++) {
		Node* friend_node; // 내 친구들을 순회하면서
		for (friend_node = user[id].friend_list[k].next; friend_node != NULL; friend_node = friend_node->next) {
			int fid = friend_node->id;
			for (key = 0; key < TBL_SIZE; key++) { // 친구들의 친구들을 방문하면서
				for (node = user[fid].friend_list[key].next; node != NULL; node = node->next)
					num_mutual[node->id]++; // 친구들의 친구의 등장 횟수를 센다.
			}
		}
	}

	num_mutual[id] = 0; // 추천에서 자신과 자신의 친구는 제외 해야한다.
	for (key = 0; key < TBL_SIZE; key++)
		for (node = user[id].friend_list[key].next; node != NULL; node = node->next)
			num_mutual[node->id] = 0;

	register int cnt = 0; // 추천할 친구를 최대 5명 찾아봅시다.
	for (; cnt < MAXL; cnt++) {
		int max_mutual_num = 0;
		int max_mutual_id = 0;
		for (register int i = 1; i <= gN; i++) { // 함께 아는 친구가 제일 많은 것을 뽑는다.
			if (num_mutual[i] > max_mutual_num) {
				max_mutual_num = num_mutual[i];
				max_mutual_id = i;
			}
		}
		if (max_mutual_num == 0) break;
		list[cnt] = max_mutual_id; // 선택을 하고 다음 선택에서 배제되도록
		num_mutual[max_mutual_id] = 0; // num_mutual 을 0 으로 만들어 줍니다.
	}
	return cnt;
}
