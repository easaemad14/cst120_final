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
#define DOT_BUTTON 2
#define DASH_BUTTON 3
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
#define SS_CHAR_B 0x7C
#define SS_CHAR_C 0x39
#define SS_CHAR_D 0x5E
#define SS_CHAR_E 0x79
#define SS_CHAR_F 0x71
#define SS_CHAR_G 0x7D
#define SS_CHAR_H 0x76
#define SS_CHAR_I 0x30
#define SS_CHAR_J 0x0E
#define SS_CHAR_K 0x70
#define SS_CHAR_L 0x38
#define SS_CHAR_M 0x37
#define SS_CHAR_N 0x54
#define SS_CHAR_O 0x5C
#define SS_CHAR_P 0x73
#define SS_CHAR_Q 0xBF
#define SS_CHAR_R 0x31
#define SS_CHAR_S 0x6D
#define SS_CHAR_T 0x78
#define SS_CHAR_U 0x1C
#define SS_CHAR_V 0x3E
#define SS_CHAR_W 0x7E
#define SS_CHAR_X 0x40
#define SS_CHAR_Y 0x6F
#define SS_CHAR_Z 0x5B
#define SS_CHAR_0 0x80

/*
 * This part gets a little tricky. Our tree height is four, meaning we only need four bits to
 * represent a letter, however, we have 26 letters. How does this make sense? Let's think in
 * terms of don't care bits (like a real EE).
 * 
 * Because I am using only a dot (0) and a dash (1), this doesn't seem like it's possible
 * without any other variables. In fact, we do have another variable: the length of the
 * character that was inputted. One dot (0x00) could represent an E (length one), or it can
 * represent an I, S, or H as well for length 2, 3, and 4 respectively.
 */
#define M_CHAR_A 0x01
#define M_CHAR_B 0x08
#define M_CHAR_C 0x0A
#define M_CHAR_D 0x04
#define M_CHAR_E 0x00
#define M_CHAR_F 0x02
#define M_CHAR_G 0x06
#define M_CHAR_H 0x00
#define M_CHAR_I 0x00
#define M_CHAR_J 0x07
#define M_CHAR_K 0x05
#define M_CHAR_L 0x04
#define M_CHAR_M 0x03
#define M_CHAR_N 0x02
#define M_CHAR_O 0x07
#define M_CHAR_P 0x06
#define M_CHAR_Q 0x0D
#define M_CHAR_R 0x02
#define M_CHAR_S 0x00
#define M_CHAR_T 0x01
#define M_CHAR_U 0x01
#define M_CHAR_V 0x01
#define M_CHAR_W 0x03
#define M_CHAR_X 0x09
#define M_CHAR_Y 0x0B
#define M_CHAR_Z 0x0C

//I prefer to begin all of my global vars with "g_" so I can easily tell
volatile bool g_dot_button;
volatile bool g_dash_button;
char g_morse;
char g_mlen;

/*
 * This macros seems a little scary at first, but it's actually quite simple. Our character is
 * essentially just a mapping of the pins, with 0x01 corresponding to SS_START, and so on. In
 * order for me to check if each position in our char is set, I use the variable _j to check
 * each bit with the logical AND. This is done with the ternary operator, which either sets the
 * pin to HIGH or LOW.
 */
#define SET_SS(C) {									\
		int _i, _j = 1;								\
		for(_i = SS_START; _i <= SS_END; _i++){					\
			(C & _j) ? digitalWrite(_i, HIGH) : digitalWrite(_i, LOW);	\
			_j <<= 1;}}

//==============================================================================================
// Functions
//==============================================================================================
//Using a SS char, return the appropriate M char
char decode(char byt){
	//Is there an algorithmic way to do this? Probably if I set my pins on SS differently
	if(byt == SS_CHAR_A) return M_CHAR_A;
	if(byt == SS_CHAR_B) return M_CHAR_B;
	if(byt == SS_CHAR_C) return M_CHAR_C;
	if(byt == SS_CHAR_D) return M_CHAR_D;
	if(byt == SS_CHAR_E) return M_CHAR_E;
	if(byt == SS_CHAR_F) return M_CHAR_F;
	if(byt == SS_CHAR_G) return M_CHAR_G;
	if(byt == SS_CHAR_H) return M_CHAR_H;
	if(byt == SS_CHAR_I) return M_CHAR_I;
	if(byt == SS_CHAR_J) return M_CHAR_J;
	if(byt == SS_CHAR_K) return M_CHAR_K;
	if(byt == SS_CHAR_L) return M_CHAR_L;
	if(byt == SS_CHAR_M) return M_CHAR_M;
	if(byt == SS_CHAR_N) return M_CHAR_N;
	if(byt == SS_CHAR_O) return M_CHAR_O;
	if(byt == SS_CHAR_P) return M_CHAR_P;
	if(byt == SS_CHAR_Q) return M_CHAR_Q;
	if(byt == SS_CHAR_R) return M_CHAR_R;
	if(byt == SS_CHAR_S) return M_CHAR_S;
	if(byt == SS_CHAR_T) return M_CHAR_T;
	if(byt == SS_CHAR_U) return M_CHAR_U;
	if(byt == SS_CHAR_V) return M_CHAR_V;
	if(byt == SS_CHAR_W) return M_CHAR_W;
	if(byt == SS_CHAR_X) return M_CHAR_X;
	if(byt == SS_CHAR_Y) return M_CHAR_Y;
	if(byt == SS_CHAR_Z) return M_CHAR_Z;
	return SS_CHAR_0; //Default
}

/*
 * Encoding is a little more difficult, due to the fact that we have multiple mappings based
 * off of the size of the character.
 */
char encode(char let, char len){
	int i;

	//This will mask the number of bits we are using from our let
	char mask = 0;
	for(i = 0; i < len; i++){
		mask <<= 1;
		mask += 1;
	}

	if(len == 1){
		if((let & mask) == M_CHAR_E) return SS_CHAR_E;
		if((let & mask) == M_CHAR_T) return SS_CHAR_T;
	}
	if(len == 2){
		if((let & mask) == M_CHAR_I) return SS_CHAR_I;
		if((let & mask) == M_CHAR_A) return SS_CHAR_A;
		if((let & mask) == M_CHAR_N) return SS_CHAR_N;
		if((let & mask) == M_CHAR_M) return SS_CHAR_M;
	}
	if(len == 3){
		if((let & mask) == M_CHAR_S) return SS_CHAR_S;
		if((let & mask) == M_CHAR_U) return SS_CHAR_U;
		if((let & mask) == M_CHAR_R) return SS_CHAR_R;
		if((let & mask) == M_CHAR_W) return SS_CHAR_W;
		if((let & mask) == M_CHAR_D) return SS_CHAR_D;
		if((let & mask) == M_CHAR_K) return SS_CHAR_K;
		if((let & mask) == M_CHAR_G) return SS_CHAR_G;
		if((let & mask) == M_CHAR_O) return SS_CHAR_O;
	}
	if(len == 4){
		if((let & mask) == M_CHAR_H) return SS_CHAR_H;
		if((let & mask) == M_CHAR_V) return SS_CHAR_V;
		if((let & mask) == M_CHAR_F) return SS_CHAR_F;
		if((let & mask) == M_CHAR_L) return SS_CHAR_L;
		if((let & mask) == M_CHAR_P) return SS_CHAR_P;
		if((let & mask) == M_CHAR_J) return SS_CHAR_J;
		if((let & mask) == M_CHAR_B) return SS_CHAR_B;
		if((let & mask) == M_CHAR_X) return SS_CHAR_X;
		if((let & mask) == M_CHAR_C) return SS_CHAR_C;
		if((let & mask) == M_CHAR_Y) return SS_CHAR_Y;
		if((let & mask) == M_CHAR_Z) return SS_CHAR_Z;
		if((let & mask) == M_CHAR_Q) return SS_CHAR_Q;
	}
	return SS_CHAR_0;
}

/*
 * setup is one of the two required functions according to the kit that I was provided with.
 * This function will execute once upon startup, and I will use it to setup my pins as seen
 * below.
 */
void setup(){
	int i; //Used for setting up all pins for our seven seg

	pinMode(DOT_BUTTON, INPUT);
	pinMode(DASH_BUTTON, INPUT);
	for(i = SS_START; i <= SS_END; i++)
		pinMode(i, OUTPUT);
	
	g_dot_button = 0;
	g_dash_button = 0;
	g_morse = 0;
	g_mlen = 0;
	SET_SS(SS_CHAR_0);
}

/*
 * We have to be very careful with our delays! I wanted to do this as a fsm,
 * but having delays inline will cause problems. You could always set up an
 * interrupt (not sure if the UNO has an interrupt vector table), but this
 * would be a bit too much work for this type of project, so let's keep it as
 * simple as we can.
 */
void loop(){
	g_dot_button = digitalRead(DOT_BUTTON);
	if(g_dot_button){
		char morse_char;

		//If we maxed out morse code character, reset and then start
		//This is my only solution to resetting your char :(
		if(g_mlen == MORSE_HEIGHT)
			g_mlen = 0;

		morse_char = decode(g_morse);

		delay(250); //Let's give the user some time to let go of the button
		morse_char = (morse_char << 1) + DOT;
		g_morse = encode(morse_char, ++g_mlen);
		SET_SS(g_morse);
	}

	g_dash_button = digitalRead(DASH_BUTTON);
	if(g_dash_button){
		char morse_char;

		//If we maxed out morse code character, reset and then start
		//This is my only solution to resetting your char :(
		if(g_mlen == MORSE_HEIGHT)
			g_mlen = 0;

		morse_char = decode(g_morse);

		delay(250);
		morse_char = (morse_char << 1) + DASH;
		g_morse = encode(morse_char, ++g_mlen);
		SET_SS(g_morse);
	}
}
