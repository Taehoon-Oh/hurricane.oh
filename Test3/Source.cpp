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
//섹션 id 별로 총 10개의 섹션PQ 를 만들고 위에서 만들어지는 new 구조체의 totalreadcnt 를 기준으로 소팅을 계속 한다. 
priotiry_que sectionPQ[SECTION + 1]

//사용자별 마지막에 읽은 섹션을 저장한다. 
int lastusersection[USER + 1]; 

void newNews(int nid, int section) {
	// 이런식으로 뉴스구조체 새로 만들어주고 
	news[nid] = { nid, 0, section, false }; 

	//해당 섹션 id 별로 관리하는 priorityque 에 넣는다. 
	sectionPQ[section].heappush(&news[nid]); // 뭐 이런식으로 

}


//사용자가 뉴스를 읽는다. 
void readNews(int newid, int userid) {
	// 해당 뉴스 읽은 숫자를 키워주고 
	news[newid].tottalreadcnt++; 

	//저 사용자가 마지막에 읽은 section을 기억해두고 
	int lastsection = news[newid].sectionid;; 
	lastusersection[userid] = lastsection;


	//해당 뉴스의 read 숫자가 달라졌으니 순서를 다시 조정한다. 
	sectionPQ[lastsection], ==> 여기서 순서를 재조정하는걸 한다. ( up , down 을 활요하면 된다. )

}


// 문의하는 사용자에게 기사 10개를 추천해라 ( 최대 조회수 + 해당 사용자가 막지막 읽은 섹션 가산점 포함 ) 
int recommend(int userid , int list[10] ) {
	int lastsection = lastusersection[userid]; 
	1. 마지막 섹션의 PQ 에 들어있는 10개에 대해서 가산점을 부과한다. (별도 리스트에 저장등 )

	2. 각 섹션별 탑 10개와 위 섹션 기사와 다 비교를 해서 최종 10개를 선정한다.

	3. 최종 10개를 반환한다. 

	

}


