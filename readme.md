## 명령어
### netstat -tulnp | grep 8080
-> 특정 포트를 사용 중인 프로세스 정보 확인 가능

### chmod 755 build.sh
build.sh 파일에 대한 권한 변경

chmod -> change mode , 파일이나 디렉토리의 권한을 변경

755 -> 소유자, 그룹, 기타사용자에게 부여할 파일 권한을 나타냄(3자리 8진수 권한 코드)

* 소유자 7 -> read + write + execute / 읽기 + 쓰기 + 실행 권한 모두 가짐 (111)
* 그룹 5 & 기타사용자 5 -> read + execute (101)

## pthread.h vs std::thread
* pthread

    C언어 기반의 POSIX(Portable Operating System Interface) 표준 스레드 라이브러리
    
    유닉스 기반 시스템(Linus, macOS)에서 사용 가능

    저수준 API로 세밀한 제어 가능, 하지만 코드 복잡하고 가독성 떨어짐

* std::thread

    c++ 표준 라이브러리 기반의 스레드

    플랫폼 독립적(Windows, Linux, macOS 등에서 동작)

    std::mutex로 동기화 가능

=> 해당 구현은 모두 cpp을 이용했으므로 여기서는 thread를 이용함.


## mutex vs semaphore
동기화 설계 방식

공유된 자원의 데이터 혹은 critical section에 하나의 process 또는 thread가 접근하는 것을 막아줌

* mutex

    동기화 대상이 >>>하나<<<

    한 프로세스에 의해 소유될 수 있는 "key"를 기반으로한 mutual exclusion 기법

    이 객체를 소유한 스레드/프로세스만 공유자원에 접근 가능

    key를 얻으면 자원을 소유

    프로세스의 범위, 프로세스 종료될때 자동으로 Clean up

* semaphore

    동기화 대상이 >>>하나이상<<<

    사용하는 스레드/프로세스의 "수"를 공통으로 관리하는 하나의 값을 이용해 mutual exclusion 구현

    공유 자원에 접근할 수 있는 프로세스의 최대 허용치 만큼 동시에 사용자가 접근 가능

    자원 소유 불가

    시스템 범위, 파일시스템 상의 파일로 존재.