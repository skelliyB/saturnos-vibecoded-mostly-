#include <stdint.h>
#include "drivers/pic.h"
#include "drivers/keyboard.h"
#include "drivers/io.h"
#include "drivers/idt.h"



#define MAX_INPUT 128
#define WHITE_TXT 0x07
#define status0  "Ran without errors"
#define status1  "Errors reached"
#define yourfucked  "Kernel panic your on your own good luck"
#define RED 0x04
#define GREEN 0x02
#define BLUE  0x01
#define LIGHT_RED 0x0C
#define YELLOW 0x0E

    

int running = 1;
char input_buffer[MAX_INPUT];
int input_index = 0;
static int pos = 0;

int booleans_t(void);
void kprint(const char* str, uint8_t color);
void k_clear_screen();
int kerror(char *errorcode);
char scancode_to_ascii(uint8_t sc);
void kfgets(char* buffer, int max);
int strcmp(const char *a, const char *b);
void shutdown_KF(void);
void kernel_panic();
void printprompt(char *username);
void kbackspace();
void beep_on(uint32_t freq);
void beep_off();
void delay(int count);
void True_false_k(char* arga, char* argb);
int str_equal(const char* a, const char* b);
int katoi(const char* str);
void add_k(char* num1, char* num2);
void kprint_int(int num);
void sub_l(char* num1, char* num2);
void multiply(char* num1, char* num2);
void divide(char* num1, char* num2);
void ascii();









void kernel_main(void)
{
    pic_remap();
    //idt_init();
    //asm volatile("sti");        
    char buffer[128];
    char username[25] = "root";

    while (running)
    {
        printprompt(username);

        kfgets(buffer, MAX_INPUT);

        if (strcmp(buffer, "clear") == 0)
        {
            k_clear_screen();
        }
        if (strcmp(buffer, "echo") == 0) {
            char echoinpt[128];
            kprint("enter: echoing msg:  ", GREEN);
            kfgets(echoinpt, MAX_INPUT);
            kprint(echoinpt, WHITE_TXT);
        }

        if(strcmp(buffer,"chuser") == 0){
            kprint("enter new user: ", BLUE);
            kfgets(username, 25);
            if (username[0] == 0){
                kerror(status1);
            }
        }

        if (strcmp(buffer, "shutdown")==0){
            shutdown_KF();
        }

        if (strcmp(buffer, "kernelpanic") ==0) {
            kernel_panic();
        }
        
        if (strcmp(buffer, "beepon") ==0){
            beep_on(1000);
            delay(50);
            beep_off();
        }

        if (strcmp(buffer, "truefalse") == 0){
            char argua[128];
            char argub[128];

            kprint("num 1: ", WHITE_TXT);
            kfgets(argua, MAX_INPUT);

            kprint("\nnum 2: ", WHITE_TXT);
            kfgets(argub, MAX_INPUT);

            True_false_k(argua, argub);
        }

        if (strcmp(buffer, "add") == 0){
            char numb1[128];
            char numb2[128];
            kprint("firstnumber: ", WHITE_TXT);
            kfgets(numb1, MAX_INPUT);
            kprint("second: ", WHITE_TXT);
            kfgets(numb2, MAX_INPUT);
            add_k(numb1, numb2);

        }
        if (strcmp(buffer, "sub") == 0){
            char numb1[128];
            char numb2[128];
            kprint("firstnumber: ", WHITE_TXT);
            kfgets(numb1, MAX_INPUT);
            kprint("second: ", WHITE_TXT);
            kfgets(numb2, MAX_INPUT);
            sub_l(numb1, numb2);

        }

        if (strcmp(buffer, "mult")==0){
            char numb1[128];
            char numb2[128];
            kprint("first number: ", WHITE_TXT);
            kfgets(numb1, MAX_INPUT);
            kprint("second number: ", WHITE_TXT);
            kfgets(numb2, MAX_INPUT);
            multiply(numb1, numb2);

        }

        if (strcmp(buffer, "div")==0){
            char numb1[128];
            char numb2[128];
            kprint("first number: ", WHITE_TXT);
            kfgets(numb1, MAX_INPUT);
            kprint("second number: ", WHITE_TXT);
            kfgets(numb2, MAX_INPUT);
            divide(numb1, numb2);

        }

        if (strcmp(buffer, "ascii")==0){
            ascii();
        }
        kprint("\n", WHITE_TXT);
    }
}

void kprint(const char* str, uint8_t color)
{
    volatile char* video = (volatile char*)0xB8000;

    

    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == '\n')
        {
            pos += (160 - (pos % 160)); 
            continue;
        }

        video[pos] = str[i];
        video[pos + 1] = color;
        pos += 2;
    }
}

void kprint_int(int num) {
    char buf[16];
    int i = 0;

    if (num == 0) {
        kprint("0", WHITE_TXT);
        return;
    }

    while (num > 0) {
        buf[i++] = '0' + (num % 10);
        num /= 10;
    }

    for (int j = i - 1; j >= 0; j--) {
        char c[2] = {buf[j], 0};
        kprint(c, WHITE_TXT);
    }
}

void k_clear_screen()
{

	char *vidmem = (char *) 0xb8000;
	unsigned int i=0;
	while(i < (80*25*2))
	{
		vidmem[i]=' ';
		i++;
		vidmem[i]=WHITE_TXT;
		i++;
	};

    pos = 0;

    
};

int kerror(char *errorcode) {
    
    if (strcmp(errorcode, status1)== 0){
        kprint("error recived", RED);
        return 1;
    }

    else if (strcmp(errorcode, yourfucked) == 0){
        kprint(yourfucked, RED);
        return -1;
    }
    
    
    return 0;
}



void kfgets(char* out, int max)
{
    int i = 0;

    while (1)
    {
        char c = keyboard_read_char();

        if (c == '\n')
        {
            out[i] = '\0';
            kprint("\n", 0x07);
            return;
        }

        if (c == '\b')
        {
            if (i > 0)
            {
                i--;
                kbackspace();
            }
            continue;
        }

        if (i < max - 1)
        {
            out[i++] = c;

            char tmp[2] = {c, 0};
            kprint(tmp, 0x07);
        }
    }
}

int strcmp(const char *a, const char *b) {
    while (*a && (*a == *b)) {
        a++;
        b++;
    }
    return *(unsigned char*)a - *(unsigned char*)b;
}

void shutdown_KF(void)
{
    outw(0x604, 0x2000);
}

void kernel_panic(){
    kprint("good luck your on your own", RED);
    kerror(yourfucked);
    running = 0;
    
}

void printprompt(char *user)
{
    kprint(user, WHITE_TXT);
    kprint(": ", WHITE_TXT);
}

void kbackspace()
{
    if (pos <= 0) return;

    pos -= 2;  

    volatile char* video = (volatile char*)0xB8000;
    video[pos] = ' ';
    video[pos + 1] = WHITE_TXT;
}

void beep_on(uint32_t freq) {
    if (freq == 0) return;

    uint32_t div = 1193180 / freq;

    outb(0x43, 0xB6);

    outb(0x42, (uint8_t)(div & 0xFF));
    outb(0x42, (uint8_t)((div >> 8) & 0xFF));

    uint8_t tmp = inb(0x61);
    outb(0x61, tmp | 3);
}

void beep_off(void) {
    uint8_t tmp = inb(0x61) & 0xFC;
    outb(0x61, tmp);
}

void delay(int count) {
    for (volatile int i = 0; i < count; i++) {
        for (volatile int j = 0; j < 10000; j++) {
            __asm__ volatile ("nop");
        }
    }
}

int str_equal(const char* a, const char* b) {
    int i = 0;

    while (a[i] && b[i]) {
        if (a[i] != b[i]) {
            return 0; 
        }
        i++;
    }

    return a[i] == b[i]; 
}

void True_false_k(char* arga, char* argb){
    
    if (str_equal(arga, argb)) {
        kprint("True", LIGHT_RED);
    }
    else {
        kprint("False", RED);
    }

    
}

void add_k(char* num1, char* num2){
    int result = katoi(num1) + katoi(num2);
    kprint("Result: ", GREEN);
    kprint_int(result);
}

void sub_l(char* num1, char* num2){
    int result = katoi(num1) - katoi(num2);
    kprint("Result: ", GREEN);
    kprint_int(result);
}

int katoi(const char* str) {
    int result = 0;
    int i = 0;

    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
        i++;
    }

    return result;
}

void multiply(char* num1, char* num2){
    int result = katoi(num1) * katoi(num2);
    kprint("result: ", GREEN);
    kprint_int(result);
}

void divide(char* num1, char* num2){
    int b = katoi(num2);

    if (b == 0) {
        kerror("DIV_ZERO");
        kprint("Cannot divide by zero\n", RED);
        return;
    }

    int result = katoi(num1) / b;
    kprint("result: ", GREEN);
    kprint_int(result);
}

void ascii(){

    char input[128];
    kprint("enter ascii 1,2,3: ",GREEN);

    kfgets(input, MAX_INPUT);

    char art1[] =
    "        ,MMM8&&&.\n"
    "    _...MMMMM88&&&&..._\n"
    " .::'''MMMMM88&&&&&&'''::.\n"
    "::     MMMMM88&&&&&&     ::\n"
    "'::....MMMMM88&&&&&&....::'\n"
    "   `''''MMMMM88&&&&''''`\n"
    "   jgs   'MMM8&&&'\n";

    char art2[] =
    "        _nnnn_\n"
    "        dGGGGMMb\n"
    "       @p~qp~~qMb\n"
    "       M|@||@) M|\n"
    "       @,----.JM|\n"
    "        JS^\\__/  qKL\n"
    "     dZP        qKRb\n"
    "    dZP          qKKb\n"
    "   fZP            SMMb\n"
    "   HZM            MMMM\n"
    "   FqM            MMMM\n"
    " __| \".        |\\dS\"qML\n"
    " |    `.       | `' \\Zq\n"
    "_)      \\.___.,|     .'\n"
    "\\____   )MMMMMP|   .'\n"
    "     `-'       `--' hjm\n";

    char art3[] =
    "        ***************        \n"
    "     ***             ***      \n"
    "   **                   **    \n"
    "  **         | |         **   \n"
    " **          | |          **  \n"
    " **        ---+---        **  \n"
    " **          | |          **  \n"
    " **          | |          **  \n"
    "  **                     **   \n"
    "   **                   **    \n"
    "     ***             ***      \n"
    "        ***************        \n";


    if (strcmp(input, "1")==0){
        kprint(art1,YELLOW);
    }
    if (strcmp(input, "2")==0){
        kprint(art2,WHITE_TXT);
    }
    if (strcmp(input, "3")==0){
        kprint(art3,LIGHT_RED);
    }
}