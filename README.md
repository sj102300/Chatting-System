
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

