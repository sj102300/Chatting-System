#!/bin/bash

# 서버 소스 파일 경로 설정
SERVER_FILES=(
  "server.cc"
  "shared.cc"
  "client_thread.cc"
  "main.cc"
)

# 컴파일 옵션 설정
# -std=c++17 : C++17 표준 사용
# -g       : 디버그 심볼 포함 (디버그 빌드)
# -O0      : 최적화 제거
CXXFLAGS="-std=c++17 -g -O0"

# 헤더 파일이 위치한 디렉토리 지정
INCLUDES="-I."

# 출력 실행 파일 이름 지정
OUTPUT="server"

# g++로 서버 소스 파일들을 컴파일하여 실행 파일 생성
g++ ${CXXFLAGS} ${INCLUDES} "${SERVER_FILES[@]}" -o ${OUTPUT}

# 빌드 결과 확인
if [ $? -eq 0 ]; then
  echo "서버 빌드가 완료되었습니다: ${OUTPUT}"
else
  echo "빌드 중 오류가 발생했습니다."
fi
