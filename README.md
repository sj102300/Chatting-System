
## 요구사항 명세

1. client와 server간의 에코 서버 시스템을 구현한다.
2. client는 server와 연결을 맺은 후, 자신의 id를 보낸다
3. 이후 client는 데이터를 보낸다. 데이터의 형식은 아래와 같다.
    ```
    typedef struct _data{
        int flag;
        unsigned int data_len;
        char data[1000];
    }
    ```

4. server는 client + id : message 형식으로 input.txt파일에 데이터를 저장한다.

## 설계
1. mutex를 이용해 input.txt(공유자원)에 클라이언트 한명씩 접근할 수 있도록 한다.
2. fd_set과 select()를 이용해 connection을 맺는 client수는 3명으로 제한한다.

## 동작 흐름

### 메인스레드

1. arrange 스레드를 생성한다.
2. 클라이언트의 연결 요청을 받아서 fd값을 할당한다.
3. waitingQueue에 clientfd를 push한다. (Shared::queuePush()호출)

### arrange 스레드

1. 작업이 끝난 client thread 객체를 삭제한다. (Shared::arrangeClientThread()호출)
2. 해당 작업을 1초에 한번 수행한다.

### clientThread

1. waitingQueue에 clientfd가 있고, MAX_CLIENT수보다 작으면 스레드를 만들어서 클라이언트의 입력을 받는다.
2. 클라이언트의 메시지를 받아서 파일에 쓴다.
3. 종료 메시지를 받으면 removeClientfd를 호출하여 스레드 종료 flag를 남기고 작업을 종료한다.

## 피드백 내용

1. 이렇게 static으로 모든 스레드에서 접근이 가능하도록 설계할거라면 차라리 singleton 패턴을 사용하는 것이 좋다.
2. lock_guard()의 유효 범위는 block 안에서만 이다.
    ```
    if(!hasMtx)
        lock_guard()...
    ```
    이렇게 쓰면 if문 안에서만 유효한것
3. Shared에 clientThread가 종속되어있는 구조. 스레드는 Shared에 파일 입출력 관련 요청만 처리하도록 설계하는 것이 좋다.
현재는 Shared에서 ClientThread까지 모두다 관리를 하고 이게 static으로 만들어져있기때문에 ,, 안전하지 않다
4. 이럴거면 Shared를 관리하는 스레드를 추가하는것도 방법일수도
