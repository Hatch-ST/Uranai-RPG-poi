#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<windows.h>

//�t�B�[���h�̃T�C�Y
#define FIELD_WIDTH 10
#define FIELD_HEIGHT 8
//�t�B�[���h�\���p�̘g�̃T�C�Y
#define FRAME_WIDTH FIELD_WIDTH*2+1
#define FRAME_HEIGHT FIELD_HEIGHT*2+1
//�v���C���[���̍ő啶����
#define MAX_NAME_LENGTH 20
//�G���ނ̔z�u��
#define ONE_ENEMY_NUMBER 5
//�G�S���̔z�u��
#define ALL_ENEMY_NUMBER ONE_ENEMY_NUMBER*3+1
//�X���C���̗̑�,�U����
#define SLIME_STAMINA 20
#define SLIME_ATTACK 4
//�S�u�����̗̑�,�U����
#define GOBLIN_STAMINA 15
#define GOBLIN_ATTACK 5
//�n�[�s�[�̗̑�,�U����
#define HARPY_STAMINA 18
#define HARPY_ATTACK 6
//�K�[�S�C���̗̑�,�U����
#define GARGOYLE_STAMINA 50
#define GARGOYLE_ATTACK 8
//�򑐂̉񕜃|�C���g
#define HEAL_POINT 10
//�L�[�{�[�h�̃{�^���ԍ�
#define ESC_BUTTON 0x1b
#define ENTER_BUTTON 0x0d
#define UP_BUTTON 0x48				//���L�[
#define DOWN_BUTTON 0x50			//���L�[
#define LEFT_BUTTON 0x4B			//���L�[
#define RIGHT_BUTTON 0x4D			//���L�[




//�Q�[����ʊǗ��p�萔
enum GAME_STATE {START,MAP,BATTLE,CLEAR,GAMEOVER,END};
//�G�̈ړ��^�C�v
enum MOVE_TYPE {STOP,PURSUE,RANDOM};
//�A�C�e���ԍ�
enum ITEM_NUMBER {NOTHING,HEAL_ITEM,POWER_ITEM};
//�����Ă��邩����ł��邩
enum ENEMY_STATE {ALIVE,DEAD};
//�ړ�����
enum WAY{UP,DOWN,LEFT,RIGHT};
//�G�̕\��
enum ENEMY_SEE{VISIBLE,INVISIBLE};


//�v���C���[�̍\����
typedef struct gPlayer{
	char name[MAX_NAME_LENGTH];			//���O
	int stamina;						//�̗�
	int attack;							//�U����
	int x;								//X���W
	int y;								//Y���W
	int firstStamina;					//�����̗�
}gPlayer;

//�G�̍\����
typedef struct gEnemy{
	char name[MAX_NAME_LENGTH];			//���O
	int stamina;						//�̗�
	int attack;							//�U����
	int x;								//X���W
	int y;								//Y���W
	int moveType;						//������
	int item;							//�����A�C�e��
	int isSurvived;						//�����Ă��邩�ǂ���
	char icon[3];						//�\�����镶��
}gEnemy;


//�v���g�^�C�v�錾
void displayNameInput();
void setStrNumber();
int checkInputStr(char str[MAX_NAME_LENGTH],int length);
void setPlayerState(int nameLength);
int getStrNumber(char c);
void drawMap();
void setEnemy();
void setEnemyCoord();
int displayPlayingMap();
void displayCharcter();
int movePlayer();
int checkHitEnemy();
void displayPlayerData();
void displayEnemyData(int enemyNumber);
void displayPlayingBattle(int enemyNumber);
int setPlayerAttack(char str[100],int attackLength);
void waitPressedEsc();
void waitPressedEnter();
void displayGameover();
void displayClear();
int checkEnemyExist(int enemy_x,int enemy_y);
void movePursueEnemy();
void displayOperation();
void moveEnemy(int enemyNumber,int moveWay);
void moveRandomEnemy();



//�O���[�o���ϐ�
gPlayer gMyPlayer;						//�v���C���[�̏����i�[����\����
int gStrNumber[36];						//�e�����̐������i�[����z��(0~10,a~z�̏���)
gEnemy gEnemySet[ALL_ENEMY_NUMBER];		//�G�̏����i�[����\���̂̔z��
int gGameState = START;					//�Q�[����ʊǗ��ϐ�
int gEnemyState = INVISIBLE;			//�G�̌���邩�̃t���O

int main(){
	int enemyNumber;				//�G�̔ԍ�

	//�����̏�����
	srand((unsigned)time(NULL));
	//�����ɐ��������蓖�Ă�
	setStrNumber();
	//�G�������ݒ肷��
	setEnemy();
	//�G�̍��W���Z�b�g����
	setEnemyCoord();

	//�Q�[���̃��C�����[�v
	while(gGameState != END){
		//��ʂ��N���A����
		system("cls");
		//�Q�[����ʂɂ�镪��
		switch(gGameState){
			//�X�^�[�g���
			case START:
				displayNameInput();
				break;
			case MAP:
				enemyNumber = displayPlayingMap();
				break;
			case BATTLE:
				displayPlayingBattle(enemyNumber);
				break;
			case GAMEOVER:
				displayGameover();
				break;
			case CLEAR:
				displayClear();
				break;
		}
	}
	
}

//���O���͉�ʂ�\������֐�
void displayNameInput(){
	char input[100];					//���͗p�ϐ�
	int length;							//������
	int inputChecker;					//���͂����������L�����`�F�b�N����t���O�ϐ�


	//���̓��[�v
	do{
		//��ʂ��N���A����
		system("cls");
		//�t���O�̏�����
		inputChecker = 1;
		printf("�v���C���[�̖��O����͂��Ă�������\n");
		printf("�����p�p����20�����܂�\n");
		//���O���̓��[�v
		do{
			printf(">");
			gets(input);
			length = strlen(input);
			if(length > MAX_NAME_LENGTH){
				printf("���͕��������������܂�\n");
			}else if(length == 0){
				printf("�������͂���Ă��܂���\n");
			}else{
				inputChecker = checkInputStr(input,length);
				if(inputChecker == 1){
					printf("�����ȕ������g�p����Ă��܂�\n");
				}
			}
		}while(inputChecker);

		strcpy(gMyPlayer.name, input);
		setPlayerState(length);

		printf("\n");

		//�v���C���[����\������
		displayPlayerData();

		printf("\n\n");

		//y,n���̓��[�v
		do{
			printf("�ȏ�̃X�e�[�^�X�ŗǂ���΁uy�v\n");
			printf("���͂���蒼���ꍇ�́un�v����͂��Ă�������\n");
			printf(">");
			gets(input);
			length = strlen(input);
		}while(length != 1 || (input[0] != 'n' && input[0] != 'y'));
		printf("\n\n");
	}while(input[0] == 'n');

	//�v���C���[�̏����ʒu���Z�b�g
	gMyPlayer.x = 0;
	gMyPlayer.y = 0;

	//�Q�[����ʂɑJ�ڂ���
	gGameState = MAP;
}

//�e�����ɔԍ������蓖�Ă�֐�
void setStrNumber(){
	//11=a,12=b,13=c,14=d,15=e,16=f,17=g,18=h,19=i,20=j,21=k,22=l,23=m,
	//24=n,25=o,26=p,27=q,28=r,29=s,30=t,31=u,32=v,33=w,34=x,35=y,36=z

	int i;			//���[�v�p�ϐ�

	for(i = 0; i < 36; i++){
		if(i == 13 || i == 22){
			//l��c��2�����蓖�Ă�
			gStrNumber[i] = 2;
		}else if(i == 21 || i == 31 || i == 35){
			//u��k��y��1�����蓖�Ă�
			gStrNumber[i] = 1;
		}else{
			//���̑��̓����_����-1~3�����蓖�Ă�
			gStrNumber[i] = rand() % 5 - 1;
		}
	}

	//�f�o�b�O
	/*
	for(i = 0; i < 36; i++){
		printf("gStrNumber[%d] = %d \n",i,gStrNumber[i]);
	}
	*/
}

//�����ȕ����񂪎g���Ă��Ȃ����𔻒肷��֐�
int checkInputStr(char str[20],int length){
	int i;					//���[�v�p�ϐ�
	for(i = 0; i < length; i++){
		if((str[i] < '0' || '9' < str[i]) && (str[i] < 'a' || 'z' < str[i]) && (str[i] < 'A' || 'Z' < str[i])){
			//�����ȕ������g���Ă���̂�1��Ԃ�
			return 1;
		}
	}
	//�����܂ł�����S�ėL���̕����Ȃ̂�0��Ԃ�
	return 0;
}

//�����񂩂�X�e�[�^�X�����肷��֐�
void setPlayerState(int nameLength){
	int i;							//���[�v�p�ϐ�
	int stamina=0;					//�̗͗p�ϐ�
	int bonusCheker[5]={0};			//lucky���܂܂�Ă��邩�`�F�b�N����z��

	for(i = 0; i < nameLength; i++){
		//�̗͂𑝌�������
		stamina += getStrNumber(gMyPlayer.name[i]);
		//lucky���܂܂�Ă��邩���肷��
		switch(gMyPlayer.name[i]){
			case 'l':
				bonusCheker[0] = 1;
				break;
			case 'u':
				bonusCheker[1] = 1;
				break;
			case 'c':
				bonusCheker[2] = 1;
				break;
			case 'k':
				bonusCheker[3] = 1;
				break;
			case 'y':
				bonusCheker[4] = 1;
				break;
		}
	
		//�f�o�b�O
		//printf("stamina = %d\n",stamina);
	}

	//�̗͂�0�ȉ��̏ꍇ
	if(stamina <= 0){
			stamina = 0;
	}else{
		//�̗͂Ƀ��[�g��������
		stamina = stamina * MAX_NAME_LENGTH / nameLength;

		//���O�̕��������V�̔{���̏ꍇ
		if(nameLength % 7 == 0){
			//�̗͂�1.2�{����
			stamina = (int)(stamina * 1.2);
		}
	}
	
	//�v���C���[�̗̑͂�����
	gMyPlayer.stamina = stamina + 10;
	gMyPlayer.firstStamina = gMyPlayer.stamina;

	//�v���C���[�̍U���͂�����
	gMyPlayer.attack = 5;
	//���O��lucky���܂܂�Ă���ꍇ
	if(bonusCheker[0] == 1 && bonusCheker[1] == 1 && bonusCheker[2] == 1 && bonusCheker[3] == 1 && bonusCheker[4] == 1){
		//�U���͂��{�[�i�X�łP���Z
		gMyPlayer.attack++;
	}

}

//���̕����Ɋ��蓖�Ă��Ă���ԍ���Ԃ��֐�
int getStrNumber(char c){
	//0~9�̏ꍇ
	if('0' <= c && c <= '9'){
		return gStrNumber[c-48];
	}
	//�������̏ꍇ
	else if('a' <= c && c <= 'z'){
		return gStrNumber[c-54-32];
	}
	//�啶���̏ꍇ
	else if('A' <= c && c <= 'Z'){
		return gStrNumber[c-54];
	}
	//�����ȕ����̏ꍇ
	else{
		return 0;
	}
}

//�}�b�v��\������֐�
void drawMap(){
	int width,height;					//�g�̕��ƍ���

	//�g�̕\��
	for(height = 0; height < FRAME_HEIGHT; height++){
		//width�~height�ŃR�����g
		for(width = 0; width < FRAME_WIDTH; width++){
			if(height == 0){
				//0�~0
				if(width == 0)					printf("��");
				//0�~�ŏI��
				else if(width == FRAME_WIDTH-1)	printf("��");
				//0�~������
				else if(width % 2 == 0)			printf("��");
				//0�~���
				else							printf("��");
			}else if(height == FRAME_HEIGHT-1){
				//�ŏI�s�~0
				if(width == 0)					printf("��");
				//�ŏI�s�~�ŏI��
				else if(width == FRAME_WIDTH-1)	printf("��");
				//�ŏI�s�~������
				else if(width % 2 == 0)			printf("��");
				//�ŏI�s�~���
				else							printf("��");
			}else if(height % 2 == 0){
				//�����s�~0
				if(width == 0)					printf("��");
				//�����s�~�ŏI��
				else if(width == FRAME_WIDTH-1)	printf("��");
				//�����s�~������
				else if(width % 2 == 0)			printf("��");
				//�����s�~���
				else							printf("��");
			}else{
				//��s�~0
				if(width == 0){
					//�s���̕\��
					printf("��");
				}
				//��s�~������
				else if(width % 2 == 0)			printf("��");
				//��s�~���
				else{
					printf("  ");
				}
			}
		}
		//1��I���̉��s
		printf("\n");
	}
}

//�}�b�v��ʂ�\������֐�
int displayPlayingMap(){
	int moved;										//�ړ������������̃t���O�ϐ�
	int hit;										//�G�ɐڐG�������̃t���O�ϐ�

	//�J�[�\���̕\��������
	CONSOLE_CURSOR_INFO dursorInfo = { 1, FALSE };
	SetConsoleCursorInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &dursorInfo);


	//�}�b�v��\������
	drawMap();
	printf("\n");
	//�v���C���[����\������
	displayPlayerData();
	//���������\������
	displayOperation();
	//�L�����N�^�[��\������
	displayCharcter();

	while(1){
		//�v���C���[�̈ړ�����
		moved = movePlayer();
		//�ړ������������ꍇ
		if(moved == 1){
			//�G�̈ړ�����
			movePursueEnemy();
			moveRandomEnemy();
			//�����蔻��𒲂ׂ�
			hit = checkHitEnemy();
			//�G�ƐڐG�����ꍇ
			if(hit != -1){
				//�o�g����ʂɑJ��
				gGameState = BATTLE;
				//�ڐG�����G�̔ԍ���Ԃ�
				return hit;
			}
		}else if(moved == 2){
			return -1;
		}
	}

	return -1;
}

//�G�̏����ݒ������֐�
void setEnemy(){
	//0~4 = �����Ȃ��G, 5~9 = �ǂ�������G, 10~14 = �����_���ɓ����G, 15 = �{�X
	//�e�G1,2�Ԗڂ̓A�C�e������,3,4�Ԗڂ͉񕜃A�C�e��,5�Ԗڂ͍U���A�b�v�A�C�e��

	int i;											//���[�v�p�ϐ�
	//char c = 'a';
	char num = '1';
	char icon[3];
	icon[2] = '\0';

	//���O,�ړ��^�C�v,�U����,�̗͂��Z�b�g����
	for(i = 0 ;i < ALL_ENEMY_NUMBER; i++){
		if(i < 5){
			strcpy(gEnemySet[i].name , "Slime");
			gEnemySet[i].moveType = STOP;
			gEnemySet[i].attack = SLIME_ATTACK;
			gEnemySet[i].stamina = SLIME_STAMINA + (rand()%3-1);
			//�\�����镶�����Z�b�g
			icon[0] = 'S';
			icon[1] = num;
			strcpy(gEnemySet[i].icon ,icon);
		}else if(i < 10){
			strcpy(gEnemySet[i].name , "Goblin");
			gEnemySet[i].moveType = PURSUE;
			gEnemySet[i].attack = GOBLIN_ATTACK;
			gEnemySet[i].stamina = GOBLIN_STAMINA + (rand()%3-1);
			//�\�����镶�����Z�b�g
			icon[0] = 'G';
			icon[1] = num;
			strcpy(gEnemySet[i].icon ,icon);
		}else if(i < 15){
			strcpy(gEnemySet[i].name , "Harpy");
			gEnemySet[i].moveType = RANDOM;
			gEnemySet[i].attack = HARPY_ATTACK;
			gEnemySet[i].stamina = HARPY_STAMINA + (rand()%3-1);
			//�\�����镶�����Z�b�g
			icon[0] = 'H';
			icon[1] = num;
			strcpy(gEnemySet[i].icon ,icon);
		}else{
			strcpy(gEnemySet[i].name , "Gargoyle");
			gEnemySet[i].moveType = STOP;
			gEnemySet[i].attack = GARGOYLE_ATTACK;
			gEnemySet[i].stamina = GARGOYLE_STAMINA + (rand()%3-1);
			//�\�����镶�����Z�b�g
			icon[0] = 'B';
			icon[1] = 's';
			strcpy(gEnemySet[i].icon ,icon);
		}
		//�S�Ăɐ����Ă����Ԃ��Z�b�g����
		gEnemySet[i].isSurvived = ALIVE;

		if(num < '5'){ 
			num++;
		}else{
			num = '1';
		}
	}

	//�A�C�e�����Z�b�g����
	for(i = 0 ;i < ALL_ENEMY_NUMBER; i++){
		if(i % 5 == 0 || i % 5 == 1){
			gEnemySet[i].item = NOTHING;
		}else if(i % 5 == 2 || i % 5 == 3){
			gEnemySet[i].item = HEAL_ITEM;
		}else{
			gEnemySet[i].item = POWER_ITEM;
		}
	}
}

//�}�b�v�Ɋe�G�L�����̍��W���Z�b�g����֐�
void setEnemyCoord(){
	int map[FIELD_WIDTH][FIELD_HEIGHT]={0};	//�G�z�u�p�ɗp�ӂ����}�b�v
	int countEmpty;							//�u����ꏊ�̐�
	int countEnemy;							//�z�u����G�̔ԍ�
	int enemyNumber = ALL_ENEMY_NUMBER-1;	//�{�X�������S�G�̐�
	int randomNumber;						//�����p�ϐ�
	int x,y;								//���W
	int xTable[FIELD_WIDTH*FIELD_HEIGHT],yTable[FIELD_WIDTH*FIELD_HEIGHT];	//�u����ꏊ���i�[����z��

	//�v���C���[�̍��W���}�b�v�ɃZ�b�g����
	map[gMyPlayer.x][gMyPlayer.y] = 1;
	//�{�X�L�����̍��W���Z�b�g����
	gEnemySet[enemyNumber].x = FIELD_WIDTH-1;
	gEnemySet[enemyNumber].y = FIELD_HEIGHT-1;
	//�{�X�L�����̍��W���}�b�v�ɃZ�b�g����
	map[gEnemySet[enemyNumber].x][gEnemySet[enemyNumber].y] = 1;

	countEnemy = 0;
	//�u���Ă��Ȃ��ꏊ����G��z�u����
	while(countEnemy < enemyNumber){
		countEmpty = 0;
		for(x = 0; x < FIELD_WIDTH; x++){
			for(y = 0; y < FIELD_HEIGHT; y++){
				if(map[x][y] == 0){
					xTable[countEmpty] = x;
					yTable[countEmpty] = y;
					countEmpty++;
				}
			}
		}
		randomNumber = rand()%countEmpty;
		x = xTable[randomNumber];
		y = yTable[randomNumber];
		map[x][y] = 1;
		//�G�̍��W���Z�b�g����
		gEnemySet[countEnemy].x = x;
		gEnemySet[countEnemy].y = y;
		countEnemy++;
	}
}

//�J�[�\���̈ʒu�����W����g�ɍ��킹�ĕϊ�����֐�
void setCursorPosition( const int x, const int y ){
	COORD pos;
	pos.X = x * 4 + 2;
	pos.Y = y * 2 + 1;
	SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), pos );
}

//�}�b�v�ɃL������\������֐�
void displayCharcter(){
	int i;										//���[�v�p�ϐ�

	//�v���C���[�̕\��
	setCursorPosition(gMyPlayer.x, gMyPlayer.y);
	printf("��");

	//�G�����̏ꍇ
	if(gEnemyState == VISIBLE){
		//�G�̕\��
		for(i = 0; i < ALL_ENEMY_NUMBER; i++){
			//�����Ă���G�̂ݕ\������
			if(gEnemySet[i].isSurvived == ALIVE){
				setCursorPosition(gEnemySet[i].x, gEnemySet[i].y);
				printf("%2s",gEnemySet[i].icon);
			}
		}
	}
}

//�v���C���[���ړ�������֐�
int movePlayer(){
	char input;
	//�L�[�{�[�h�̓��͂𒲂ׂ�
	if(kbhit()){
		input = getch();
		switch( input ){
			case '8' :
			case UP_BUTTON:
				if( 0 < gMyPlayer.y){
					setCursorPosition(gMyPlayer.x, gMyPlayer.y);
					printf("  ");
					gMyPlayer.y--;
					setCursorPosition(gMyPlayer.x, gMyPlayer.y);
					printf("��");
					return 1;
				}
				break;
			case '2' :
			case DOWN_BUTTON:
				if( gMyPlayer.y < FIELD_HEIGHT-1){
					setCursorPosition(gMyPlayer.x, gMyPlayer.y);
					printf("  ");
					gMyPlayer.y++;
					setCursorPosition(gMyPlayer.x, gMyPlayer.y);
					printf("��");
					return 1;
				}
				break;
			case '4' :
			case LEFT_BUTTON:
				if( 0 < gMyPlayer.x){
					setCursorPosition(gMyPlayer.x, gMyPlayer.y);
					printf("  ");
					gMyPlayer.x--;
					setCursorPosition(gMyPlayer.x, gMyPlayer.y);
					printf("��");
					return 1;
				}
				break;
			case '6' :
			case RIGHT_BUTTON:
				if( gMyPlayer.x < FIELD_WIDTH-1){
					setCursorPosition(gMyPlayer.x, gMyPlayer.y);
					printf("  ");
					gMyPlayer.x++;
					setCursorPosition(gMyPlayer.x, gMyPlayer.y);
					printf("��");
					return 1;
				}
				break;
			case ESC_BUTTON :
				//�G�̉��s����ύX
				if(gEnemyState == VISIBLE){
					gEnemyState = INVISIBLE;
				}else{
					gEnemyState = VISIBLE;
				}
				return 2;
				break;
		}
	}
	return 0;
}

//�ǐՂ���G���ړ�������֐�
void movePursueEnemy(){
	int i;							//���[�v�p�ϐ�
	int my_x,my_y;					//�v���C���[�̍��W
	int enemy_x,enemy_y;			//�G�̍��W
	int enemyExist;					//�G�����݂��邩�̃t���O�ϐ�
	int difference_x,difference_y;	//�G�ƃv���C���[�̋����̍�
	//�v���C���[�̍��W���擾
	my_x = gMyPlayer.x;
	my_y = gMyPlayer.y;

	for(i = 0; i < ONE_ENEMY_NUMBER; i++){
		//���̓G�������Ă���ꍇ
		if(gEnemySet[i+ONE_ENEMY_NUMBER*1].isSurvived == ALIVE){
			//�G�̍��W���擾
			enemy_x = gEnemySet[i+ONE_ENEMY_NUMBER*1].x;
			enemy_y = gEnemySet[i+ONE_ENEMY_NUMBER*1].y;

			//�G�ƃv���C���[�̋����̍����Ƃ�
			//x�����̍�
			if(enemy_x < my_x){
				difference_x = my_x - enemy_x;
			}else{
				difference_x = enemy_x - my_x;
			}
			//y�����̍�
			if(enemy_y < my_y){
				difference_y = my_y - enemy_y;
			}else{
				difference_y = enemy_y - my_y;
			}

			//x�����̂ق��������傫���ꍇx�����D��
			if(difference_x > difference_y){
				//�v���C���[���G���E�ɂ���ꍇ
				if(enemy_x < my_x){
					//�E�ɓG�����邩���ׂ�
					enemyExist = checkEnemyExist(enemy_x+1,enemy_y);
					if(enemyExist == 0){
						//�E�ֈړ�
						moveEnemy(i+ONE_ENEMY_NUMBER*1,RIGHT);
						continue;
					}
				}
				//�v���C���[���G��荶�ɂ���ꍇ
				else if(enemy_x > my_x){
					//���ɓG�����邩���ׂ�
					enemyExist = checkEnemyExist(enemy_x-1,enemy_y);
					if(enemyExist == 0){
						//�E�ֈړ�
						moveEnemy(i+ONE_ENEMY_NUMBER*1,LEFT);
						continue;
					}
				}

				//�v���C���[���G��艺�ɂ���ꍇ
				if(enemy_y < my_y){
					//���ɓG�����邩���ׂ�
					enemyExist = checkEnemyExist(enemy_x,enemy_y+1);
					if(enemyExist == 0){
						//���ֈړ�
						moveEnemy(i+ONE_ENEMY_NUMBER*1,DOWN);
						continue;
					}
				}
				//�v���C���[���G����ɂ���ꍇ
				else if(enemy_y > my_y){
					//��ɓG�����邩���ׂ�
					enemyExist = checkEnemyExist(enemy_x,enemy_y-1);
					if(enemyExist == 0){
						//��ֈړ�
						moveEnemy(i+ONE_ENEMY_NUMBER*1,UP);
						continue;
					}
				}
			}
			//y�����̂ق��������傫���ꍇy�����D��
			else{
				
				//�v���C���[���G��艺�ɂ���ꍇ
				if(enemy_y < my_y){
					//���ɓG�����邩���ׂ�
					enemyExist = checkEnemyExist(enemy_x,enemy_y+1);
					if(enemyExist == 0){
						//���ֈړ�
						moveEnemy(i+ONE_ENEMY_NUMBER*1,DOWN);
						continue;
					}
				}
				//�v���C���[���G����ɂ���ꍇ
				else if(enemy_y > my_y){
					//��ɓG�����邩���ׂ�
					enemyExist = checkEnemyExist(enemy_x,enemy_y-1);
					if(enemyExist == 0){
						//��ֈړ�
						moveEnemy(i+ONE_ENEMY_NUMBER*1,UP);
						continue;
					}
				}

				//�v���C���[���G���E�ɂ���ꍇ
				if(enemy_x < my_x){
					//�E�ɓG�����邩���ׂ�
					enemyExist = checkEnemyExist(enemy_x+1,enemy_y);
					if(enemyExist == 0){
						//�E�ֈړ�
						moveEnemy(i+ONE_ENEMY_NUMBER*1,RIGHT);
						continue;
					}
				}
				//�v���C���[���G��荶�ɂ���ꍇ
				else if(enemy_x > my_x){
					//���ɓG�����邩���ׂ�
					enemyExist = checkEnemyExist(enemy_x-1,enemy_y);
					if(enemyExist == 0){
						//�E�ֈړ�
						moveEnemy(i+ONE_ENEMY_NUMBER*1,LEFT);
						continue;
					}
				}
			}
		}
	}	
}

//�����_���ړ��̓G���ړ�������֐�
void moveRandomEnemy(){
	int i;							//���[�v�p�ϐ�
	int countEmpty;					//�ړ��\�����̌�
	int movableTable[4];			//�ړ��\����
	int enemy_x,enemy_y;			//�G�̍��W
	int enemyExist;					//�G�����݂��邩�̃t���O�ϐ�
	int random;						//�����p�ϐ�
	int my_x,my_y;					//�v���C���[�̍��W

	//�v���C���[�̍��W���擾
	my_x = gMyPlayer.x;
	my_y = gMyPlayer.y;

	for(i = 0; i < ONE_ENEMY_NUMBER; i++){
		countEmpty = 0;
		//���̓G�������Ă���ꍇ
		if(gEnemySet[i+ONE_ENEMY_NUMBER*2].isSurvived == ALIVE){
			//�G�̍��W���擾
			enemy_x = gEnemySet[i+ONE_ENEMY_NUMBER*2].x;
			enemy_y = gEnemySet[i+ONE_ENEMY_NUMBER*2].y;

			//�v���C���[�ƍ��W����v�����ꍇ�͈ړ����Ȃ�
			if(my_x == enemy_x && my_y == enemy_y){
				continue;
			}

			//������𒲂ׂ�
			if(enemy_y != 0){
				//��ɓG�����邩���ׂ�
				enemyExist = checkEnemyExist(enemy_x,enemy_y-1);
				if(enemyExist == 0){
					movableTable[countEmpty] = UP;
					countEmpty++;
				}
			}
			//�������𒲂ׂ�
			if(enemy_y != FIELD_HEIGHT-1){
				//���ɓG�����邩���ׂ�
				enemyExist = checkEnemyExist(enemy_x,enemy_y+1);
				if(enemyExist == 0){
					movableTable[countEmpty] = DOWN;
					countEmpty++;
				}
			}
			//�������𒲂ׂ�
			if(enemy_x != 0){
				//���ɓG�����邩���ׂ�
				enemyExist = checkEnemyExist(enemy_x-1,enemy_y);
				if(enemyExist == 0){
					movableTable[countEmpty] = LEFT;
					countEmpty++;
				}
			}
			//�E�����𒲂ׂ�
			if(enemy_x != FIELD_WIDTH-1){
				//�E�ɓG�����邩���ׂ�
				enemyExist = checkEnemyExist(enemy_x+1,enemy_y);
				if(enemyExist == 0){
					movableTable[countEmpty] = RIGHT;
					countEmpty++;
				}
			}
			
			//�����_���Ɉړ�
			if(countEmpty != 0){
				random = rand() % countEmpty;
				moveEnemy(i+ONE_ENEMY_NUMBER*2,movableTable[random]);
			}
		}
	}	
}

//�G��\���ɍ��킹�Ĉړ�������
void moveEnemy(int enemyNumber,int moveWay){
	//�G�����̏ꍇ
	if(gEnemyState == VISIBLE){
		setCursorPosition(gEnemySet[enemyNumber].x, gEnemySet[enemyNumber].y);
		printf("  ");
	}
		
	switch(moveWay){
		case UP:
			gEnemySet[enemyNumber].y--;
			break;
		case DOWN:
			gEnemySet[enemyNumber].y++;
			break;
		case LEFT:
			gEnemySet[enemyNumber].x--;
			break;
		case RIGHT:
			gEnemySet[enemyNumber].x++;
			break;
	}
	//�G�����̏ꍇ
	if(gEnemyState == VISIBLE){
		setCursorPosition(gEnemySet[enemyNumber].x, gEnemySet[enemyNumber].y);
		printf("%2s",gEnemySet[enemyNumber].icon);
	}
}
//�G�����̍��W�ɂ��邩�𒲂ׂ�֐�
int checkEnemyExist(int enemy_x,int enemy_y){
	int i;					//���[�v�p�ϐ�

	for(i = 0; i < ALL_ENEMY_NUMBER; i++){
		//���̓G������ł���ꍇ
		if(gEnemySet[i].isSurvived == DEAD){
			continue;
		}else{
			//�������W�̓G������ꍇ
			if(enemy_x == gEnemySet[i].x && enemy_y == gEnemySet[i].y){
				//1��Ԃ�
				return 1;
			}
		}
	}
	//�����܂ł�����G�����Ȃ��̂�0��Ԃ�
	return 0;
}



//�G�Ƃ̓����蔻��𒲂ׂ�֐�
int checkHitEnemy(){
	int i;					//���[�v�p�ϐ�
	int my_x,my_y;			//���L�����̍��W

	//�v���C���[�̍��W���擾����
	my_x = gMyPlayer.x;
	my_y = gMyPlayer.y;

	for(i = 0; i < ALL_ENEMY_NUMBER; i++){
		//���̓G������ł���ꍇ
		if(gEnemySet[i].isSurvived == DEAD){
			continue;
		}else{
			//�������W�̓G������ꍇ
			if(my_x == gEnemySet[i].x && my_y == gEnemySet[i].y){
				//���̓G�̔ԍ���Ԃ�
				return i;
			}
		}
	}

	//�d�Ȃ��Ă��Ȃ����-1��Ԃ�
	return -1;
}

//�v���C���[����\������֐�
void displayPlayerData(){
	printf("�v���C���[���F%s\n",gMyPlayer.name);
	printf("�@�@�̗́@�@�F%d\n",gMyPlayer.stamina);
	printf("�@ �U���� �@�F%d\n",gMyPlayer.attack);
}
//�G�̃X�e�[�^�X��\������
void displayEnemyData(int enemyNumber){
	printf("�@�@���O�@�@�F%s\n",gEnemySet[enemyNumber].name);
	printf("�@�@�̗́@�@�F%d\n",gEnemySet[enemyNumber].stamina);
	printf("�@ �U���� �@�F%d\n",gEnemySet[enemyNumber].attack);
}

//�o�g����ʂ�\������֐�
void displayPlayingBattle(int enemyNumber){
	int damagePoint;						//�_���[�W�Ɖ񕜃|�C���g
	char input[100];						//���͗p�ϐ�
	int random;								//�G�̍U���p�ϐ�

	printf("%s�����ꂽ!!",gEnemySet[enemyNumber].name);
	printf("\n\n");

	//�o�g�����C�����[�v
	while(gGameState == BATTLE){
		
		//�v���C���[�̏���\��
		displayPlayerData();
		printf("\n");

		//�G�̃X�e�[�^�X��\��
		displayEnemyData(enemyNumber);
		printf("\n\n");

		//�v���C���[�̍U��
		printf("%s�̍U��!(�U�����̕�������͂��悤)\n",gMyPlayer.name);
		printf(">");
		gets(input);
		//�_���[�W�v�Z
		damagePoint = setPlayerAttack(input,gMyPlayer.attack);
		if(damagePoint == 0){
			printf("�~�X!!�_���[�W��^�����Ȃ�����!!\n");
		}else{
			printf("%s��%d�̃_���[�W��^����!!\n",gEnemySet[enemyNumber].name,damagePoint);
			//�̗͌��Z
			gEnemySet[enemyNumber].stamina -= damagePoint;
			//�̗͊m�F
			if(gEnemySet[enemyNumber].stamina <= 0){
				printf("%s��|����!!\n",gEnemySet[enemyNumber].name);
				gEnemySet[enemyNumber].isSurvived = DEAD;
				//�{�X���j�̊m�F
				if(enemyNumber == ALL_ENEMY_NUMBER-1){
					//�N���A��ʂɑJ��
					gGameState= CLEAR;
				}else{
					//�A�C�e���m�F
					switch (gEnemySet[enemyNumber].item){
						case NOTHING:
							printf("%s�͉��������Ă��Ȃ������c\n",gEnemySet[enemyNumber].name);
							break;
						case HEAL_ITEM:
							printf("%s�͖򑐂������Ă���!!\n",gEnemySet[enemyNumber].name);
							//�񕜂���|�C���g���v�Z
							damagePoint = HEAL_POINT + (rand()%5-2);
							printf("%s��%d�񕜂���!!\n",gMyPlayer.name,damagePoint);
							gMyPlayer.stamina += damagePoint;
							break;
						case POWER_ITEM:
							printf("%s�͗͂̎�������Ă���!!\n",gEnemySet[enemyNumber].name);
							printf("%s�̍U���͂�1�オ����!!\n",gMyPlayer.name);
							gMyPlayer.attack++;
							break;
					}
					//�}�b�v��ʂɑJ��
					gGameState = MAP;
				}
			}
		}

		printf("\n\n");
		
		//�G�������Ă���ꍇ
		if(gEnemySet[enemyNumber].isSurvived == ALIVE){
			//�G�̍U��
			printf("%s�̍U��!!",gEnemySet[enemyNumber].name);
			random = rand() % 100;
			//25%�̊m���Ń~�X
			if(0 <= random && random < 25){
				printf("�~�X!!�_���[�W��^�����Ȃ�����!!\n");
				damagePoint = 0;
			}else{
				//65%�̊m���Œʏ�_���[�W
				if(25 <= random && random < 90){
					//�_���[�W���v�Z
					damagePoint = gEnemySet[enemyNumber].attack+ (rand()%3-1);
					printf("%s��%d�̃_���[�W��^����!!\n",gMyPlayer.name,damagePoint);
				}
				//10%�̊m���ŃN���e�B�J��
				else{
					//�_���[�W���v�Z
					damagePoint = gEnemySet[enemyNumber].attack+2;
					printf("�N���e�B�J���q�b�g!!%s��%d�̃_���[�W��^����!!\n",gMyPlayer.name,damagePoint);
				}
				//�̗͌��Z
				gMyPlayer.stamina -= damagePoint;
				//�̗͊m�F
				if(gMyPlayer.stamina <= 0){
					printf("%s�͂���Ă��܂����c\n",gMyPlayer.name);
					//�Q�[���I�[�o�[��ʂɑJ��
					gGameState = GAMEOVER;
				}
			}
		}

		printf("\n");
		printf("�uEnter�v�Ŏ��ɐi�݂܂��B");
		//�m�F��҂�
		waitPressedEnter();

		//��ʂ��N���A����
		system("cls");
	}
}

//�v���C���[���^����_���[�W�����肷��֐�
int setPlayerAttack(char str[100],int attackLength){
	int i;							//���[�v�p�ϐ�
	int damagePoint = 0;			//�_���[�W

	for(i = 0; i < attackLength; i++){
		//�̗͂𑝌�������
		damagePoint += getStrNumber(str[i]);
	}

	//0�ȉ��̏ꍇ��0�ɂ���
	if(damagePoint < 0){
		damagePoint = 0;
	}

	return damagePoint;
}

//�Q�[���I�[�o�[��ʂ�\������
void displayGameover(){
	printf("���̌�%s��m����̂͂��Ȃ������c\n\n",gMyPlayer.name);
	//�Q�[�����I������
	gGameState = END;
}

//�N���A��ʂ�\������
void displayClear(){
	int staminaPoint;		//�̗̓|�C���g
	int attackPoint;		//�U���̓|�C���g
	int luckyPoint;			//�^���|�C���g
	printf("�uEnter�v�Ŏ��ɐi�݂܂��B\n\n");
	printf("�N���A���߂łƂ��������܂�!!\n");
	printf("�Ō��%s����̉^����肢�܂��傤!\n\n",gMyPlayer.name);
	//�m�F��҂�
	waitPressedEnter();

	//�c��̗�-�����̗�
	staminaPoint = gMyPlayer.stamina - gMyPlayer.firstStamina;
	//�U���́~2
	attackPoint = gMyPlayer.attack * 2;
	//�^���|�C���g�̌v�Z
	luckyPoint = staminaPoint + attackPoint;

	printf("���Ȃ��̃��b�L�[�|�C���g��%d�ł�\n\n",luckyPoint);

	//�m�F��҂�
	waitPressedEnter();

	//�^���̕\��
	if(10 < luckyPoint ){
		printf("�Ȃ�Ă��Ȃ��͉^��������ł��傤��!!\n");
		printf("�����Ƃ��̌���f�G�Ȃ��Ƃ����邱�Ƃł��傤!!\n");	
	}else if(-10 < luckyPoint){
		printf("���������Ƃ������Ƃ���ł��傤���B\n");
		printf("�����ʂ�ȂP���ɂȂ�ł��傤�ˁB\n");	
	}else{
		printf("���₨��U�X�ł������H\n");
		printf("�ł��N���A�ł��������ǂ����Ƃ�����܂���B\n");	
	}

	printf("�����l�ł����B�uEsc�v�ŏI�����܂�\n");
	//�m�F��҂�
	waitPressedEsc();

	//�Q�[�����I������
	gGameState = END;
}

//ESC�������܂ő҂֐�
void waitPressedEsc(){
	char input;			//���͂����L�[
	
	//�m�F��҂��[�v
	while(1){
		if(kbhit()){
			input = getch();
			if(input == ESC_BUTTON){
				break;
			}
		}
	}
}

//Enter�������܂ő҂֐�
void waitPressedEnter(){
	char input;			//���͂����L�[
	
	//�m�F��҂��[�v
	while(1){
		if(kbhit()){
			input = getch();
			if(input == ENTER_BUTTON){
				break;
			}
		}
	}
}

//������@��\������֐�
void displayOperation(){
	COORD pos;					//���W

	pos.X = 21 * 2 + 2;
	pos.Y = 1;
	SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), pos );
	printf("������@");

	pos.X = 21 * 2 + 1 + 4;
	pos.Y = 2;
	SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), pos );
	printf("��:�W");

	pos.X = 21 * 2 + 1;
	pos.Y = 3;
	SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), pos );
	printf("��:�S�@���F�U");

	pos.X = 21 * 2 + 1 + 4;
	pos.Y = 4;
	SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), pos );
	printf("��:�Q");

	pos.X = 21 * 2 + 1 + 1;
	pos.Y = 5;
	SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), pos );
	printf("Esc:�G�̉��؂�ւ�");
}





	










	
	
