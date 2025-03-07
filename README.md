# 📜 요구사항 명세
1. Client와 Server간의 **채팅** 시스템을 개발한다.
2. Server는 Client에서 온 메시지를 LogFile에 client의 id와 함께 저장한다.
3. Client는 맨 처음 자신의 고유 Id를 보내고 이후에 메시지를 보낸다.
4. Client는 종료 요청시 "exit"을 전송한다.
5. Server는 exit 메시지를 받으면 연결을 해제하고 할당받았던 fd값을 반환한다.
6. Server는 **최대 3명의 Client**까지 **동시에 처리**할 수 있다. (최대 3개의 ClientThread만 사용할 수 있다)
7. 이후에 접속하는 Client들은 대기 이후에 전송할 수 있다.

# 💡 구현시 고려사항
1. 각 공유자원은 접근시, 동시성 문제가 발생하지 않도록 **mutex**를 이용한다.
2. 공유자원은 **Singleton** 패턴을 이용하여, 하나의 instance로 관리한다.
3. Client 요청의 처리 순서는 **FIFO 구조**를 지킨다.

# 🏗️ 설계
![flow chart](https://github.com/user-attachments/assets/fd8b3d98-cb13-472b-b8f7-153ce1c3c127)

## 🌟 전체 흐름
1. Client 요청이 들어오면 Main Thread가 이를 Waiting Queue에 push한다.
2. Client Management Thread가 대기 중인 요청을 읽어 새로운 Client Thread를 생성한다.
3. Client Thread는 client의 메시지를 받아 Log File에 작성한다.
4. Client Thread는 종료 메시지를 수신하면 Delete Queue에 자기 Id를 Push한다.
5. Arrange ClientThread Thread는 Delete Queue에서 Id를 가져와 Client Thread를 정리하고 제거한다.

## 🍀 공유자원(Shared)
* Waiting Queue: TCP 연결 맺은 후에, 메시지 보내기를 기다리는 Client들의 대기열 큐
* Current Working ClientThread Set: 현재 작업중인 Client Thread를 관리하기 위한 set
* Log File: Client가 송신한 메시지를 받아 기록할 파일
* Delete Queue: 작업 종료 후에 정리되길 기다리는 thread의 대기열 큐

## 1️⃣ Main Thread
1. 클라이언트의 요청을 수신한다.
2. 클라이언트와 통신할 socket을 열고 해당 fd값을 waiting queue에 push한다.
3. 다음 클라이언트를 기다린다.

## 2️⃣ Client Management Thread
1. Waiting Queue를 확인하여 대기중인 Client를 Pop한다. 
    * 대기중인 클라이언트가 없다면 대기한다.
2. 현재 실행 중인 스레드의 개수를 확인하여 최대 클라이언트 수(maxClient)를 넘지 않는지 체크한다.
    * 최대 클라이언트 수보다 크다면 현재 실행중인 Client Thread의 작업 종료까지 대기한다.
3. 새로운 Client Thread를 생성한다.
4. 새로운 Client의 Fd값을 Current Working ClientThread set에 추가한다.

## 3️⃣ Client Thread
1. Client에게서 Id를 수신한다.
2. Client에서 보낸 메시지를 수신한다.
3. 메시지를 Log File에 Id와 함께 기록한다.
4. 종료 메시지를 수신하면 Delete Queue에 자신의 Fd값을 적고 작업을 종료한다.

## 4️⃣ Arrange ClientThread Thread
1. Delete Queue를 확인하여 제거할 Client Thread를 Pop한다.
    * 제거할 Client Thread가 없다면 대기한다.
2. 해당 thread를 종료하고 소멸시킨다. 
2. 해당 Client Fd의 값을 Current Working ClientThread Set에서 제거한다.
