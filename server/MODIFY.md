1. main.cc 추가
2. shared.cc static 구조 변경
 - singleton방식으로 개선
3. client_thread.cc에서 shared.cc의 deleteclient Queue와 관련된 부분을 직접 호출하지 않고 deletedClientThreads에 추가하도록 변경
4. clientQueueManagement(대기열 관리 큐) 쓰레드로동작하도록 변경
5. shared.cc, client_thread 소멸될때 쓰레드 정리할 수 있도록 변경 => stop_추가하여 쓰레드 종료 가능하도록