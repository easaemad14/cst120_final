/*
 * Author:	Easa El Sirgany
 * Email:	easa.elsirgany@oit.edu
 * Date:	06/01/2016
 * File:	morse.ino
 *
 * Description:	This is my final project for CST 120, which takes input from the user in the
 * 		form of a push button, and outputs the corresponding letter to a seven segment
 *		LED.
 *
 *		Code should be sufficiently commented where needed for additional information.
 *		For any questions or bug reports, please contact the author.
 */

/*
 * The following should be included in a header file, however, externs are not usable in the
 * Arduino IDE (I found out the hard way), so I am going to exhibit some poor programming
 * structure here, and put all of this code into one file :^)
 */

//==============================================================================================
// Global variables and macros
//==============================================================================================
/*
 * My button input is mapped to pin 2 on the UNO, and my seven segment LED pins are mapped from
 * pins 7 to 13. I will use these for simplicity of my code, as seen in morse.ino.
 *
 * Also, I prefer looking at the binary tree for my morse encoder (.png file attached), and I
 * need a height of four to cover all of my characters in the alphabet, so we will use a four
 * byte int array, as shown below.
 */
#define BUTTON_PIN 2
#define DOT 0
#define DASH 1
#define MORSE_HEIGHT 4
#define MORSE_LENGTH 8

/*
 * Seven Segment macros
 *
 * The following macros are based off of the pins that I have mapped accordingly:
 *	pin 06: a
 * 	pin 07: b
 *	pin 08: c
 *	pin 09: d
 *	pin 10: e
 *	pin 11: f
 *	pin 12: g
 *	pin 13: h
 */
#define SS_START 6
#define SS_END 13
#define SS_CHAR_A 0x77
#define SS_CHAR_B 0x78
#define SS_CHAR_C 0x39
#define SS_CHAR_D 0x5E
#define SS_CHAR_E 0x79
#define SS_CHAR_F 0x71
#define SS_CHAR_G 0x7D
#define SS_CHAR_H 0x76
#define SS_CHAR_I 0x30
#define SS_CHAR_J 0x1E
#define SS_CHAR_K 0x70
#define SS_CHAR_L 0x38
#define SS_CHAR_M 0x64
#define SS_CHAR_N 0x52
#define SS_CHAR_O 0x3F
#define SS_CHAR_P 0x73
#define SS_CHAR_Q 0xBF
#define SS_CHAR_R 0x31
#define SS_CHAR_S 0x6C
#define SS_CHAR_T 0x78
#define SS_CHAR_U 0x1C
#define SS_CHAR_V 0x3E
#define SS_CHAR_W 0xBE
#define SS_CHAR_X 0x49
#define SS_CHAR_Y 0x6E
#define SS_CHAR_Z 0x5B
#define SS_CHAR_0 0x80


//I prefer to begin all of my global vars with "g_" so I can easily tell
volatile bool g_button_state = 0;
volatile bool g_reset = 0;
char g_morse;
char g_mlen;

#define SET_SS(C, L) {					\
		int _i, _j;				\
		for(_i = SS_START; _i < L; _i++){	\
			digitalWrite(_i, C | _j);	\
			_j <<= 1;}}

//==============================================================================================
// Functions
//==============================================================================================
char decode(char byt, char len){
}

char encode(char let, char len){
}

/*
 * setup is one of the two required functions according to the kit that I was provided with.
 * This function will execute once upon startup, and I will use it to setup my pins as seen
 * below.
 */
void setup(){
	int i; //Used for setting up all pins for our seven seg

	pinMode(BUTTON_PIN, INPUT);
	for(i = SS_START; i <= SS_END; i++)
		pinMode(i, OUTPUT);
	
	g_morse = 0;
	g_mlen = 0;
	SET_SS(SS_CHAR_0, MORSE_LENGTH);
}

void loop(){
	g_button_state = digitalRead(BUTTON_PIN);
	if(!g_button_state){ //Active high
		char morse_char;

		//If we maxed out morse code character, reset and then start
		if(g_mlen == MORSE_HEIGHT){
			SET_SS(SS_CHAR_0, MORSE_LENGTH);
			g_mlen = 0;
		}

		morse_char = decode(g_morse, g_mlen);

		/*
		 * We have to be very careful with our delays! I wanted to do this as a fsm,
		 * but having delays inline will cause problems. You could always set up an
		 * interrupt (not sure if the UNO has an interrupt vector table), but this
		 * would be a bit too much work for this type of project, so let's keep it as
		 * simple as we can.
		 */
		delay(250);
		if(!g_button_state) //This would mean that it's still pressed, so dash
			morse_char = morse_char << 1 + 1;
		else
			morse_char <<= 1;

		delay(250);
		g_morse = encode(morse_char, ++g_mlen);
		SET_SS(g_morse, g_mlen);
	}
	else if(!g_reset){
		SET_SS(SS_CHAR_0, MORSE_LENGTH);
		g_mlen = 0;
	}
}
