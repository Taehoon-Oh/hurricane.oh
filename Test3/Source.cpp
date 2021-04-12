#include <stdio.h>

#define MAX 100000
#define USER 100000

class News{
public: 
	int newsid;
	int totalreadcnt; 
	int sectionid; 
	bool deleted; 
} news[MAX + 1]; 
int newscnt=0; 


#define SECTION 10
//���� id ���� �� 10���� ����PQ �� ����� ������ ��������� new ����ü�� totalreadcnt �� �������� ������ ��� �Ѵ�. 
priotiry_que sectionPQ[SECTION + 1]

//����ں� �������� ���� ������ �����Ѵ�. 
int lastusersection[USER + 1]; 

void newNews(int nid, int section) {
	// �̷������� ��������ü ���� ������ְ� 
	news[nid] = { nid, 0, section, false }; 

	//�ش� ���� id ���� �����ϴ� priorityque �� �ִ´�. 
	sectionPQ[section].heappush(&news[nid]); // �� �̷������� 

}


//����ڰ� ������ �д´�. 
void readNews(int newid, int userid) {
	// �ش� ���� ���� ���ڸ� Ű���ְ� 
	news[newid].tottalreadcnt++; 

	//�� ����ڰ� �������� ���� section�� ����صΰ� 
	int lastsection = news[newid].sectionid;; 
	lastusersection[userid] = lastsection;


	//�ش� ������ read ���ڰ� �޶������� ������ �ٽ� �����Ѵ�. 
	sectionPQ[lastsection], ==> ���⼭ ������ �������ϴ°� �Ѵ�. ( up , down �� Ȱ���ϸ� �ȴ�. )

}


// �����ϴ� ����ڿ��� ��� 10���� ��õ�ض� ( �ִ� ��ȸ�� + �ش� ����ڰ� ������ ���� ���� ������ ���� ) 
int recommend(int userid , int list[10] ) {
	int lastsection = lastusersection[userid]; 
	1. ������ ������ PQ �� ����ִ� 10���� ���ؼ� �������� �ΰ��Ѵ�. (���� ����Ʈ�� ����� )

	2. �� ���Ǻ� ž 10���� �� ���� ���� �� �񱳸� �ؼ� ���� 10���� �����Ѵ�.

	3. ���� 10���� ��ȯ�Ѵ�. 

	

}


