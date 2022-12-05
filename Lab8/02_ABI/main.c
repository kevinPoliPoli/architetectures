extern int check_square(int, int, int);
extern float my_division(float* a, float* b);
float __aeabi_fdiv (float a ,float b);

extern char ROWS;
extern char COLUMNS;
extern int Matrix_Coordinates[];


int main(void){
	
	/*		EXERCISE 1		*/

	float r = 2; 
	volatile float pi;
	float area = 0;
	volatile int j, a = 0, b = 0;		

	for(j=0; j<COLUMNS*ROWS ;j+=2){
			a = Matrix_Coordinates[j];
			b = Matrix_Coordinates[j+1];
			area += check_square(a, b, r);
		}

	r = r*r;
		
	pi = my_division(&area, &r);
		
	/*		EXERCISE 2		*/
		
	__asm__("svc 0xca");
		
	__asm__("svc 0xfe");
	
		
	while(1);
}
