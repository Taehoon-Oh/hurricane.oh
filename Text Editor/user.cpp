#define MAX_N 700
#define NULL 0

struct Node {
    char ch;
    Node* prev;
    Node* next;
};

struct Line {
    Node* head;
    Node* tail;
    int len;
    Line* prev;
    Line* next;
};

Line LinePool[MAX_N];
int LinePoolCnt;
Node NodePool[MAX_N * (MAX_N + 1)];
int NodePoolCnt;
Line* Head = NULL;
Line* CurLine;
Node* CurNode;
int CurColIdx;

void init(int n) {
    LinePoolCnt = 0;
    NodePoolCnt = 0;

    CurNode = &NodePool[NodePoolCnt++];
    CurNode->prev = CurNode->next = NULL;
    CurNode->ch = 0;

    Head = &LinePool[LinePoolCnt++];
    CurLine = Head;
    CurLine->head = CurLine->tail = CurNode;
    CurLine->len = 0;
    CurLine->prev = CurLine->next = NULL;

    CurColIdx = 0;
}

void input_char(char in_char) {
    Node* newNode = &NodePool[NodePoolCnt++];

    newNode->prev = CurNode->prev;
    newNode->next = CurNode;
    if (CurNode->prev) {
        CurNode->prev->next = newNode;
    }
    else {
        CurLine->head = newNode;
    }
    CurNode->prev = newNode;

    newNode->ch = in_char;
    CurLine->len++;
    CurColIdx++;
}

void input_newline() {
    Line* newLine = &LinePool[LinePoolCnt++];

    newLine->prev = CurLine;
    newLine->next = CurLine->next;
    if (CurLine->next) {
        CurLine->next->prev = newLine;
    }
    CurLine->next = newLine;

    Node* newTailNode = &NodePool[NodePoolCnt++];
    newTailNode->prev = newTailNode->next = NULL;
    newTailNode->ch = 0;

    if (CurNode->ch == 0) {
        newLine->head = newLine->tail = newTailNode;
        newLine->len = 0;
    }
    else {
        if (CurNode->prev) {
            CurNode->prev->next = newTailNode;
            newTailNode->prev = CurNode->prev;
        }
        else {
            CurLine->head = newTailNode;
        }

        newLine->head = CurNode;
        CurNode->prev = NULL;
        newLine->tail = CurLine->tail;
        newLine->len = CurLine->len - CurColIdx;

        CurLine->tail = newTailNode;
        CurLine->len = CurColIdx;
    }

    CurLine = newLine;
    CurNode = newLine->head;
    CurColIdx = 0;
}

void move_cursor(int direction) { // 0: Up, 1: Down, 2: Left, 3: Right
    switch (direction) {
    case 0:
        if (CurLine->prev) {
            CurLine = CurLine->prev;

            if (CurColIdx >= CurLine->len) {
                CurColIdx = CurLine->len;
                CurNode = CurLine->tail;
            }
            else {
                Node* pNode = CurLine->head;
                for (int i = 0; i < CurColIdx; ++i) {
                    pNode = pNode->next;
                }
                CurNode = pNode;
            }
        }
        break;
    case 1:
        if (CurLine->next) {
            CurLine = CurLine->next;

            if (CurColIdx >= CurLine->len) {
                CurColIdx = CurLine->len;
                CurNode = CurLine->tail;
            }
            else {
                Node* pNode = CurLine->head;
                for (int i = 0; i < CurColIdx; ++i) {
                    pNode = pNode->next;
                }
                CurNode = pNode;
            }
        }
        break;

    case 2:
        if (CurColIdx > 0) {
            CurColIdx--;
            CurNode = CurNode->prev;
        }
        else if (CurLine->prev) {
            CurLine = CurLine->prev;
            CurNode = CurLine->tail;
            CurColIdx = CurLine->len;
        }
        break;
    case 3:
        if (CurColIdx < CurLine->len) {
            CurColIdx++;
            CurNode = CurNode->next;
        }
        else if (CurLine->next) {
            CurLine = CurLine->next;
            CurNode = CurLine->head;
            CurColIdx = 0;
        }
        break;
    }
}

char get_char(int row, int column) {
    register Line* pLine = Head;
    for (register int i = 0; i < row - 1; ++i)
        pLine = pLine->next;

    register Node* pNode = pLine->head;
    for (register int i = 0; i < column - 1; ++i)
        pNode = pNode->next;

    return pNode->ch;
}