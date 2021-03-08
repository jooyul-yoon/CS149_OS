
// fork example

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    // 프로세스 번호가 저장될 변수
    pid_t pid;

    // 자식 프로세서 생성
    // 성공시 0, 실패시 -1, 부모 프로세스이면 양수를 반환
    pid = fork();

    if (pid < 0) {
        // pid가 음수이면 에러가 난 것.
        fprintf(stderr, "fort failed!");
        return -1;
    }
    else if (pid == 0){
        // pid가 0 이면 자식 프로세스
        // 자식이 해야할 일 선언
        fprintf("Child Process (# %i)", getpid());
    }
    else {
        // pid 가 양수이면 부모프로세스
        // 부모가 해야할 일 선언
        fprintf("Parent Process (# %i)", getpid());

        // 자식 프로세스들이 모두 종료될 때까지 기다린다.
        wait(NULL);
    }

    return 0;
}